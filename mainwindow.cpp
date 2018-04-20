#include "mainwindow.h"
#include "ui_mainwindow.h"

#define m_dist(a,b) (sqrt((a.x() - b.x())*(a.x() - b.x()) + (a.y() - b.y())*(a.y() - b.y())))
#define m_angle(a,b) (atan2(a.y() - b.y(), a.x() - b.x()))
#define readLine(x) (ui->x->text().toInt())

const double pi = std::acos(-1);
std::mt19937 engine;
std::uniform_real_distribution<double> unif_dist(0.05,0.95);
std::normal_distribution<double> normal_dist(0.0, pi/3);
auto generator_normal = std::bind(std::ref(normal_dist), std::ref(engine));
auto generator_uniform = std::bind(std::ref(unif_dist), std::ref(engine));
void resetRandom(){
    engine.seed(14);
    unif_dist.reset();
    normal_dist.reset();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    secondDisplay(new QWidget),
    GazePt(new GazeComunicator)
{
    ui->setupUi(this);

    ui->b_log_folder->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
    ui->w_warning->setPixmap(QApplication::style()->standardPixmap(QStyle::SP_MessageBoxWarning));
    ui->i_log_folder->setText(QDir::currentPath());

    dispView = new QGraphicsView(secondDisplay);
    dispScene = new QGraphicsScene(dispView);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    secondDisplay->setLayout(new QVBoxLayout);
    secondDisplay->layout()->addWidget(dispView);
    secondDisplay->layout()->setMargin(0);
    secondDisplay->setPalette(pal);
    dispView->setStyleSheet(tr("border: 0px solid black"));
    dispView->setScene(dispScene);
    dispView->setPalette(pal);
    dispView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    dispScene->setBackgroundBrush(QBrush(Qt::black));

    red_dot = new QDot(0, 0, readLine(i_dotSize), QBrush(Qt::red), 0);
    center_dot = new QDot(0, 0, readLine(i_dotSize), QBrush(Qt::white), 0);

    connect(ui->i_dotSize, &QLineEdit::editingFinished, this, [&](){
       red_dot->setSize(readLine(i_dotSize));
       center_dot->setSize(readLine(i_dotSize));
       for(auto &&dot: octaDot) dot->setSize(readLine(i_dotSize));
    });

    for (int k = 0; k < 8; k++){
        octaDot.append(new QDot(0, 0, readLine(i_dotSize), QBrush(Qt::white), generator_normal() + k * pi/2));
    }

    runTimer.setTimerType(Qt::PreciseTimer);
    cyclicTimer.setTimerType(Qt::PreciseTimer);

    for (auto &&screen: QGuiApplication::screens()){
        ui->i_screenBox->insertItem(0,screen->name().remove(0,4),screen->geometry());
    }

    connect(ui->b_test, &QPushButton::clicked, this, [&](){
        QRect &&screenres = ui->i_screenBox->currentData().toRect();
        secondDisplay->move(QPoint(screenres.x(), screenres.y()));
        secondDisplay->resize(screenres.width(), screenres.height());

        dispWidth = screenres.width();
        dispHeight = screenres.height();
        red_dot->setCord(dispWidth/2, dispHeight/2);
        dispScene->addItem(red_dot);

        secondDisplay->showFullScreen();

        QTimer::singleShot(3000,[&](){
            secondDisplay->close();
            dispScene->removeItem(red_dot);
        });
    });

    connect(ui->b_pokreni, &QPushButton::clicked, this, [&](){

        if(!ui->i_participant->text().compare("")){
            QMessageBox::warning(this, "Greška",
                                 "Nije unesena šifra korisnika!");
            return;
        }

        bool temp_useGaze = ui->i_gazePoint->isChecked();
        int temp_taskNum = ui->i_numDynamicTask->value();

        QRect &&screenres = ui->i_screenBox->currentData().toRect();

        secondDisplay->move(QPoint(screenres.x(), screenres.y()));
        secondDisplay->resize(screenres.width(), screenres.height());

        dispWidth = screenres.width();
        dispHeight = screenres.height();
        dispScene->setSceneRect(0,0,dispWidth, dispHeight);

        if(ui->i_write_log->isChecked()) GazePt->startLog(ui->i_log_folder->text(), ui->i_participant->text());
        GazePt->setPerimeter((100.0 / dispWidth), (100.0 / dispHeight), temp_useGaze);

        secondDisplay->showFullScreen();
        secondDisplay->setCursor(Qt::BlankCursor);

        resetRandom();
        runStaticSegment(temp_useGaze);
        runDynamicSegment(1, temp_taskNum);
        resetRandom();
        runStaticSegment(temp_useGaze);
        runDynamicSegment(2, temp_taskNum);
        resetRandom();
        runStaticSegment(temp_useGaze);
        runDynamicSegment(3, temp_taskNum);
        resetRandom();
        runStaticSegment(temp_useGaze);

        secondDisplay->close();

        if(ui->i_write_log->isChecked()) GazePt->stopLog();
    });

    connect(ui->b_izlaz, &QPushButton::clicked, this, [&](){
        secondDisplay->close();
        this->close();
    });

    connect(ui->i_gazePoint, &QCheckBox::toggled, this, [&](){
       ui->i_timeStaticDot->setDisabled(ui->i_gazePoint->isChecked());
    });

    connect(ui->b_log_folder, &QPushButton::clicked, this, [&](){
        ui->i_log_folder->setText(
                    QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                        ui->i_log_folder->text(),
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks));
    });

    GazePt->Start();
}

