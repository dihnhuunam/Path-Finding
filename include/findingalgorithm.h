#ifndef FINDINGALGORITHM_H
#define FINDINGALGORITHM_H

#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <QString>

// Node struct lưu thông tin địa điểm
struct Node
{
    double lat;
    double lng;
    QString name;
};

class FindingAlgorithm
{
public:
    virtual ~FindingAlgorithm() = default;

    // Hàm thuần ảo, lớp con phải triển khai
    virtual std::vector<Node> findShortestPath(int start, int end) = 0;

protected:
    std::vector<Node> nodes; // Danh sách các node
    std::vector<std::vector<std::pair<int, double>>> adjacencyList; // Danh sách kề

    // Hàm tính khoảng cách giữa hai tọa độ
    double calculateDistance(double lat1, double lon1, double lat2, double lon2);

    // Thuật toán Dijkstra
    std::vector<int> dijkstra(int start, int end, std::vector<double> &distances);

    // Thuật toán A*
    std::vector<int> aStar(int start, int end);

    // Hàm xây dựng đường đi từ vector `previous`
    std::vector<Node> reconstructPath(const std::vector<int> &previous, int end);
};

#endif // FINDINGALGORITHM_H