#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>

#include "dynsegment.h"
#include "statsegment.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    enum paramSetting{
        Test,
        Recording
    };

    Ui::MainWindow *ui;

    segParams params;

    dynSegment* d_seg;
    statSegment* s_seg;

    void fillParams(paramSetting setting);

};


#endif // MAINWINDOW_H
