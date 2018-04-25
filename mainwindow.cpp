#include "mainwindow.h"
#include "ui_mainwindow.h"

#define readLine(x) (ui->x->text().toInt())


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
    dispView->setViewport(new QOpenGLWidget());
    dispView->setStyleSheet(tr("border: 0px solid black"));
    dispView->setScene(dispScene);
    dispView->setPalette(pal);
    dispScene->setBackgroundBrush(QBrush(Qt::black));

    d_seg = new dynSegment(readLine(i_dotSize));
    s_seg = new statSegment(readLine(i_dotSize));

    connect(ui->i_dotSize, &QLineEdit::editingFinished, this, [&](){
       d_seg->setDotSize(readLine(i_dotSize));
       s_seg->setDotSize(readLine(i_dotSize));
    });

    for (auto &&screen: QGuiApplication::screens()){
        ui->i_screenBox->insertItem(0,screen->name().remove(0,4),screen->geometry());
    }

    connect(ui->b_test, &QPushButton::clicked, this, [&](){
      /*  QRect &&screenres = ui->i_screenBox->currentData().toRect();
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
        });*/
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
        dispCenter = QPoint(screenres.center());
        dispScene->setSceneRect(0,0,dispWidth, dispHeight);

        if(ui->i_write_log->isChecked()) GazePt->startLog(ui->i_log_folder->text(), ui->i_participant->text());
        GazePt->setPerimeter((100.0 / dispWidth), (100.0 / dispHeight), temp_useGaze);

        secondDisplay->showFullScreen();
        secondDisplay->setCursor(Qt::BlankCursor);

        resetRandom();
        s_seg->runStaticSegment();
        d_seg->runDynamicSegment(1, temp_taskNum, 8);
        resetRandom();
        s_seg->runStaticSegment();
        d_seg->runDynamicSegment(2, temp_taskNum, 8);
        resetRandom();
        s_seg->runStaticSegment();
        d_seg->runDynamicSegment(3, temp_taskNum, 10);
        resetRandom();
        s_seg->runStaticSegment();

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

