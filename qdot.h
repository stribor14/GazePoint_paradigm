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
    double dotSpeed = 0;
    double oldDist = 0;
public:
    bool acceptMouse = false;
    QDot(int x, int y, int size, QBrush color);
    ~QDot(){}

    bool isTarget();
    void setTarget(bool state);

    void setSize(int size);
    void setCord(double x, const double y);
    void setAngle(double angle);
    void setSpeed(double speed);
    double getAngle();
    double getDist();
    double getSpeed();
    int getResult();
    void moveDot(double angle, double dist);
    void moveDot(double dist);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
};

#endif // QDOT_H
