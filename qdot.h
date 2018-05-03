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
    QPair<double, double> dotVelocity;
public:
    bool acceptMouse = false;
    QDot(int x, int y, int size, QBrush color);
    ~QDot(){}

    bool isTarget();
    void setTarget(bool state);

    void setSize(int size);
    void setCord(double x, const double y);
    void setVelocity(double vx, double vy);
    double getAngle();
    double getSpeed();
    QPair<double, double> getVelocity();
    int getResult();
    void moveDot(double dist);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
};

#endif // QDOT_H
