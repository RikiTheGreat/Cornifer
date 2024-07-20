#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _finder = new Finder;
    _finder->moveToThread(&_finderThread);

    connect(ui->findBtn, &QPushButton::clicked, this, &MainWindow::toggleTask);
    connect(&_finderThread, &QThread::finished, _finder, &QObject::deleteLater);
    connect(_finder, &Finder::finished, this, &MainWindow::taskFinished);

    _finderThread.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleTask()
{
    if(_taskRunning) {
        QMetaObject::invokeMethod(_finder, "stopTask");
    }else {
        QMetaObject::invokeMethod(_finder, "startTask");
    }

    _taskRunning = !_taskRunning;
}

void MainWindow::taskFinished(QStringList files)
{

    qInfo() << "task finished haha";
    _taskRunning = false;
    qInfo() << files;

}