MainWindow::~MainWindow()
{
    GazePt->Stop();

    delete GazePt;
    delete secondDisplay;

    delete ui;
}

void MainWindow::runStaticSegment(bool useGaze)
{
    targetNum = 1;

    QEventLoop tempLoop;

    runTimer.singleShot(1000 * readLine(i_timeStaticSegment), Qt::PreciseTimer,[&](){
        if(useGaze){
            GazePt->disconnect();
        }
        else {
            cyclicTimer.disconnect();
            cyclicTimer.stop();
        };
        dispScene->removeItem(red_dot);
        tempLoop.quit();
    });

    if (useGaze){
        // NEW DOT AFTER GAZE FIXATION
        connect(GazePt, &GazeComunicator::targetReached,this, [&](){
            QPair<double, double> newCords = generateNewCords();
            red_dot->setCord(newCords.first, newCords.second);
            GazePt->setTarget(targetNum++, newCords.first/dispWidth, newCords.second/dispHeight);
        }, Qt::QueuedConnection);
        red_dot->setCord(dispWidth/2, dispHeight/2);
        GazePt->setTarget(targetNum++, 0.5, 0.5);
        dispScene->addItem(red_dot);
    }
    else{
        // NEW DOT AFTER FIXED TIME
        connect(&cyclicTimer, &QTimer::timeout, this, [&](){
            QPair<double, double> newCords = generateNewCords();
            red_dot->setCord(newCords.first, newCords.second);
            GazePt->setTarget(targetNum++, newCords.first/dispWidth, newCords.second/dispHeight);
        });
        red_dot->setCord(dispWidth/2, dispHeight/2);
        GazePt->setTarget(targetNum++, 0.5, 0.5);
        dispScene->addItem(red_dot);

        cyclicTimer.start(readLine(i_timeStaticDot));
        }

    tempLoop.exec();
}

void MainWindow::runDynamicSegment(int lvl, int taskNum)
{
    QEventLoop tempLoop;

    for(int k = 1; k <= taskNum; k++){

        GazePt->logCustomEvent("DYN_START", lvl + k/10.0, 0, 0);
        GazePt->setTarget(0, 0.5, 0.5);

        // part 1
        center_dot->setCord(dispWidth/2, dispHeight/2);
        dispScene->addItem(center_dot);
        runTimer.singleShot(readLine(i_timeDynamicCenter),Qt::PreciseTimer,[&](){
            dispScene->removeItem(center_dot);
            tempLoop.quit();
        });
        tempLoop.exec();

        // part 2
        octaColor(lvl);
        for(int k=0; k<8; k++){
            octaDot[k]->setCord(dispWidth/2 + cos(k*pi/4) * readLine(i_dotOctaOffset),
                                dispHeight/2 + sin(k*pi/4) * readLine(i_dotOctaOffset));
            dispScene->addItem(octaDot[k]);
        }
        runTimer.singleShot(readLine(i_timeDynamicOctagon),Qt::PreciseTimer,[&](){
            octaColor(0);
            tempLoop.quit();
        });
        tempLoop.exec();

        // part 3
        octaReset();
        connect(&cyclicTimer, &QTimer::timeout, this, [&](){
            for(auto &&dot: octaDot)
                //dot->moveDot(generator_uniform()*2*pi,generator_uniform() * maxDist);
                dot->moveDot(0, 15);
            octaCollisionCheck();
        });

        cyclicTimer.start(1000/30);
        runTimer.singleShot(readLine(i_timeDynamicMovement),Qt::PreciseTimer,[&](){
            cyclicTimer.disconnect();
            cyclicTimer.stop();
            tempLoop.quit();
        });
        tempLoop.exec();

        // part 4
        int resF = 0;
        int resT = 0;
        runTimer.singleShot(readLine(i_timeDynamicUserInput),Qt::PreciseTimer,[&](){
            secondDisplay->setCursor(Qt::ArrowCursor);
            for(auto &&dot: octaDot){
                int temp = dot->getResult();
                (temp == 1 ? resT : resF) += temp;
                dot->acceptMouse = false;
                dispScene->removeItem(dot);
            }
            secondDisplay->setCursor(Qt::BlankCursor);
            tempLoop.quit();
        });
        for(auto &&dot: octaDot) dot->acceptMouse = true;
        secondDisplay->setCursor(Qt::ArrowCursor);
        tempLoop.exec();
        GazePt->logCustomEvent("DYN_END", lvl + k/10.0, resT, -resF);
    }
}

