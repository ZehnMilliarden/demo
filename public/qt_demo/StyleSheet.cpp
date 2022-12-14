#include "StyleSheet.h"

#include <QFile>
#include <QTextStream>

bool qt_demo::SetStyleSheet(const QString& sheetPath, QApplication& app)
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