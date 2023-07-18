#include <QtCore/QStringDecoder>
#include <QtDebug>
#include <QtGui/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTableWidgetItem>

#include "mainwindow.h"

MyApp::MyApp() {
    setupUi(this);
    setUnifiedTitleAndToolBarOnMac(true);
    connect(actionOpenDatabase, &QAction::triggered, [this]() {
        auto fileDirectory = QFileDialog::getExistingDirectory(
            this,
            QStringLiteral("Select directory"),
            QString(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!fileDirectory.isEmpty()) {
            addLevelDb(fileDirectory);
        }
    });
}

MyApp::~MyApp() {
    workerThread.quit();
    workerThread.wait();
}

void MyApp::addLevelDb(const QString &levelDbDir) {
    levelDbTableWidget->clearContents();
    levelDbTableWidget->setRowCount(0);
    const auto worker = new Worker();
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MyApp::operate, worker, &Worker::doWork);
    connect(worker, &Worker::resultReady, [this](const QMap<QByteArray, QByteArray> &results) {
        auto toUtf16 = QStringDecoder(QStringDecoder::Utf8);
        levelDbTableWidget->setDisabled(true);
        quint64 rowNum = 0;
        for (auto [key, value] : results.asKeyValueRange()) {
            auto keyIsBinary = false;
            auto valueIsBinary = false;
            levelDbTableWidget->insertRow(rowNum);
            QString decodedKey = toUtf16(key);
            keyIsBinary = toUtf16.hasError();
            levelDbTableWidget->setItem(
                rowNum,
                0,
                new QTableWidgetItem(!keyIsBinary ? decodedKey :
                                                    QStringLiteral("0x%1").arg(
                                                        QString::fromLocal8Bit(key.toHex(' ')))));
            toUtf16.resetState();
            QString decodedValue = toUtf16(value);
            valueIsBinary = toUtf16.hasError();
            levelDbTableWidget->setItem(
                rowNum,
                1,
                new QTableWidgetItem(
                    !valueIsBinary ?
                        decodedValue :
                        QStringLiteral("0x%1").arg(QString::fromLocal8Bit(value.toHex(' ')))));
            toUtf16.resetState();
            levelDbTableWidget->setItem(
                rowNum,
                2,
                new QTableWidgetItem(keyIsBinary || valueIsBinary ? tr("Yes") : tr("No")));
            rowNum++;
        }
        levelDbTableWidget->setDisabled(false);
    });
    workerThread.start();
    emit operate(levelDbDir);
}
