#include "findingalgorithm.h"
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>

// Haversine Formula
double FindingAlgorithm::calculateDistance(double lat1, double lon1, double lat2, double lon2)
{
    constexpr double R = 6371.0; // Earth radius (km)
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
                   sin(dLon / 2) * sin(dLon / 2);
    return R * 2 * atan2(sqrt(a), sqrt(1 - a));
}

// Dijkstra Algorithm
std::vector<int> FindingAlgorithm::dijkstra(int start, int end, std::vector<double> &distances)
{
    int n = nodes.size();
    distances.assign(n, std::numeric_limits<double>::max());
    std::vector<int> previous(n, -1);
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> pq;

    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty())
    {
        auto [currentDist, current] = pq.top();
        pq.pop();
        if (current == end)
            break;

        for (auto &[next, weight] : adjacencyList[current])
        {
            double newDist = currentDist + weight;
            if (newDist < distances[next])
            {
                distances[next] = newDist;
                previous[next] = current;
                pq.push({newDist, next});
            }
        }
    }
    return previous;
}

// A* Algorithm
std::vector<int> FindingAlgorithm::aStar(int start, int end)
{
    int n = nodes.size();
    std::vector<double> gCost(n, std::numeric_limits<double>::max());
    std::vector<double> fCost(n, std::numeric_limits<double>::max());
    std::vector<int> previous(n, -1);
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> pq;

    gCost[start] = 0;
    fCost[start] = calculateDistance(nodes[start].lat, nodes[start].lng, nodes[end].lat, nodes[end].lng);
    pq.push({fCost[start], start});

    while (!pq.empty())
    {
        auto [_, current] = pq.top();
        pq.pop();
        if (current == end)
            break;

        for (auto &[next, weight] : adjacencyList[current])
        {
            double tentativeG = gCost[current] + weight;
            if (tentativeG < gCost[next])
            {
                gCost[next] = tentativeG;
                fCost[next] = tentativeG + calculateDistance(
                                               nodes[next].lat, nodes[next].lng, nodes[end].lat, nodes[end].lng);
                previous[next] = current;
                pq.push({fCost[next], next});
            }
        }
    }
    return previous;
}

// Reconstruct Path
std::vector<Node> FindingAlgorithm::reconstructPath(const std::vector<int> &previous, int end)
{
    std::vector<Node> path;
    for (int at = end; at != -1; at = previous[at])
        path.push_back(nodes[at]);
    std::reverse(path.begin(), path.end());
    return path;
}
