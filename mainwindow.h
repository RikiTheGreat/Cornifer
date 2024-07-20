#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include "finder.h"
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleTask();
    void taskFinished(QStringList  files);
private:


private:
    Ui::MainWindow *ui;
    Finder *_finder;
    QThread _finderThread;
    bool _taskRunning = false;
};
#endif // MAINWINDOW_H
