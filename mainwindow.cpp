#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDir>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Cornifer");

    _finder = new Finder;
    _finder->moveToThread(&_finderThread);

    connect(ui->findBtn, &QPushButton::clicked, [this]{
        _finder->setData(ui->fileNameLineEdit->text(), ui->pathLineEdit->text());
    });

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

void MainWindow::taskFinished(QStringList const& files)
{

    qInfo() << "task finished haha";
    _taskRunning = false;
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(files);
    ui->answersListView->setModel(model);
    ui->answersListView->setSelectionMode(QAbstractItemView::SingleSelection);
}
