#include "routegraph.h"

int RouteGraph::addNode(double lat, double lng, const QString &name)
{
    nodes.push_back({lat, lng, name});
    adjacencyList.push_back({});
    return nodes.size() - 1;
}

void RouteGraph::addEdge(int from, int to, double roadDistance)
{
    if (roadDistance < 0)
    {
        roadDistance = calculateDistance(
            nodes[from].lat, nodes[from].lng,
            nodes[to].lat, nodes[to].lng);
        roadDistance *= 1.2;
    }
    adjacencyList[from].push_back({to, roadDistance});
    adjacencyList[to].push_back({from, roadDistance});
}

std::vector<Node> RouteGraph::findShortestPath(int start, int end)
{
    std::vector<double> distances;
    std::vector<int> previous = dijkstra(start, end, distances);
    return reconstructPath(previous, end);
}
