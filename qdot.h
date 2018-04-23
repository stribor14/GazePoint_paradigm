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
    void setTarget(const bool &state);

    void setSize(const int &size);
    void setCord(const int &x, const int &y);
    void setAngle(const double &angle);
    void setSpeed(const double &speed);
    double getAngle();
    double getDist();
    double getSpeed();
    int getResult();
    void moveDot(const double &angle, const double &dist);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
};

#endif // QDOT_H
