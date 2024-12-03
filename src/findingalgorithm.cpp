#include "findingalgorithm.h"

// Tính khoảng cách giữa hai điểm sử dụng công thức Haversine
double FindingAlgorithm::calculateDistance(double lat1, double lon1, double lat2, double lon2)
{
    const double R = 6371.0; // Bán kính Trái Đất (km)
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
                   sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

// Thuật toán Dijkstra
std::vector<int> FindingAlgorithm::dijkstra(int start, int end, std::vector<double> &distances)
{
    int n = nodes.size();
    distances.assign(n, std::numeric_limits<double>::max());
    std::vector<int> previous(n, -1);
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>>
        pq;

    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty())
    {
        int current = pq.top().second;
        double currentDist = pq.top().first;
        pq.pop();

        if (current == end)
            break;

        for (const auto &edge : adjacencyList[current])
        {
            double newDist = currentDist + edge.second;
            if (newDist < distances[edge.first])
            {
                distances[edge.first] = newDist;
                previous[edge.first] = current;
                pq.push({newDist, edge.first});
            }
        }
    }

    return previous;
}

// Thuật toán A*
std::vector<int> FindingAlgorithm::aStar(int start, int end)
{
    int n = nodes.size();
    std::vector<double> gCost(n, std::numeric_limits<double>::max());
    std::vector<double> fCost(n, std::numeric_limits<double>::max());
    std::vector<int> previous(n, -1);
    std::priority_queue<std::pair<double, int>,
                        std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>>
        pq;

    gCost[start] = 0;
    fCost[start] = calculateDistance(nodes[start].lat, nodes[start].lng, nodes[end].lat, nodes[end].lng);
    pq.push({fCost[start], start});

    while (!pq.empty())
    {
        int current = pq.top().second;
        pq.pop();

        if (current == end)
            break;

        for (const auto &edge : adjacencyList[current])
        {
            double tentativeG = gCost[current] + edge.second;
            if (tentativeG < gCost[edge.first])
            {
                gCost[edge.first] = tentativeG;
                fCost[edge.first] = tentativeG +
                                    calculateDistance(nodes[edge.first].lat, nodes[edge.first].lng,
                                                       nodes[end].lat, nodes[end].lng);
                previous[edge.first] = current;
                pq.push({fCost[edge.first], edge.first});
            }
        }
    }

    return previous;
}

// Xây dựng đường đi từ vector `previous`
std::vector<Node> FindingAlgorithm::reconstructPath(const std::vector<int> &previous, int end)
{
    std::vector<Node> path;
    for (int at = end; at != -1; at = previous[at])
    {
        path.push_back(nodes[at]);
    }
    std::reverse(path.begin(), path.end());
    return path;
}
