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

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGuiApplication>
#include <QScreen>
#include <QPalette>
#include <QTimer>
#include <QPair>
#include <QEventLoop>

#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>

#include "gazecomunicator.h"
#include "qdot.h"

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
    int dispPadding = 150;

    QDot* red_dot;
    QDot* center_dot;
    QList<QDot*> octaDot;

    void runStaticSegment(bool useGaze=false);
    void runDynamicSegment(int lvl, int taskNum);

    QPair<double, double> generateNewCords();

    void octaColor(int lvl);
    void octaReset();
    void octaCollisionCheck();
};


#endif // MAINWINDOW_H
