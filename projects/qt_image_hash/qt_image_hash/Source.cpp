#include <QApplication>
#include <QtWidgets/QWidget>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "Widgets\QMainWidget.h"
#include "public\qt_demo\StyleSheet.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    qt_demo::SetStyleSheet(":/qss/res/style_sheet.css", app);

    QMainWidget dlg;
    dlg.show();

    return app.exec();
}