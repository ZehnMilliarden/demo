#include "RedDotWidgetLab.h"

#include <QPainter>

RedDotWidget::RedDotWidget(QWidget* parent) : QWidget(parent)
{
    setFixedHeight(30);
    m_textColor = QColor("#FFFFFF");
    m_bgColor = QColor("#F45633");
}

void RedDotWidget::setText(const QString& text)
{
    m_text = text;
    QFontMetrics metrics(font());
    setFixedWidth(metrics.width(text) + height());
    update();
}

void RedDotWidget::setTextColor(const QColor& color)
{
    m_textColor = color;
    update();
}

void RedDotWidget::setBgColor(const QColor& color)
{
    m_bgColor = color;
    update();
}

void RedDotWidget::setFontSize(int size)
{
    m_fontSize = size;
    update();
}

void RedDotWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    // 绘制背景
    painter.setBrush(m_bgColor);
    painter.drawRoundedRect(rect(), height() / 2, height() / 2);

    // 绘制左半圆形状
    QRectF leftRect(0, 0, height(), height());
    painter.setBrush(m_bgColor);
    painter.drawPie(leftRect, 90 * 16, 180 * 16);

    // 绘制右半圆形状
    QRectF rightRect(width() - height(), 0, height(), height());
    painter.setBrush(m_bgColor);
    painter.drawPie(rightRect, -90 * 16, 180 * 16);

    // 绘制文案
    QFont font = this->font();
    font.setPointSize(m_fontSize);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter, m_text);
}