#include <QtWidgets/QApplication>

#include "mainwindow.h"

#define kAppVersion "0.0.1"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("LevelDB Viewer"));
    QCoreApplication::setApplicationVersion(QStringLiteral(kAppVersion));
    QCoreApplication::setOrganizationName(QStringLiteral("Tatsh"));
    MyApp window(argc, argv);
    window.show();
    return app.exec();
}
