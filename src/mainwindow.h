#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QThread>
#include <QtWidgets/QMainWindow>

#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenuBar;
QT_END_NAMESPACE

class MyApp : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    MyApp(int argc, char *argv[]);
    ~MyApp();

private:
    inline void addLevelDb(const QString &levelDbDir);
    QMenuBar *menuBar;
    QThread workerThread;

Q_SIGNALS:
    void operate(const QString &levelDbDir);
};

class Worker : public QObject {
    Q_OBJECT

public Q_SLOTS:
    void doWork(const QString &levelDbDir);

Q_SIGNALS:
    void resultReady(const QList<std::pair<QByteArray, QByteArray>> &results);
};

#endif // MAINWINDOW_H
