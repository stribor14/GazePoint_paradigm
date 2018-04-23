#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include <cmath>
#include <random>
#include <functional>

#include "gazecomunicator.h"
#include "qdot.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGuiApplication>
#include <QScreen>
#include <QPalette>
#include <QTimer>
#include <QPair>
#include <QEventLoop>
#include <QOpenGLWidget>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QWidget *secondDisplay;
    QGraphicsView *dispView;
    QGraphicsScene *dispScene;

    GazeComunicator* GazePt;

    QTimer runTimer;
    QTimer cyclicTimer;

    int maxDist = 25;
    int targetNum;

    int dispWidth;
    int dispHeight;
    QPoint dispCenter;
    int dispPadding = 150;


    QDot* red_dot;
    QDot* center_dot;
    QList<QDot*> dynDot;

    void runStaticSegment(bool useGaze=false);
    void runDynamicSegment(int lvl, int taskNum, int numDot);

    QPair<double, double> generateNewCords();

    void dynColor(int lvl, int numDot = 0);
    void dynCollisionCheck(int numDot);
};


#endif // MAINWINDOW_H
