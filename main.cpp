#include "game.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game w;
    w.setWindowIcon(QIcon("dots.ico"));
    w.showMaximized();

    return a.exec();
}
