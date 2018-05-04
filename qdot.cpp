#include "qdot.h"

QDot::QDot(int x, int y, int size, QBrush color) : QGraphicsEllipseItem(){
    dotSize = size;
    this->setRect(x - dotSize/2, y - dotSize/2, dotSize, dotSize);
    this->setBrush(color);
}

bool QDot::isTarget(){
    return target;
}

void QDot::setTarget(bool state){
    target = state;
}

void QDot::setSize(int size)
{
    dotSize = size;
}

void QDot::setCord(double x, double y){
    this->setRect(x - dotSize/2, y - dotSize/2, dotSize, dotSize);
}

void QDot::setVelocity(double vx, double vy)
{
    dotVelocity.first = vx;
    dotVelocity.second = vy;
}

double QDot::getAngle()
{
    return atan2(dotVelocity.second, dotVelocity.first);
}

QPair<double, double> QDot::getVelocity()
{
    return dotVelocity;
}

int QDot::getResult()
{
    return this->brush() == QBrush(Qt::green) ? 1 : this->brush() == QBrush(Qt::yellow) ? -1 : 0;
}

void QDot::moveDot(double dT)
{
    QRectF tempCurrent = this->rect();
    this->setRect(tempCurrent.x() + dT*dotVelocity.first,
                  tempCurrent.y() + dT*dotVelocity.second,
                  dotSize, dotSize);
}

void QDot::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event)
    if(!acceptMouse) return;
    if(this->isUnderMouse()) this->setBrush(QBrush(target ? Qt::green : Qt::yellow));
}
