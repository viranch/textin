#include <QtGui/QApplication>
#include "textin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Viranch Mehta, Inc.");
    QCoreApplication::setApplicationName("TextIn");

    TextIn w;
    w.show();

    return a.exec();
}
