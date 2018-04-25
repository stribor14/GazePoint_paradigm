#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "gazecomunicator.h"
#include "dynsegment.h"
#include "statsegment.h"

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



    int maxDist = 25;
    int targetNum;

    int dispWidth;
    int dispHeight;
    QPoint dispCenter;
    double dispPadding = 150;

    dynSegment* d_seg;
    statSegment* s_seg;

};


#endif // MAINWINDOW_H
