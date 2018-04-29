#ifndef STATSEGMENT_H
#define STATSEGMENT_H

#include <QObject>
#include <QTimer>
#include <QEventLoop>

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

    const segParams &params;

    QPair<double, double> generateNewCords();
public:
    statSegment(const segParams &parameters);
    ~statSegment();

    void runStaticSegment();
    void setDotSize();
};

#endif // STATSEGMENT_H
