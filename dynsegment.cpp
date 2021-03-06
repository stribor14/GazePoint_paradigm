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
    for(auto &&dot : dynDot) delete dot;
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
            double speed = generator_uniform()/2 + 0.5;
            double angle = generator_normal()/2  + k*pi*2/numDot;
            dynDot[k]->setVelocity(speed*cos(angle), speed*sin(angle));
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
    bool collisionExists = false;
    for(auto &&dot : dynDot){
        collisionExists |= edgeXCollision(dot);
        collisionExists |= edgeYCollision(dot);
    }
    for (int k = 0; k < dynDot.size() ; k++)
        for (int i = k + 1; i < dynDot.size() ; i++)
            collisionExists |= dotCollision(dynDot[k], dynDot[i]);

    if(collisionExists) collisionCheck();
}

bool dynSegment::edgeXCollision(QDot* dot){
    bool collisionExists = false;
    if (dot->rect().center().x() <= params.dispPadding){
        double dT = (params.dispPadding - dot->rect().center().x()) / -dot->getVelocity().first;
        dot->moveDot(-dT);
        dot->setVelocity(-dot->getVelocity().first, dot->getVelocity().second);
        dot->moveDot(dT);
        collisionExists = true;
    }
    if (dot->rect().center().x() >= params.dispWidth - params.dispPadding){
        double dT = (dot->rect().center().x() - (params.dispWidth - params.dispPadding)) / dot->getVelocity().first;
        dot->moveDot(-dT);
        dot->setVelocity(-dot->getVelocity().first, dot->getVelocity().second);
        dot->moveDot(dT);
        collisionExists = true;
    }
    return collisionExists;
}

bool dynSegment::edgeYCollision(QDot* dot){
    bool collisionExists = false;
    if (dot->rect().center().y() <= params.dispPadding){
        double dT = (params.dispPadding - dot->rect().center().y()) / -dot->getVelocity().second;
        dot->moveDot(-dT);
        dot->setVelocity(dot->getVelocity().first, -dot->getVelocity().second);
        dot->moveDot(dT);
        collisionExists = true;
    }
    if (dot->rect().center().y() >= params.dispHeight - params.dispPadding){
        double dT = (dot->rect().center().y() - (params.dispHeight - params.dispPadding)) / dot->getVelocity().second;
        dot->moveDot(-dT);
        dot->setVelocity(dot->getVelocity().first, -dot->getVelocity().second);
        dot->moveDot(dT);
        collisionExists = true;
    }
    return collisionExists;
}

bool dynSegment::dotCollision(QDot* dot1, QDot* dot2){
    if(params.dotSize >= m_dist(dot1->rect().center(), dot2->rect().center())){
        QPair<double, double> n((dot2->rect().center().x() - dot1->rect().center().x())/m_dist(dot1->rect().center(), dot2->rect().center()),
                                (dot2->rect().center().y() - dot1->rect().center().y())/m_dist(dot1->rect().center(), dot2->rect().center()));

        QPair<double, double> v1 = dot1->getVelocity();
        QPair<double, double> v2 = dot2->getVelocity();

        double v1_n = v1.first*n.first + v1.second*n.second;
        double v1_t = v1.first*n.second - v1.second*n.first;
        double v2_n = v2.first*n.first + v2.second*n.second;
        double v2_t = v2.first*n.second - v2.second*n.first;

        double dT = (params.dotSize - m_dist(dot1->rect().center(), dot2->rect().center()))/ (v1_n-v2_n);

        dot1->moveDot(-dT);
        dot2->moveDot(-dT);

        dot1->setVelocity(v2_n*n.first + v1_t*n.second, v2_n*n.second - v1_t*n.first);
        dot2->setVelocity(v1_n*n.first + v2_t*n.second, v1_n*n.second - v2_t*n.first);

        dot1->moveDot(dT);
        dot2->moveDot(dT);

        return true;
    }
    else return false;
}
