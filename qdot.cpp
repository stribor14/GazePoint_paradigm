#include "qdot.h"

QDot::QDot(int x, int y, int size, QBrush color) : QGraphicsEllipseItem(){
    dotSize = size;
    this->setRect(x - dotSize/2, y - dotSize/2, dotSize, dotSize);
    this->setBrush(color);
}

bool QDot::isTarget(){
    return target;
}

void QDot::setTarget(const bool &state){
    target = state
            ;}

void QDot::setSize(const int &size)
{
    dotSize = size;
}

void QDot::setCord(const int &x, const int &y){
    this->setRect(x - dotSize/2, y - dotSize/2, dotSize, dotSize);
}

void QDot::setAngle(const double &angle){
    dotAngle = angle;
}

void QDot::setSpeed(const double &speed)
{
    dotSpeed = speed;
}

double QDot::getAngle(){
    return dotAngle;
}

double QDot::getDist(){
    return oldDist;
}

double QDot::getSpeed()
{
    return dotSpeed;
}

int QDot::getResult()
{
    return this->brush() == QBrush(Qt::green) ? 1 : this->brush() == QBrush(Qt::yellow) ? -1 : 0;
}

void QDot::moveDot(const double &angle, const double &dist){
    dotAngle += angle;
    QRectF tempCurrent = this->rect();
    oldDist = dist;
    this->setRect(tempCurrent.x() + cos(dotAngle)*dist*dotSpeed,
                  tempCurrent.y() + sin(dotAngle)*dist*dotSpeed,
                  dotSize, dotSize);
}

void QDot::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event)
    if(!acceptMouse) return;
    if(this->isUnderMouse()) this->setBrush(QBrush(target ? Qt::green : Qt::yellow));
}