QPair<double, double> MainWindow::generateNewCords()
{
    double newX = generator_uniform() * (dispWidth - 2*dispPadding) + dispPadding;
    double newY = generator_uniform() * (dispHeight - 2*dispPadding) + dispPadding;
    return QPair<double, double>(newX, newY);
}

void MainWindow::octaColor(int lvl)
{
    QList<int> greenIndex;
    for(auto &&dot: octaDot) dot->setBrush(QBrush(Qt::white));
    if(lvl){
        int tempIndex = generator_uniform() * 8;
        for(auto &&dot: octaDot) dot->setTarget(false);
        switch(lvl){
            case 1: greenIndex.append(tempIndex);
                    break;
            case 2: greenIndex.append(tempIndex);
                    greenIndex.append(tempIndex == 0 ? 7 : tempIndex - 1);
                    greenIndex.append(tempIndex == 7 ? 0 : tempIndex + 1);
                    break;
            case 3: while(greenIndex.size()<3){
                        bool tempOK = true;
                        for (auto &&index : greenIndex){
                            if(index - 1 <= tempIndex && tempIndex <= index + 1) tempOK = false;
                            if(index == 0 && tempIndex == 7) tempOK = false;
                            if(index == 7 && tempIndex == 0) tempOK = false;
                        }
                        if(tempOK) greenIndex.append(tempIndex);
                        tempIndex = generator_uniform() * 8;
                    }
        }
        for(auto &&index: greenIndex){
            octaDot[index]->setTarget(true);
            octaDot[index]->setBrush(QBrush(Qt::green));
        }
    }
}

void MainWindow::octaReset()
{
    for (int k = 0; k<8 ; k++) octaDot[k]->setAngle(generator_normal() + k*pi/4);
}

void MainWindow::octaCollisionCheck()
{
    for(auto &&dot: octaDot){
        QRectF temp = dot->rect();
        double angle = fmod(dot->getAngle(),2*pi);
        angle += angle < -pi ? 2*pi : (angle > pi ? -2*pi : 0);

        if (temp.x()-dispPadding <= 0){
                dot->setAngle(pi-angle);
                dot->moveDot(0, dispPadding-temp.x());
        }
        if (temp.x()+ readLine(i_dotSize) + dispPadding >= dispWidth){
                dot->setAngle(pi-angle);
                dot->moveDot(0, temp.x()+ readLine(i_dotSize) + dispPadding - dispWidth);
        }

        if (temp.y()-dispPadding <= 0){
                dot->setAngle(-angle);
                dot->moveDot(0, dispPadding-temp.y());
        }
        if (temp.y()+ readLine(i_dotSize) + dispPadding >= dispHeight){
                dot->setAngle(-angle);
                dot->moveDot(0, temp.y()+ readLine(i_dotSize) + dispPadding - dispHeight);
        }
    }
    for (int k = 0; k<8 ; k++){
        QRectF temp1 = octaDot[k]->rect();
        for (int i = k+1; i<8 ; i++){
            QRectF temp2 = octaDot[i]->rect();
            if (m_dist(temp1, temp2) <= readLine(i_dotSize)){
                double tempAngle = octaDot[k]->getAngle();
                double tempDist = octaDot[k]->getDist();
                octaDot[k]->setAngle(octaDot[i]->getAngle());
                octaDot[i]->setAngle(tempAngle);
                octaDot[k]->moveDot(0, octaDot[i]->getDist());
                octaDot[i]->moveDot(0, tempDist);

            }
        }
    }
}

