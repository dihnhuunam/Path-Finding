#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include "routegraph.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void setCenter(QVariant lat, QVariant lng);
    void addMarker(QVariant lat, QVariant lng);
    void drawRoute(QVariant coordinates);

private slots:
    void handleMapClick(qreal lat, qreal lng); 
    void onSearchClicked();

private:
    Ui::MainWindow *ui;
    RouteGraph *routeGraph;

    qreal startLat, startLng; 
    qreal endLat, endLng;     
    bool isSelectingStart = true;

    void setupLayout();
    void setupStyles();
    void setupMap();
    int findNearestNode(double lat, double lng);
};
#endif // MAINWINDOW_H