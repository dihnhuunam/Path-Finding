#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQuickItem>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;
// Helper struct for graph nodes
struct Node
{
    double lat;
    double lng;
    QString name; // Thêm tên địa điểm
};

// Graph for routing
class RouteGraph
{
private:
    vector<Node> nodes;
    vector<vector<pair<int, double>>> adjacencyList;

    // Haversine formula to calculate distance between two coordinates
    double haversineDistance(double lat1, double lon1, double lat2, double lon2)
    {
        const double R = 6371.0; // Earth radius in kilometers
        double dLat = (lat2 - lat1) * M_PI / 180.0;
        double dLon = (lon2 - lon1) * M_PI / 180.0;
        double a = sin(dLat / 2) * sin(dLat / 2) +
                   cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
                       sin(dLon / 2) * sin(dLon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        return R * c;
    }

public:
    // Add a node to the graph
    int addNode(double lat, double lng, const QString &name)
    {
        nodes.push_back({lat, lng, name});
        adjacencyList.push_back({});
        return nodes.size() - 1;
    }

    // Add an edge between two nodes with actual road distance
    void addEdge(int from, int to, double roadDistance = -1)
    {
        // If road distance is not provided, calculate using haversine
        if (roadDistance < 0)
        {
            roadDistance = haversineDistance(
                nodes[from].lat, nodes[from].lng,
                nodes[to].lat, nodes[to].lng);
            // Add 20% to account for road curvature
            roadDistance *= 1.2;
        }
        adjacencyList[from].push_back({to, roadDistance});
        adjacencyList[to].push_back({from, roadDistance}); // Undirected graph
    }

    // Get node information
    Node getNode(int index) const
    {
        return nodes[index];
    }

    // Dijkstra's algorithm to find shortest path
    vector<Node> findShortestPath(int start, int end)
    {
        int n = nodes.size();
        vector<double> dist(n, numeric_limits<double>::max());
        vector<int> prev(n, -1);
        priority_queue<pair<double, int>,
                       vector<pair<double, int>>,
                       greater<pair<double, int>>>
            pq;

        dist[start] = 0;
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
                if (newDist < dist[edge.first])
                {
                    dist[edge.first] = newDist;
                    prev[edge.first] = current;
                    pq.push({newDist, edge.first});
                }
            }
        }

        // Reconstruct path
        vector<Node> path;
        for (int at = end; at != -1; at = prev[at])
        {
            path.push_back(nodes[at]);
        }
        reverse(path.begin(), path.end());

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

    // Thêm các thành phố và điểm trung gian chính
    int hanoi = routeGraph->addNode(21.012915, 105.85667, "Hà Nội");
    int ninhbinh = routeGraph->addNode(20.250676, 105.974060, "Ninh Bình");
    int thanhhoa = routeGraph->addNode(19.806665, 105.784873, "Thanh Hóa");
    int vinh = routeGraph->addNode(18.679585, 105.681335, "Vinh");
    int hatinh = routeGraph->addNode(18.340490, 105.897757, "Hà Tĩnh");
    int dongha = routeGraph->addNode(16.815561, 107.104056, "Đông Hà");
    int hue = routeGraph->addNode(16.461381, 107.590169, "Huế");
    int danang = routeGraph->addNode(16.047079, 108.206230, "Đà Nẵng");
    int quangngai = routeGraph->addNode(15.121460, 108.804089, "Quảng Ngãi");
    int quynhon = routeGraph->addNode(13.775398, 109.223590, "Quy Nhơn");
    int tuyhoa = routeGraph->addNode(13.095720, 109.310333, "Tuy Hòa");
    int nhatrang = routeGraph->addNode(12.248236, 109.189679, "Nha Trang");
    int phanthiet = routeGraph->addNode(10.928891, 108.100487, "Phan Thiết");
    int vungtau = routeGraph->addNode(10.346577, 107.084417, "Vũng Tàu");
    int hochiminh = routeGraph->addNode(10.762622, 106.660172, "TP.HCM");

    // Thêm các đường cao tốc
    int haiphong = routeGraph->addNode(20.844912, 106.688084, "Hải Phòng");
    int caobang = routeGraph->addNode(22.665418, 106.258111, "Cao Bằng");
    int langson = routeGraph->addNode(21.853708, 106.761519, "Lạng Sơn");
    int laocai = routeGraph->addNode(22.485927, 103.975163, "Lào Cai");
    int dienbien = routeGraph->addNode(21.385935, 103.017731, "Điện Biên");
    int sonla = routeGraph->addNode(21.327667, 103.919266, "Sơn La");
    int hagiang = routeGraph->addNode(22.823477, 104.978103, "Hà Giang");

    // Thêm các kết nối theo quốc lộ 1A và đường cao tốc
    routeGraph->addEdge(hanoi, ninhbinh, 93.7);       // QL1A
    routeGraph->addEdge(ninhbinh, thanhhoa, 140.0);   // QL1A
    routeGraph->addEdge(thanhhoa, vinh, 180.0);       // QL1A
    routeGraph->addEdge(vinh, hatinh, 50.0);          // QL1A
    routeGraph->addEdge(hatinh, dongha, 280.0);       // QL1A
    routeGraph->addEdge(dongha, hue, 70.0);           // QL1A
    routeGraph->addEdge(hue, danang, 108.0);          // QL1A
    routeGraph->addEdge(danang, quangngai, 130.0);    // QL1A
    routeGraph->addEdge(quangngai, quynhon, 210.0);   // QL1A
    routeGraph->addEdge(quynhon, tuyhoa, 120.0);      // QL1A
    routeGraph->addEdge(tuyhoa, nhatrang, 120.0);     // QL1A
    routeGraph->addEdge(nhatrang, phanthiet, 230.0);  // QL1A
    routeGraph->addEdge(phanthiet, hochiminh, 200.0); // QL1A
    routeGraph->addEdge(hochiminh, vungtau, 95.0);    // QL51

    // Thêm các kết nối cao tốc và quốc lộ khác
    routeGraph->addEdge(hanoi, haiphong, 105.0);  // CT.05
    routeGraph->addEdge(hanoi, langson, 154.0);   // CT.03
    routeGraph->addEdge(langson, caobang, 170.0); // QL4A
    routeGraph->addEdge(hanoi, laocai, 245.0);    // CT.05
    routeGraph->addEdge(hanoi, hagiang, 320.0);   // QL2
    routeGraph->addEdge(hanoi, sonla, 320.0);     // QL6
    routeGraph->addEdge(sonla, dienbien, 180.0);  // QL6

    // Tìm đường từ Hà Nội đến TP.HCM
    vector<Node> route = routeGraph->findShortestPath(laocai, vinh);

    // Gửi tọa độ route để vẽ trên bản đồ
    QVariantList routeCoordinates;
    for (const auto &node : route)
    {
        routeCoordinates.append(node.lat);
        routeCoordinates.append(node.lng);
    }
    emit drawRoute(routeCoordinates);

    // Đặt tâm bản đồ tại Hà Nội
    emit setCenter(21.012915, 105.85667);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete routeGraph;
}