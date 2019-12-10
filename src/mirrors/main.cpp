#include "screenmirrors.h"
#include <QApplication>

QString libPath;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenMirrors w;
    w.show();
    return a.exec();
}
