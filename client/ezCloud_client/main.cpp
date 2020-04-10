#include "ezclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EZClient w(QCoreApplication::applicationDirPath());
    return a.exec();
}
