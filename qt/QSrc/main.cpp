#include "QSrcWnd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSrcWnd w;
    w.show();
    w.init();

    return a.exec();
}
