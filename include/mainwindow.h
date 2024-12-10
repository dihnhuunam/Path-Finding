#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include "findingalgorithm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleMapClick(qreal lat, qreal lng);
    void onSearchClicked();
    void calculateDistanceFromCoordinates(QVariantList coordinates);

signals:
    void setCenter(QVariant lat, QVariant lng);
    void addMarker(QVariant lat, QVariant lng);
    void drawRoute(QVariant coordinates);

private:
    Ui::MainWindow *ui;
    FindingAlgorithm *findingAlgorithm;
    
    bool isSelectingStart = true;
    double startLat = 0, startLng = 0;
    double endLat = 0, endLng = 0;

    void setupLayout();
    void setupStyles();
    void setupMap();
    int findNearestNode(double lat, double lng);
};

#endif // MAINWINDOW_H