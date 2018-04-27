#include "dynsegment.h"

#define m_dist(a,b) (sqrt((a.x() - b.x())*(a.x() - b.x()) + (a.y() - b.y())*(a.y() - b.y())))
#define m_angle(a,b) (atan2(a.y() - b.y(), a.x() - b.x()))

dynSegment::dynSegment(const segParams &parameters) :
    params(parameters)
{
    runTimer.setTimerType(Qt::PreciseTimer);
    cyclicTimer.setTimerType(Qt::PreciseTimer);

    center_dot = new QDot(0, 0, params.dotSize, QBrush(Qt::white));
}

dynSegment::~dynSegment()
{
    delete center_dot;
}

void dynSegment::runDynamicSegment(int lvl, int taskNum, int numDot)
{
    QEventLoop tempLoop;

    for (int k = 0; k < numDot; k++){
        dynDot.append(new QDot(0, 0, params.dotSize, QBrush(Qt::white)));
    }

    for(int k = 1; k <= taskNum; k++){

        params.GazePt->logCustomEvent("DYN_START", lvl + k/10.0, 0, 0);
        params.GazePt->setTarget(0, 0.5, 0.5);

        // part 1
        center_dot->setCord(params.dispWidth/2, params.dispHeight/2);
        params.dispScene->addItem(center_dot);
        runTimer.singleShot(params.timeCenter * 1000, Qt::PreciseTimer, this, [&](){
            params.dispScene->removeItem(center_dot);
            tempLoop.quit();
        });
        tempLoop.exec();

        // part 2
        setColor(lvl);
        for(int k = 0; k < numDot; k++){
            dynDot[k]->setCord(params.dispWidth/2 + cos(k*pi*2/numDot) * params.dotOffset,
                                params.dispHeight/2 + sin(k*pi*2/numDot) * params.dotOffset);
            dynDot[k]->setSpeed(generator_uniform()/2 + 0.5);
            dynDot[k]->setAngle(generator_normal()/2  + k*pi*2/numDot);
            params.dispScene->addItem(dynDot[k]);
        }
        runTimer.singleShot(params.timeTarget * 1000, Qt::PreciseTimer, this, [&](){
            setColor(0);
            tempLoop.quit();
        });
        tempLoop.exec();

        // part 3
        connect(&cyclicTimer, &QTimer::timeout, this, [&](){
            for(auto &&dot: dynDot)
                //dot->moveDot(generator_uniform()*2*pi,generator_uniform() * maxDist);
                dot->moveDot(20);
            collisionCheck();
        });

        cyclicTimer.start(1000/60);
        runTimer.singleShot(params.timeMovement * 1000, Qt::PreciseTimer, this, [&](){
            cyclicTimer.disconnect();
            cyclicTimer.stop();
            tempLoop.quit();
        });
        tempLoop.exec();

        // part 4
        int resF = 0;
        int resT = 0;
        runTimer.singleShot(params.timeAnswer * 1000, Qt::PreciseTimer, this, [&](){
            for(auto &&dot: dynDot){
                int temp = dot->getResult();
                (temp == 1 ? resT : resF) += temp;
                dot->acceptMouse = false;
                params.dispScene->removeItem(dot);
            }
            params.display->setCursor(Qt::BlankCursor);
            tempLoop.quit();
        });

        for(auto &&dot: dynDot) dot->acceptMouse = true;
        QCursor tempCursor = QCursor(Qt::ArrowCursor);
        tempCursor.setPos(params.dispCenter);
        params.display->setCursor(tempCursor);

        tempLoop.exec();
        params.GazePt->logCustomEvent("DYN_END", lvl + k/10.0, resT, -resF);
    }

    dynDot.clear();
}

void dynSegment::updateDotSize()
{
    center_dot->setSize(params.dotSize);
    for(auto &&dot: dynDot) dot->setSize(params.dotSize);
}

void dynSegment::setColor(int lvl)
{
    int numDot = dynDot.size();
    QList<int> greenIndex;
    for(auto &&dot: dynDot) dot->setBrush(QBrush(Qt::white));
    if(lvl){
        int tempIndex = generator_uniform() * numDot;
        for(auto &&dot: dynDot) dot->setTarget(false);
        switch(lvl){
            case 1: greenIndex.append(tempIndex);
                    break;
            case 2: greenIndex.append(tempIndex);
                    greenIndex.append(tempIndex == 0 ? numDot - 1 : tempIndex - 1);
                    greenIndex.append(tempIndex == numDot - 1 ? 0 : tempIndex + 1);
                    break;
            case 3: while(greenIndex.size()<3){
                        bool tempOK = true;
                        for (auto &&index : greenIndex){
                            if(index - 1 <= tempIndex && tempIndex <= index + 1) tempOK = false;
                            if(index == 0 && tempIndex == numDot - 1) tempOK = false;
                            if(index == numDot - 1 && tempIndex == 0) tempOK = false;
                        }
                        if(tempOK) greenIndex.append(tempIndex);
                        tempIndex = generator_uniform() * numDot;
                    }
        }
        for(auto &&index: greenIndex){
            dynDot[index]->setTarget(true);
            dynDot[index]->setBrush(QBrush(Qt::green));
        }
    }
}

void dynSegment::collisionCheck()
{
    {
        for(auto &&dot : dynDot){
            QPointF center = dot->rect().center();
            double angle =  dot->getAngle();

            if (center.x() <= params.dispPadding)
                dot->moveDot(pi-2*angle, (params.dispPadding - center.x()) / cos(pi - angle));
            if (center.x() >= params.dispWidth - params.dispPadding)
                dot->moveDot(pi-2*angle, (center.x() - (params.dispWidth - params.dispPadding)) / cos(angle));
            if (center.y() <= params.dispPadding)
                dot->moveDot(-2*angle, (params.dispPadding - center.y()) / sin(-angle));
            if (center.y() >= params.dispHeight - params.dispPadding)
                dot->moveDot(-2*angle, (center.y() - (params.dispHeight - params.dispPadding)) / sin(angle));
        }
        for (int k = 0; k < dynDot.size() ; k++){
            for (int i = k + 1; i < dynDot.size() ; i++){
                if(dynDot[k]->collidesWithItem(dynDot[i])){
                    QPointF center1 = dynDot[k]->rect().center();
                    QPointF center2 = dynDot[i]->rect().center();
                    double dist = (params.dotSize - m_dist(center1, center2))/2;
                    double angle = m_angle(center1, center2);
                    double tempAngle = dynDot[k]->getAngle();
                    double tempSpeed = dynDot[k]->getSpeed();
                    dynDot[k]->setAngle(dynDot[i]->getAngle());
                    dynDot[i]->setAngle(tempAngle);
                    dynDot[k]->setSpeed(dynDot[i]->getSpeed());
                    dynDot[i]->setSpeed(tempSpeed);
                    dynDot[k]->setCord(center1.x() + cos(angle)*dist,
                                       center1.y() + sin(angle)*dist);
                    dynDot[i]->setCord(center2.x() - cos(angle)*dist,
                                       center2.y() - sin(angle)*dist);
                }
            }
        }
    }
}
