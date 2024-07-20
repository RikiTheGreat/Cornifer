#include "finder.h"
#include <QDebug>

Finder::Finder(QObject *parent)
    : QObject{parent}
{
    _isRunning.store(true);
        _t = new QThread();
}

void Finder::startTask()
{

    if (_t && _t->isRunning()) {
        qWarning() << "Task is already running.";
        return;
    }

    _isRunning.store(true);
    _t = QThread::create(&Finder::update, this);
    connect(_t, &QThread::finished, _t, &QObject::deleteLater);
    connect(_t, &QThread::finished, [this](){
        _t = nullptr;
        qDebug() << "Thread finished.";
    });
    _t->start();
    qDebug() << "Task started in thread:" << _t->currentThreadId();
}

void Finder::stopTask()
{

    if (!_t || !_t->isRunning()) {
        qWarning() << "No running task to stop.";
        return;
    }

    _isRunning.store(false);
    _t->quit();
    _t->wait();
    qDebug() << "Stopping task in thread:" << QThread::currentThreadId();
}

bool Finder::actualTask()
{
    _files.clear();
    for (int i = 0; i < 5; ++i) {
        if (!_isRunning.load()) {
            break;
        }
        _files.append(QString::number(i));
        QThread::sleep(1);
    }

    return true;
}

void Finder::update()
{
    while (_isRunning.load()) {
        if (actualTask()) {
            emit finished(_files);
            _isRunning.store(false);
        }
    }
}
