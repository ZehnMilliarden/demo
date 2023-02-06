#include "StyleWidget.h"

#include <QPainter>
#include <QStyleOption>

namespace qt_demo
{
    StyleWidget::StyleWidget(QWidget* parent)
        : QWidget(parent)
    {
    }


    void StyleWidget::paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);

        QStyleOption opt;
        opt.init(this);

        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
}
