#include "finder.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QDirIterator>

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

void Finder::setData(const QString &file, const QString &dir)
{
    _fileName = file;
    _path = dir;
}

bool Finder::actualTask()
{
    _files.clear();

    findFiles();
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

void Finder::findFiles()
{
        QDir dir(_path);
        if(!dir.exists()) {
            qWarning() << "Invalid Path: " << _path;
            _isRunning.store(false);
        }

        if(_fileName.isEmpty()) {
            qWarning() << "Empty name: " << _fileName;
            _isRunning.store(false);
        }
        QDirIterator it (dir, QDirIterator::Subdirectories);
        while(it.hasNext()) {
            QFileInfo fileInfo = it.nextFileInfo();
            if(fileInfo.fileName().contains(_fileName) || fileInfo.fileName() == _fileName) {
                _files.append(fileInfo.absolutePath() + QDir::separator() + fileInfo.fileName());
            }
        }
        _isRunning.store(false);

}
