#include <QApplication>
#include <QtWidgets/QWidget>
#include <QLabel>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QLabel label;
    label.setText("Hello World!!!");
    label.show();
    return app.exec();
}