#ifndef ROUTEGRAPH_H
#define ROUTEGRAPH_H

#include "findingalgorithm.h"

class RouteGraph : public FindingAlgorithm
{
public:
    int addNode(double lat, double lng, const QString &name);
    void addEdge(int from, int to, double roadDistance = -1);

    // Ghi đè phương thức tìm đường ngắn nhất (sử dụng Dijkstra mặc định)
    std::vector<Node> findShortestPath(int start, int end) override;
};

#endif // ROUTEGRAPH_H
