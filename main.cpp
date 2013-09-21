#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationDomain("sitnin.com");
    QCoreApplication::setOrganizationName("Sitnin.com");
    QCoreApplication::setApplicationName("JSONView");
    QCoreApplication::setApplicationVersion("0.0.1");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
