#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "routegraph.h"

#include <QQuickItem>
#include <QIcon>
#include <vector>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), routeGraph(new RouteGraph())
{
    ui->setupUi(this);
    setupStyles();
    setupMap();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete routeGraph;
}

void MainWindow::setupStyles()
{
    ui->fromLineEdit->setPlaceholderText("From");
    ui->toLineEdit->setPlaceholderText("To");

    ui->distanceLabel->setVisible(false);
    ui->timeLabel->setVisible(false);

    QSize iconSize(28, 28);
    ui->searchButton->setIcon(QIcon(":/styles/search.jpg"));
    ui->searchButton->setIconSize(iconSize);
    ui->searchButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    ui->mapQuickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->mapQuickWidget->show();
}

void MainWindow::setupMap()
{
    auto obj = ui->mapQuickWidget->rootObject();
    connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
    connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj, SLOT(addMarker(QVariant, QVariant)));
    connect(this, SIGNAL(drawRoute(QVariant)), obj, SLOT(drawRoute(QVariant)));

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

    int haiphong = routeGraph->addNode(20.844912, 106.688084, "Hải Phòng");
    int caobang = routeGraph->addNode(22.665418, 106.258111, "Cao Bằng");
    int langson = routeGraph->addNode(21.853708, 106.761519, "Lạng Sơn");
    int laocai = routeGraph->addNode(22.485927, 103.975163, "Lào Cai");
    int dienbien = routeGraph->addNode(21.385935, 103.017731, "Điện Biên");
    int sonla = routeGraph->addNode(21.327667, 103.919266, "Sơn La");
    int hagiang = routeGraph->addNode(22.823477, 104.978103, "Hà Giang");

    routeGraph->addEdge(hanoi, ninhbinh, 93.7);
    routeGraph->addEdge(ninhbinh, thanhhoa, 140.0);
    routeGraph->addEdge(thanhhoa, vinh, 180.0);
    routeGraph->addEdge(vinh, hatinh, 50.0);
    routeGraph->addEdge(hatinh, dongha, 280.0);
    routeGraph->addEdge(dongha, hue, 70.0);
    routeGraph->addEdge(hue, danang, 108.0);
    routeGraph->addEdge(danang, quangngai, 130.0);
    routeGraph->addEdge(quangngai, quynhon, 210.0);
    routeGraph->addEdge(quynhon, tuyhoa, 120.0);
    routeGraph->addEdge(tuyhoa, nhatrang, 120.0);
    routeGraph->addEdge(nhatrang, phanthiet, 230.0);
    routeGraph->addEdge(phanthiet, hochiminh, 200.0);
    routeGraph->addEdge(hochiminh, vungtau, 95.0);

    routeGraph->addEdge(hanoi, haiphong, 105.0);
    routeGraph->addEdge(hanoi, langson, 154.0);
    routeGraph->addEdge(langson, caobang, 170.0);
    routeGraph->addEdge(hanoi, laocai, 245.0);
    routeGraph->addEdge(hanoi, hagiang, 320.0);
    routeGraph->addEdge(hanoi, sonla, 320.0);
    routeGraph->addEdge(sonla, dienbien, 180.0);

    vector<Node> route = routeGraph->findShortestPath(hanoi, hochiminh);

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
