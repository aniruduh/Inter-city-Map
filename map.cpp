#include <bits/stdc++.h>
using namespace std;

class CityMap {
public:
    unordered_map<string, int> cityIndex;
    unordered_map<int, string> indexToCity;
    vector<vector<pair<int, int>>> dayGraph;  // adjacency list for day
    vector<vector<pair<int, int>>> nightGraph;  // adjacency list for night
    int cityCount = 0;

    void addCity(const string &cityName) {
        if (cityIndex.find(cityName) == cityIndex.end()) {
            cityIndex[cityName] = cityCount;
            indexToCity[cityCount] = cityName;
            cityCount++;
            dayGraph.emplace_back();   // Add a new row for day graph
            nightGraph.emplace_back(); // Add a new row for night graph
        }
    }

    void addEdge(bool day, const string &city1, const string &city2, int weight) {
        int index1 = cityIndex[city1];
        int index2 = cityIndex[city2];
        if (day) {
            dayGraph[index1].emplace_back(index2, weight);
            dayGraph[index2].emplace_back(index1, weight);
        } else {
            nightGraph[index1].emplace_back(index2, weight);
            nightGraph[index2].emplace_back(index1, weight);
        }
    }

    // Optimized Dijkstra's algorithm using adjacency list and priority queue
    vector<int> dijkstra(bool day, const string &startCity) {
        int startIdx = cityIndex[startCity];
        vector<int> dist(cityCount, INT_MAX);
        dist[startIdx] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push({0, startIdx});

        const auto &graph = day ? dayGraph : nightGraph;

        while (!pq.empty()) {
            auto [currentDist, u] = pq.top(); pq.pop();

            if (currentDist > dist[u]) continue;

            for (auto &[v, weight] : graph[u]) {
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        return dist;
    }

    vector<vector<int>> floydWarshall(bool day) {
        vector<vector<int>> dist(cityCount, vector<int>(cityCount, INT_MAX));

        // Initialize distances based on the day or night graph
        for (int i = 0; i < cityCount; ++i) dist[i][i] = 0;
        const auto &graph = day ? dayGraph : nightGraph;

        for (int u = 0; u < cityCount; ++u) {
            for (auto &[v, weight] : graph[u]) {
                dist[u][v] = weight;
                dist[v][u] = weight;
            }
        }

        // Run Floyd-Warshall algorithm
        for (int k = 0; k < cityCount; ++k) {
            for (int i = 0; i < cityCount; ++i) {
                for (int j = 0; j < cityCount; ++j) {
                    if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
        return dist;
    }

    // Helper function to print the shortest path distances
    void printShortestPaths(const vector<int>& dist) const {
        for (int i = 0; i < cityCount; ++i) {
            cout << "City: " << indexToCity.at(i) << ", Distance: " << dist[i] << "\n";
        }
    }

    // Helper function for Floyd-Warshall output
    void printAllPairsShortestPaths(const vector<vector<int>>& distMatrix) const {
        for (int i = 0; i < cityCount; ++i) {
            for (int j = 0; j < cityCount; ++j) {
                if (i == j) continue;
                if (distMatrix[i][j] == INT_MAX) {
                    cout << "No route from " << indexToCity.at(i) << " to " << indexToCity.at(j) << "\n";
                } else {
                    cout << "Shortest distance from " << indexToCity.at(i) << " to " << indexToCity.at(j) << " is " << distMatrix[i][j] << "\n";
                }
            }
        }
    }
};

// User interface to handle queries
void queryHandler(CityMap& MAP) {
    cout << "Enter the current city name: ";
    string currCity; cin >> currCity;

    cout << "Enter 1 for shortest path from the current city, 2 for shortest distance between all pairs: ";
    int query; cin >> query;

    cout << "Is it day or night? ";
    string time; cin >> time;
    bool isDay = (time == "day" || time == "Day");

    if (query == 1) {
        cout << "Enter destination city name: ";
        string destCity; cin >> destCity;
        vector<int> dist = MAP.dijkstra(isDay, currCity);
        int destIdx = MAP.cityIndex[destCity];
        cout << "Shortest distance from " << currCity << " to " << destCity << " is " << dist[destIdx] << "\n";
    } else if (query == 2) {
        vector<vector<int>> distMatrix = MAP.floydWarshall(isDay);
        MAP.printAllPairsShortestPaths(distMatrix);
    } else {
        cout << "Invalid query\n";
    }
}

// Main function to solve the problem
void solve() {
    CityMap MAP;

    // Add cities
    vector<string> cities = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S"};
    for (const string &city : cities) {
        MAP.addCity(city);
    }

    // Add day edges
    MAP.addEdge(true, "A", "G", 220);
    MAP.addEdge(true, "B", "O", 650);
    MAP.addEdge(true, "C", "I", 420);
    MAP.addEdge(true, "D", "J", 330);
    // Add more edges as needed...

    // Add night edges
    MAP.addEdge(false, "A", "G", 250);
    MAP.addEdge(false, "B", "H", 700);
    MAP.addEdge(false, "C", "I", 450);
    // Add more edges as needed...

    // Handle queries from the user
    queryHandler(MAP);
}

int main() {
    solve();
    return 0;
}
