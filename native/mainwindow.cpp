#include <QtDebug>
#include <QtGui/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTableWidgetItem>

#include "mainwindow.h"

MyApp::MyApp() {
    setupUi(this);
    setUnifiedTitleAndToolBarOnMac(true);
    menubar->addMenu(QStringLiteral("&File"))->addAction(setupMenu());
    levelDbTableWidget->setColumnCount(2);
    levelDbTableWidget->setHorizontalHeaderLabels({QStringLiteral("Key"), QStringLiteral("Value")});
}

MyApp::~MyApp() {
    workerThread.quit();
    workerThread.wait();
}

void MyApp::addLevelDb(const QString &levelDbDir) {
    leveldb::DB *db;
    leveldb::Options dbOptions;
    dbOptions.create_if_missing = false;
    leveldb::Status status = leveldb::DB::Open(dbOptions, levelDbDir.toStdString(), &db);
    if (!status.ok()) {
        qDebug() << status.ToString();
        return;
    }
    levelDbTableWidget->clearContents();
    levelDbTableWidget->setRowCount(0);
    const auto worker = new Worker();
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MyApp::operate, worker, &Worker::doWork);
    connect(worker, &Worker::resultReady, [this](const QMap<QString, QString> &results) {
        levelDbTableWidget->setDisabled(true);
        quint64 rowNum = 0;
        for (auto [key, value] : results.asKeyValueRange()) {
            levelDbTableWidget->insertRow(rowNum);
            levelDbTableWidget->setItem(rowNum, 0, createItem(key));
            levelDbTableWidget->setItem(rowNum, 1, createItem(value));
            rowNum++;
        }
        levelDbTableWidget->setDisabled(false);
    });
    workerThread.start();
    emit operate(db);
}

QTableWidgetItem *MyApp::createItem(const QString &itemString) {
    auto item = new QTableWidgetItem(itemString);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return item;
}

QAction *MyApp::setupMenu() {
    auto fileOpenAction = new QAction(QStringLiteral("&Open Database..."), this);
    fileOpenAction->setShortcut(QStringLiteral("Ctrl+O"));
    fileOpenAction->setStatusTip(QStringLiteral("Open Database"));
    connect(fileOpenAction, &QAction::triggered, [this]() {
        auto fileDirectory = QFileDialog::getExistingDirectory(
            this,
            QStringLiteral("Select directory"),
            QString(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!fileDirectory.isEmpty()) {
            addLevelDb(fileDirectory);
        }
    });
    return fileOpenAction;
}
