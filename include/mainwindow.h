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

private:
    Ui::MainWindow *ui;
    RouteGraph *routeGraph;

    void setupStyles();
    void setupMap();
};
#endif // MAINWINDOW_H