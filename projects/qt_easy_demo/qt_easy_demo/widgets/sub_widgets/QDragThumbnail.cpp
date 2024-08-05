#include "QDragThumbnail.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>

QDragThumbnail::QDragThumbnail(QWidget* parent) :
    QWidget(parent)
{
    initUi();
}

void QDragThumbnail::initUi()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setFixedSize(140, 34);

    ImageButton = new QPushButton(this);
    ImageButton->setFixedSize(20, 20);
    ImageButton->setIconSize(QSize(20, 20));
    ImageButton->setFlat(true);
    ImageButton->setStyleSheet("QPushButton{border:0px solid rgb(0, 0, 0);}");
    ImageButton->setFocusPolicy(Qt::NoFocus);
    ImageButton->move(RADIUS - RECT, RADIUS - RECT);

    TextLabel = new QLabel(this);
    TextLabel->setFixedSize(TEXT_LENGTH, 20);
    TextLabel->setAlignment(Qt::AlignVCenter);
    TextLabel->setFont(QFont("Microsoft YaHei", 8, QFont::Normal));
    TextLabel->setStyleSheet("QLabel{color:rgba(255, 255, 255, 255); border:0px solid rgb(0, 0, 0);}");
    TextLabel->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->addWidget(TextLabel);
    layout->setContentsMargins(2 * RADIUS, 0, 0, 2);
}

void QDragThumbnail::setupthumbnail(QIcon icon, QString text)
{
    ImageButton->setIcon(QIcon(icon));

    TextLabel->setText(text);
    int textSize = fontMetrics().width(text);  //字符超长检测
    if (textSize > TEXT_LENGTH) {
        QString Elide_text = fontMetrics().elidedText(text, Qt::ElideRight, TEXT_LENGTH);
        TextLabel->setText(Elide_text);
    }
}

void QDragThumbnail::setIconSize(int size)
{
    ImageButton->setIconSize(QSize(size, size));
}

void QDragThumbnail::paintEvent(QPaintEvent*)
{
    QPainter Painter(this);
    Painter.setRenderHint(QPainter::Antialiasing, true);
    Painter.setPen(Qt::NoPen);
    Painter.setBrush(QColor(114, 164, 250, 200));

    QPainterPath PainterPath;
    PainterPath.addRoundedRect(QRect(0, 0, width(), height()), RADIUS, RADIUS);  //Rect
    PainterPath.addEllipse(RADIUS - ELLIPSE_RADIUS, RADIUS - ELLIPSE_RADIUS, ELLIPSE_RADIUS * 2, ELLIPSE_RADIUS * 2);  //除去内部小圆
    Painter.drawPath(PainterPath);

    Painter.setBrush(QColor(255, 255, 255, 200));
    Painter.drawEllipse(RADIUS - ELLIPSE_RADIUS, RADIUS - ELLIPSE_RADIUS, ELLIPSE_RADIUS * 2, ELLIPSE_RADIUS * 2);  //内部小圆重新上色
}
