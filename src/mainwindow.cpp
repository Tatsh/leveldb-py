#include <QtCore/QStringDecoder>
#include <QtDebug>
#include <QtGui/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTableWidgetItem>

#include "mainwindow.h"

static inline bool convertToUtf16(const QByteArray &ba, QString *out) {
    auto toUtf16 = QStringDecoder(QStringDecoder::Utf8);
    QString decoded = toUtf16(ba);
    *out = decoded;
    return toUtf16.hasError();
}

static inline QString binToHex(const QByteArray &ba) {
    return QStringLiteral("0x%1").arg(QString::fromLocal8Bit(ba.toHex(' ')));
}

MyApp::MyApp(int argc, char *argv[]) {
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
    if (argc >= 2) {
        addLevelDb(QString::fromUtf8(argv[1]));
    }
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
        levelDbTableWidget->setDisabled(true);
        quint64 rowNum = 0;
        for (auto [key, value] : results.asKeyValueRange()) {
            QString decodedKey, decodedValue;
            auto keyIsBinary = convertToUtf16(key, &decodedKey);
            levelDbTableWidget->insertRow(rowNum);
            levelDbTableWidget->setItem(
                rowNum, 0, new QTableWidgetItem(!keyIsBinary ? decodedKey : binToHex(key)));
            auto valueIsBinary = convertToUtf16(value, &decodedValue);
            levelDbTableWidget->setItem(
                rowNum, 1, new QTableWidgetItem(!valueIsBinary ? decodedValue : binToHex(value)));
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
