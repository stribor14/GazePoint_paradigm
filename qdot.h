#ifndef QDOT_H
#define QDOT_H

#include <QObject>
#include <QBrush>
#include <QGraphicsEllipseItem>

class QDot : public QGraphicsEllipseItem
{
private:
    int dotSize;
    bool target = false;
    double dotAngle = 0;
    double oldDist = 0;
public:
    bool acceptMouse = false;
    QDot(int x, int y, int size, QBrush color, double angle);
    ~QDot(){}

    bool isTarget();
    void setTarget(bool state);

    void setCord(int x, int y);
    void setAngle(double angle);
    double getAngle();
    double getDist();
    void moveDot(double angle, double dist);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
};

#endif // QDOT_H
