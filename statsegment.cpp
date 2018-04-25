#include "statsegment.h"

QPair<double, double> statSegment::generateNewCords()
{
    runTimer.setTimerType(Qt::PreciseTimer);
    cyclicTimer.setTimerType(Qt::PreciseTimer);

    double newX = generator_uniform() * (params->dispWidth - 2*params->dispPadding) + params->dispPadding;
    double newY = generator_uniform() * (params->dispHeight - 2*params->dispPadding) + params->dispPadding;
    return QPair<double, double>(newX, newY);
}

statSegment::statSegment(int dotSize)
{
    red_dot = new QDot(0, 0, dotSize, QBrush(Qt::red));
}

statSegment::~statSegment()
{
    delete red_dot;
}

void statSegment::runStaticSegment()
{
    targetNum = 1;

    QEventLoop tempLoop;

    runTimer.singleShot(1000 * params->timeSegment, Qt::PreciseTimer,[&](){
        if(params->useGaze){
            params->GazePt->disconnect();
        }
        else {
            cyclicTimer.disconnect();
            cyclicTimer.stop();
        };
        params->dispScene->removeItem(red_dot);
        tempLoop.quit();
    });

    if (params->useGaze){
        // NEW DOT AFTER GAZE FIXATION
        connect(params->GazePt, &GazeComunicator::targetReached, this, [&](){
            QPair<double, double> newCords = generateNewCords();
            red_dot->setCord(newCords.first, newCords.second);
            params->GazePt->setTarget(targetNum++, newCords.first/params->dispWidth, newCords.second/params->dispHeight);
        }, Qt::QueuedConnection);
        red_dot->setCord(params->dispWidth/2, params->dispHeight/2);
        params->GazePt->setTarget(targetNum++, 0.5, 0.5);
        params->dispScene->addItem(red_dot);
    }
    else{
        // NEW DOT AFTER FIXED TIME
        connect(&cyclicTimer, &QTimer::timeout, this, [&](){
            QPair<double, double> newCords = generateNewCords();
            red_dot->setCord(newCords.first, newCords.second);
            params->GazePt->setTarget(targetNum++, newCords.first/params->dispWidth, newCords.second/params->dispHeight);
        });
        red_dot->setCord(params->dispWidth/2, params->dispHeight/2);
        params->GazePt->setTarget(targetNum++, 0.5, 0.5);
        params->dispScene->addItem(red_dot);

        cyclicTimer.start(params->timeDot);
        }

    tempLoop.exec();
}

void statSegment::setDotSize(int size)
{
    red_dot->setSize(size);
}
