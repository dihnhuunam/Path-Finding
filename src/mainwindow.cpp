#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "routegraph.h"

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
    : QMainWindow(parent), ui(new Ui::MainWindow), routeGraph(new RouteGraph())
{
    ui->setupUi(this);
    setupLayout();
    setupStyles();
    setupMap();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete routeGraph;
}

void MainWindow::setupLayout()
{
    // Row 1 (input): comboBox, fromLineEdit, toLineEdit, searchButton
    auto inputHorizontalLayout = new QHBoxLayout();
    inputHorizontalLayout->addWidget(ui->comboBox);
    inputHorizontalLayout->addWidget(ui->fromLineEdit);
    inputHorizontalLayout->addWidget(ui->toLineEdit);
    inputHorizontalLayout->addWidget(ui->searchButton);

    // Row 2 (output): distanceLabel, timeLabel
    auto outputHorizontalLayout = new QHBoxLayout();
    outputHorizontalLayout->addWidget(ui->distanceLabel);
    outputHorizontalLayout->addWidget(ui->timeLabel);

    // mainLayout contains Row 1, Row 2, and mapQuickWidget
    auto mainLayout = new QVBoxLayout(ui->centralwidget);
    mainLayout->addLayout(inputHorizontalLayout);
    mainLayout->addLayout(outputHorizontalLayout);
    mainLayout->addWidget(ui->mapQuickWidget);

    ui->centralwidget->setLayout(mainLayout);
}

void MainWindow::setupStyles()
{
    // Load styles.qss
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
    ui->timeLabel->setVisible(false);

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
    emit setCenter(21.028511, 105.804817);
    emit addMarker(21.028511, 105.804817);
}
