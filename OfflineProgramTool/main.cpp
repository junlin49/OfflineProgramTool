#include "OfflineProgramTool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OfflineProgramTool w;
    w.show();
    return a.exec();
}
