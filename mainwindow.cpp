#include "mainwindow.h"
#include "ui_mainwindow.h"

#define readLine(x) (ui->x->text().toInt())


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->b_log_folder->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
    ui->w_warning->setPixmap(QApplication::style()->standardPixmap(QStyle::SP_MessageBoxWarning));
    ui->i_log_folder->setText(QDir::currentPath());

    params.dotSize = readLine(i_dotSize);

    d_seg = new dynSegment(params);
    s_seg = new statSegment(params);

    connect(ui->i_dotSize, &QLineEdit::editingFinished, this, [&](){
       d_seg->updateDotSize();
       s_seg->setDotSize();
    });

    for (auto &&screen: QGuiApplication::screens()){
        ui->i_screenBox->insertItem(0,screen->name().remove(0,4),screen->geometry());
    }

    connect(ui->b_test, &QPushButton::clicked, this, [&](){
        params.GazePt->Start();
        fillParams(Test);

        params.display->showFullScreen();
        params.display->setCursor(Qt::BlankCursor);

        resetRandom();
        s_seg->runStaticSegment();
        d_seg->runDynamicSegment(1, 1, 8);

        params.display->close();
        params.GazePt->Stop();
    });

    connect(ui->b_pokreni, &QPushButton::clicked, this, [&](){       
        if(!ui->i_participant->text().compare("")){
            QMessageBox::warning(this, "Greška",
                                 "Nije unesena šifra korisnika!");
            return;
        }
        params.GazePt->Start();

        fillParams(Recording);

        int dynTaskNum = ui->i_numDynamicTask->value();

        if(ui->i_write_log->isChecked()) params.GazePt->startLog(ui->i_log_folder->text(), ui->i_participant->text());

        params.display->showFullScreen();
        params.display->setCursor(Qt::BlankCursor);

        resetRandom();
        s_seg->runStaticSegment();
        d_seg->runDynamicSegment(1, dynTaskNum, 8);
        resetRandom();
        s_seg->runStaticSegment();
        d_seg->runDynamicSegment(2, dynTaskNum, 8);
        resetRandom();
        s_seg->runStaticSegment();
        d_seg->runDynamicSegment(3, dynTaskNum, 10);
        resetRandom();
        s_seg->runStaticSegment();

        params.display->close();

        if(ui->i_write_log->isChecked()) params.GazePt->stopLog();
        params.GazePt->Stop();
    });

    connect(ui->b_izlaz, &QPushButton::clicked, this, [&](){
        params.display->close();
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillParams(paramSetting setting)
{
    if (setting == Recording){
        QRect &&screenres = ui->i_screenBox->currentData().toRect();

        params.useGaze = ui->i_gazePoint->isChecked();
        params.dispWidth = screenres.width();
        params.dispHeight = screenres.height();
        params.dispCenter = QPoint(screenres.center());
        params.dispScene->setSceneRect(0,0, params.dispWidth, params.dispHeight);
        params.display->move(QPoint(screenres.x(), screenres.y()));
        params.display->resize(params.dispWidth, params.dispHeight);

        params.dotOffset = readLine(i_dotOctaOffset);

        params.timeDot = readLine(i_timeStaticDot);
        params.timeSegment = readLine(i_timeStaticSegment);


        params.timeCenter = readLine(i_timeDynamicCenter);
        params.timeTarget = readLine(i_timeDynamicOctagon);
        params.timeMovement = readLine(i_timeDynamicMovement);
        params.timeAnswer = readLine(i_timeDynamicUserInput);

        params.GazePt->setPerimeter((params.dotPerimeter / params.dispWidth), (params.dotPerimeter / params.dispHeight), params.useGaze);
    }

    if (setting == Test){
        QRect &&screenres = ui->i_screenBox->currentData().toRect();

        params.useGaze = ui->i_gazePoint->isChecked();
        params.dispWidth = screenres.width();
        params.dispHeight = screenres.height();
        params.dispCenter = QPoint(screenres.center());
        params.dispScene->setSceneRect(0,0, params.dispWidth, params.dispHeight);
        params.display->move(QPoint(screenres.x(), screenres.y()));
        params.display->resize(params.dispWidth, params.dispHeight);

        params.dotOffset = readLine(i_dotOctaOffset);

        params.timeDot =  readLine(i_timeStaticDot);
        params.timeSegment = 10;


        params.timeCenter = 2;
        params.timeTarget = 5;
        params.timeMovement = 5;
        params.timeAnswer = 10;

        params.GazePt->setPerimeter((params.dotPerimeter / params.dispWidth), (params.dotPerimeter / params.dispHeight), params.useGaze);
    }

}

