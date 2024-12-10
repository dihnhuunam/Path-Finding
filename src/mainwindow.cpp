#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "findingalgorithm.h"

#include <QQuickItem>
#include <QtWidgets>
#include <QIcon>
#include <QFile>

#include <vector>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;

const QString styles = ":/styles/styles.qss";
const QString searchIcon = ":/images/search.png";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), findingAlgorithm(new FindingAlgorithm())
{
    ui->setupUi(this);
    setupLayout();
    setupStyles();
    setupMap();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete findingAlgorithm;
}

void MainWindow::setupLayout()
{
    auto inputHorizontalLayout = new QHBoxLayout();
    inputHorizontalLayout->addWidget(ui->comboBox);
    inputHorizontalLayout->addWidget(ui->fromLineEdit);
    inputHorizontalLayout->addWidget(ui->toLineEdit);
    inputHorizontalLayout->addWidget(ui->searchButton);

    auto mainLayout = new QVBoxLayout(ui->centralwidget);
    mainLayout->addLayout(inputHorizontalLayout);
    mainLayout->addWidget(ui->distanceLabel);
    mainLayout->addWidget(ui->mapQuickWidget);

    ui->centralwidget->setLayout(mainLayout);

    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
}

void MainWindow::setupStyles()
{
    QFile styleFile(styles);
    if (styleFile.open(QFile::ReadOnly))
    {
        setStyleSheet(QString::fromUtf8(styleFile.readAll()));
    }

    ui->comboBox->addItem(QString::fromUtf8("Algorithms"));
    ui->comboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    ui->fromLineEdit->setPlaceholderText("From");
    ui->fromLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    ui->toLineEdit->setPlaceholderText("To");
    ui->toLineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    ui->distanceLabel->setVisible(false);
    ui->distanceLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->distanceLabel->setAlignment(Qt::AlignCenter);

    QSize iconSize(28, 28);
    ui->searchButton->setIcon(QIcon(searchIcon));
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
    connect(obj, SIGNAL(mapClicked(qreal, qreal)), this, SLOT(handleMapClick(qreal, qreal)));

    emit setCenter(21.028511, 105.804817);
}

void MainWindow::handleMapClick(qreal lat, qreal lng)
{
    if (isSelectingStart)
    {
        startLat = lat;
        startLng = lng;
        ui->fromLineEdit->setText(QString::number(lat) + ", " + QString::number(lng));
        emit addMarker(lat, lng);
    }
    else
    {
        endLat = lat;
        endLng = lng;
        ui->toLineEdit->setText(QString::number(lat) + ", " + QString::number(lng));
        emit addMarker(lat, lng);
    }
    isSelectingStart = !isSelectingStart;
}

void MainWindow::onSearchClicked()
{
    int startNode = findNearestNode(startLat, startLng);
    int endNode = findNearestNode(endLat, endLng);

    std::vector<Node> path = findingAlgorithm->findShortestPath(startNode, endNode);

    QVariantList coordinates;
    for (const Node &node : path)
    {
        coordinates.append(node.lat);
        coordinates.append(node.lng);
    }

    emit drawRoute(coordinates);
}

int MainWindow::findNearestNode(double lat, double lng)
{
    int nearestNode = 0;
    double minDistance = std::numeric_limits<double>::max();

    for (int i = 0; i < findingAlgorithm->nodes.size(); i++)
    {
        double dist = FindingAlgorithm::calculateDistance(
            lat, lng,
            findingAlgorithm->nodes[i].lat,
            findingAlgorithm->nodes[i].lng);
        if (dist < minDistance)
        {
            minDistance = dist;
            nearestNode = i;
        }
    }

    return nearestNode;
}
