#include <QApplication>
#include <QtWidgets/QWidget>
#include "widgets\main_widgets\QMainWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QMainWidget win;
    win.show();
    return app.exec();
}