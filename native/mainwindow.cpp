#include <QtDebug>
#include <QtGui/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTableWidgetItem>
#include <leveldb/db.h>

#include "mainwindow.h"

MyApp::MyApp() {
    setupUi(this);
    setUnifiedTitleAndToolBarOnMac(true);
    auto fileMenu = menubar->addMenu(QStringLiteral("&File"));
    fileMenu->addAction(setupMenu());
}

void MyApp::clearLevelDbTableWidget() {
    levelDbTableWidget->clearContents();
    levelDbTableWidget->setColumnCount(0);
    levelDbTableWidget->setRowCount(0);
}

void MyApp::addLevelDb(QString &levelDbDir) {
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = false;
    leveldb::Status status = leveldb::DB::Open(options, levelDbDir.toStdString(), &db);
    if (!status.ok()) {
        qDebug() << status.ToString();
        return;
    }
    levelDbTableWidget->setColumnCount(2);
    levelDbTableWidget->setHorizontalHeaderLabels({QStringLiteral("Key"), QStringLiteral("Value")});
    levelDbTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    int rowNum = 0;
    auto it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        levelDbTableWidget->insertRow(rowNum);
        auto key = QString::fromStdString(it->key().ToString());
        auto value = QString::fromStdString(it->value().ToString());
        levelDbTableWidget->setItem(rowNum, 0, createItem(key));
        levelDbTableWidget->setItem(rowNum, 1, createItem(value));
        rowNum += 1;
    }
}

QTableWidgetItem *MyApp::createItem(QString &itemString) {
    auto item = new QTableWidgetItem(itemString);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return item;
}

QAction *MyApp::setupMenu() {
    auto fileOpenAction = new QAction(QStringLiteral("Open Database"), this);
    fileOpenAction->setShortcut(QStringLiteral("CTRL+O"));
    fileOpenAction->setStatusTip(QStringLiteral("Open Database"));
    connect(fileOpenAction, &QAction::triggered, [this]() {
        auto fileDirectory =
            QFileDialog().getExistingDirectory(this, QStringLiteral("Select directory"));
        if (fileDirectory != nullptr) {
            clearLevelDbTableWidget();
            addLevelDb(fileDirectory);
            qDebug() << "Directory selected is" << fileDirectory;
        } else {
            qDebug() << "No directory selected";
        }
    });
    return fileOpenAction;
}
