#include "routegraph.h"

int RouteGraph::addNode(double lat, double lng, const QString &name)
{
    nodes.push_back({lat, lng, name});
    adjacencyList.emplace_back();
    return nodes.size() - 1;
}

void RouteGraph::addEdge(int from, int to, double roadDistance)
{
    if (roadDistance < 0)
        roadDistance = calculateDistance(
                           nodes[from].lat, nodes[from].lng, nodes[to].lat, nodes[to].lng) *
                       1.2;

    adjacencyList[from].emplace_back(to, roadDistance);
    adjacencyList[to].emplace_back(from, roadDistance);
}

std::vector<Node> RouteGraph::findShortestPath(int start, int end)
{
    std::vector<double> distances;
    auto previous = dijkstra(start, end, distances);
    return reconstructPath(previous, end);
}
