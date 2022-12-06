#include <QApplication>
#include <QtWidgets/QWidget>
#include <QLabel>

#include "Widgets\QMainWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QMainWidget dlg;
    dlg.show();
    return app.exec();
}