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
#include <QEventLoop>

#include "gazecomunicator.h"
#include "qdot.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *secondDisplay;
    QGraphicsView *dispView;
    QGraphicsScene *dispScene;

    GazeComunicator* GazePt;

    QTimer runTimer;
    QTimer cyclicTimer;

    int maxDist = 15;

    int dispWidth;
    int dispHeight;

    QDot* red_dot;
    QDot* center_dot;
    QList<QDot*> octaDot;

    void runStaticSegment();
    void runDynamicSegment(int lvl);

    QList<int> octaColor(int lvl);
    void octaReset();
    void octaCollisionCheck();
};


#endif // MAINWINDOW_H
