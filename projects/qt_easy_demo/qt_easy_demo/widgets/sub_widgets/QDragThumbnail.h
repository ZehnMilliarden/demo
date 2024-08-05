#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>

#define RADIUS 17             //窗口边角的弧度
#define ELLIPSE_RADIUS 12     //内部小圆半径
#define RECT 10               //图标长/宽的一半
#define TEXT_LENGTH 100       //文字长度

class QDragThumbnail : public QWidget
{
    Q_OBJECT
public:
    QDragThumbnail(QWidget* parent = nullptr);
    void setupthumbnail(QIcon icon, QString str);
    void setIconSize(int size);

private:
    void initUi();
    void paintEvent(QPaintEvent*);

private:
    QPushButton* ImageButton = nullptr;
    QLabel* TextLabel = nullptr;
};
