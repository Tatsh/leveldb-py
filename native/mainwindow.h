#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
class QTableWidgetItem;
class QMenuBar;
class QAction;
QT_END_NAMESPACE

class MyApp : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    MyApp();

private:
    QAction *setupMenu();
    QTableWidgetItem *createItem(QString &itemString);
    void addLevelDb(QString &levelDbDir);
    void clearLevelDbTableWidget();

    QMenuBar *menuBar;
};

#endif // MAINWINDOW_H
