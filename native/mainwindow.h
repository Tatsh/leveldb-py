#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QThread>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableWidget>
#include <leveldb/db.h>

#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenuBar;
QT_END_NAMESPACE

class MyApp : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT
    QThread workerThread;

public:
    MyApp();
    ~MyApp();
    static QTableWidgetItem *createItem(const QString &itemString);

private:
    QAction *setupMenu();
    void addLevelDb(const QString &levelDbDir);

    QMenuBar *menuBar;

Q_SIGNALS:
    void operate(leveldb::DB *db);
};

class Worker : public QObject {
    Q_OBJECT

public Q_SLOTS:
    void doWork(leveldb::DB *db) {
        QMap<QString, QString> map;
        const auto it = db->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            map[QString::fromStdString(it->key().ToString())] =
                QString::fromStdString(it->value().ToString());
        }
        emit resultReady(map);
    }

Q_SIGNALS:
    void resultReady(const QMap<QString, QString> &results);
};

#endif // MAINWINDOW_H
