#include "countdown.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Countdown w;
    w.show();
    return a.exec();
}
