#include "qdot.h"

QDot::QDot(int x, int y, int size, QBrush color, double angle) : QGraphicsEllipseItem(){
    dotSize = size;
    dotAngle = angle;
    this->setRect(x,y,dotSize,dotSize);
    this->setBrush(color);
}

bool QDot::isTarget(){return target;}

void QDot::setTarget(bool state){target = state;}

void QDot::setCord(int x, int y){
    this->setRect(x,y,dotSize,dotSize);
}

void QDot::setAngle(double angle){dotAngle = angle;}

double QDot::getAngle(){return dotAngle;}

double QDot::getDist(){return oldDist;}

void QDot::moveDot(double angle, double dist){
    dotAngle += angle;
    QRectF tempCurrent = this->rect();
    oldDist = dist;
    this->setRect(tempCurrent.x() + cos(dotAngle)*dist,
                  tempCurrent.y() + sin(dotAngle)*dist,
                  dotSize, dotSize);
}

void QDot::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event)
    if(!acceptMouse) return;
    if(this->isUnderMouse()) this->setBrush(QBrush(target ? Qt::green : Qt::yellow));
}
