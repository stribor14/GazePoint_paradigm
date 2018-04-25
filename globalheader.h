#ifndef RANDOMGENERATORS_H
#define RANDOMGENERATORS_H

#include <random>
#include <functional>

#include <QPoint>
#include <QWidget>
#include <QGraphicsScene>

#include "gazecomunicator.h"

const double pi = std::acos(-1);

std::mt19937 engine;
std::uniform_real_distribution<double> unif_dist(0.05,0.95);
std::normal_distribution<double> normal_dist(0.0, pi/3);
auto generator_normal = std::bind(std::ref(normal_dist), std::ref(engine));
auto generator_uniform = std::bind(std::ref(unif_dist), std::ref(engine));
void resetRandom(){
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
    QPoint dispCenter;
    int dispWidth;
    int dispHeight;
    int dispPadding;
    int timeCenter;
    int timeTarget;
    int timeMovement;
    int timeAnswer;
    int timeSegment;
    int timeDot;

    // pointers
    QWidget* display;
    QGraphicsScene* dispScene;
    GazeComunicator* GazePt;

};

#endif // RANDOMGENERATORS_H
