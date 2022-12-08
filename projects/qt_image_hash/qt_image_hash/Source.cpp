#include <QApplication>
#include <QtWidgets/QWidget>
#include <QLabel>
#include <QFile>
#include <QTextStream>

#include "Widgets\QMainWidget.h"

bool SetStyleSheet(const QString& sheetPath, QApplication& app)
{
    QFile file(sheetPath);
    if (file.open(QFile::ReadOnly))
    {
        QString strStylesheet;
        QTextStream filetext(&file);
        strStylesheet = filetext.readAll();
        file.close();
        app.setStyleSheet(strStylesheet);
        return true;
    }
    return false;
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    SetStyleSheet(":/qss/res/style_sheet.css", app);

    QMainWidget dlg;
    dlg.show();

    return app.exec();
}