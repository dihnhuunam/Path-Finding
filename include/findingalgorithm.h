#ifndef FINDINGALGORITHM_H
#define FINDINGALGORITHM_H

#include <vector>
#include <string>
#include <QString>

struct Node
{
    double lat;
    double lng;
    QString name;
};

class FindingAlgorithm
{
protected:
    std::vector<Node> nodes;
    std::vector<std::vector<std::pair<int, double>>> adjacencyList;

public:
    static double calculateDistance(double lat1, double lon1, double lat2, double lon2);

private:
    std::vector<int> aStar(int start, int end);
    std::vector<Node> reconstructPath(const std::vector<int> &previous, int end);

public:
    std::vector<Node> findShortestPath(int start, int end);

    friend class MainWindow;
};

#endif // FINDINGALGORITHM_H