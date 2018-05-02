#ifndef RANDOMGENERATORS_H
#define RANDOMGENERATORS_H

#pragma once

#include <random>

#include "gazecomunicator.h"

#include <QPoint>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPalette>
#include <QVBoxLayout>
#include <QOpenGLWidget>

static const double pi = std::acos(-1);

static std::mt19937 engine;
static std::uniform_real_distribution<double> unif_dist(0.05,0.95);
static std::normal_distribution<double> normal_dist(0.0, pi/3);
static auto generator_normal = std::bind(std::ref(normal_dist), std::ref(engine));
static auto generator_uniform = std::bind(std::ref(unif_dist), std::ref(engine));
static void resetRandom(){
    engine.seed(14);
    unif_dist.reset();
    normal_dist.reset();
}

class segParams
{
public:
    // params
    bool useGaze;
    int dotSize;
    int dotOffset;
    double dotPerimeter = 100;
    QPoint dispCenter;
    int dispWidth;
    int dispHeight;
    int dispPadding = 150;
    int timeCenter;
    int timeTarget;
    int timeMovement;
    int timeAnswer;
    int timeSegment;
    int timeDot;

    // pointers
    QWidget* display;
    QGraphicsView* dispView;
    QGraphicsScene* dispScene;
    GazeComunicator* GazePt;

    segParams(){
        display = new QWidget();
        dispView = new QGraphicsView(display);
        dispScene = new QGraphicsScene(dispView);
        GazePt = new GazeComunicator();

        QPalette pal = display->palette();
        pal.setColor(QPalette::Background, Qt::black);
        display->setLayout(new QVBoxLayout);
        display->layout()->addWidget(dispView);
        display->layout()->setMargin(0);
        display->setPalette(pal);
        dispView->setViewport(new QOpenGLWidget());
        dispView->setStyleSheet("border: 0px solid black");
        dispView->setScene(dispScene);
        dispView->setPalette(pal);
        dispScene->setBackgroundBrush(QBrush(Qt::black));
    }
    ~segParams(){
        delete GazePt;
        delete dispScene;
        delete dispView;
        delete display;
    }

};

#endif // RANDOMGENERATORS_H
