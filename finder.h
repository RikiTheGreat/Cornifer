#ifndef FINDER_H
#define FINDER_H

#include <QObject>
#include <QStringList>

#include <QObject>
#include <QThread>
#include <atomic>
#include <condition_variable>
#include <mutex>

class Finder : public QObject
{
    Q_OBJECT
public:
    explicit Finder(QObject *parent = nullptr);

signals:
    void finished(QStringList  const& files);
public slots:
    void startTask();
    void stopTask();

private:
    bool actualTask();
    void update();
private:
    std::atomic<bool> _isRunning;
    QStringList _files;
    QThread *_t;
};

#endif // FINDER_H
