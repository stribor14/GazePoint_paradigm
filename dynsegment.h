#ifndef DYNSEGMENT_H
#define DYNSEGMENT_H

#include <QObject>
#include <QTimer>
#include <QCursor>

#include "globalheader.h"
#include "qdot.h"

class dynSegment : QObject
{
    Q_OBJECT
private:
    QTimer runTimer;
    QTimer cyclicTimer;
    QDot* center_dot;
    QList<QDot*> dynDot;

    segParams* params;

    void setColor(int lvl, int numDot = 0);
    void collisionCheck(int numDot);

public:
    dynSegment(int dotSize);
    ~dynSegment();

    void setParams();

    void runDynamicSegment(int lvl, int taskNum, int numDot);
    void setDotSize(int size);
};

#endif // DYNSEGMENT_H
