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

    const segParams &params;

    void setColor(int lvl, int numDot = 0);
    void collisionCheck(int numDot);

public:
    dynSegment(const segParams &parameters);
    ~dynSegment();

    void runDynamicSegment(int lvl, int taskNum, int numDot);
    void setDotSize();
};

#endif // DYNSEGMENT_H
