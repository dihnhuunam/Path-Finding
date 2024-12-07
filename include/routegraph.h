#ifndef ROUTEGRAPH_H
#define ROUTEGRAPH_H

#include <vector>
#include <string>
#include <QString>
#include "findingalgorithm.h"

class RouteGraph : public FindingAlgorithm {
public:
    int addNode(double lat, double lng, const QString &name);
    void addEdge(int from, int to, double roadDistance = -1);
    std::vector<Node> findShortestPath(int start, int end);
};

#endif // ROUTEGRAPH_H