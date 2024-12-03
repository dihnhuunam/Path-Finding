#ifndef FINDINGALGORITHM_H
#define FINDINGALGORITHM_H

#include <vector>
#include <QString>

struct Node
{
    double lat, lng;
    QString name;
};

class FindingAlgorithm
{
public:
    virtual ~FindingAlgorithm() = default;
    virtual std::vector<Node> findShortestPath(int start, int end) = 0;

protected:
    std::vector<Node> nodes;
    std::vector<std::vector<std::pair<int, double>>> adjacencyList;

    double calculateDistance(double lat1, double lon1, double lat2, double lon2);
    std::vector<int> dijkstra(int start, int end, std::vector<double> &distances);
    std::vector<int> aStar(int start, int end);
    std::vector<Node> reconstructPath(const std::vector<int> &previous, int end);
};

#endif // FINDINGALGORITHM_H
