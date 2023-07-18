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
    void clearLevelDbTableWidget();
    void addLevelDb(QString &levelDbDir);
    QTableWidgetItem *createItem(QString &itemString);
    QAction *setupMenu();
    QMenuBar *menuBar;
};

#endif // MAINWINDOW_H
