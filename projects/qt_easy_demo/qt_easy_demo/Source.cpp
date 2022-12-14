#include <QApplication>
#include <QtWidgets/QWidget>
#include "widgets\main_widgets\QMainWidget.h"
#include "public\qt_demo\StyleSheet.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    qt_demo::SetStyleSheet(":/qss/res/style_sheet.css", app);

    QMainWidget win;
    win.show();
    return app.exec();
}