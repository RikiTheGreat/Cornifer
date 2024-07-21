#ifndef FINDER_H
#define FINDER_H

#include <QObject>
#include <QStringList>

#include <QObject>
#include <QThread>
#include <atomic>


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
    void setData(QString const& file, QString const& dir);

private:
    bool actualTask();
    void update();
    void findFiles();

private:
    std::atomic<bool> _isRunning;
    QStringList _files;
    QThread *_t;
    QString _fileName;
    QString _path;
};

#endif // FINDER_H
