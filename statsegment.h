#ifndef STATSEGMENT_H
#define STATSEGMENT_H

#include <QObject>
#include <QTimer>

#include "globalheader.h"
#include "qdot.h"

class statSegment : QObject
{
    Q_OBJECT
private:
    QTimer runTimer;
    QTimer cyclicTimer;
    QDot* red_dot;

    int targetNum;

    segParams* params;

    QPair<double, double> generateNewCords();
public:
    statSegment(int dotSize);
    ~statSegment();

    void runStaticSegment();
    void setDotSize(int size);
};

#endif // STATSEGMENT_H
