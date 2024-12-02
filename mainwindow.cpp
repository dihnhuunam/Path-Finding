#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQuickItem>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>

// Helper struct for graph nodes
struct Node {
    double lat;
    double lng;
};

// Graph for routing
class RouteGraph {
private:
    std::vector<Node> nodes;
    std::vector<std::vector<std::pair<int, double>>> adjacencyList;

    // Haversine formula to calculate distance between two coordinates
    double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
        const double R = 6371.0; // Earth radius in kilometers
        double dLat = (lat2 - lat1) * M_PI / 180.0;
        double dLon = (lon2 - lon1) * M_PI / 180.0;
        double a = std::sin(dLat/2) * std::sin(dLat/2) +
                   std::cos(lat1 * M_PI / 180.0) * std::cos(lat2 * M_PI / 180.0) *
                   std::sin(dLon/2) * std::sin(dLon/2);
        double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
        return R * c;
    }

public:
    // Add a node to the graph
    int addNode(double lat, double lng) {
        nodes.push_back({lat, lng});
        adjacencyList.push_back({});
        return nodes.size() - 1;
    }

    // Add an edge between two nodes
    void addEdge(int from, int to) {
        double dist = haversineDistance(
            nodes[from].lat, nodes[from].lng, 
            nodes[to].lat, nodes[to].lng
        );
        adjacencyList[from].push_back({to, dist});
        adjacencyList[to].push_back({from, dist}); // Undirected graph
    }

    // Dijkstra's algorithm to find shortest path
    std::vector<Node> findShortestPath(int start, int end) {
        int n = nodes.size();
        std::vector<double> dist(n, std::numeric_limits<double>::max());
        std::vector<int> prev(n, -1);
        std::priority_queue<std::pair<double, int>, 
                             std::vector<std::pair<double, int>>, 
                             std::greater<std::pair<double, int>>> pq;

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int current = pq.top().second;
            double currentDist = pq.top().first;
            pq.pop();

            if (current == end) break;

            for (const auto& edge : adjacencyList[current]) {
                double newDist = currentDist + edge.second;
                if (newDist < dist[edge.first]) {
                    dist[edge.first] = newDist;
                    prev[edge.first] = current;
                    pq.push({newDist, edge.first});
                }
            }
        }

        // Reconstruct path
        std::vector<Node> path;
        for (int at = end; at != -1; at = prev[at]) {
            path.push_back(nodes[at]);
        }
        std::reverse(path.begin(), path.end());

        return path;
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), routeGraph(new RouteGraph())
{
    ui->setupUi(this);

    ui->mapQuickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->mapQuickWidget->show();

    auto obj = ui->mapQuickWidget->rootObject();
    connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
    connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj, SLOT(addMarker(QVariant, QVariant)));
    connect(this, SIGNAL(drawRoute(QVariant)), obj, SLOT(drawRoute(QVariant)));

    // Example node setup - replace with your actual locations
    int hanoi = routeGraph->addNode(21.012915, 105.85667);
    int hochiminh = routeGraph->addNode(10.762622, 106.660172);
    int hue = routeGraph->addNode(16.461381, 107.590169);

    // Add some example connections
    routeGraph->addEdge(hanoi, hue);
    routeGraph->addEdge(hue, hochiminh);

    // Find route example
    std::vector<Node> route = routeGraph->findShortestPath(hanoi, hochiminh);

    // Send route to QML for drawing
    QVariantList routeCoordinates;
    for (const auto& node : route) {
        routeCoordinates.append(node.lat);
        routeCoordinates.append(node.lng);
    }
    emit drawRoute(routeCoordinates);

    // Initial map center
    emit setCenter(21.012915, 105.85667);
    emit addMarker(21.012915, 105.85667);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete routeGraph;
}