#include "QRedDotWidgetLab.h"

#include <QPainter>
#include <QDebug>

QRedDotWidget::QRedDotWidget(QWidget* parent) : QWidget(parent)
{
    setFixedHeight(30);
    m_textColor = QColor(0xFF,0xFF,0xFF);
    m_bgColor = QColor(0xF4,0x56,0x33);
}

void QRedDotWidget::setText(const QString& text)
{
    m_text = text;
    QFont font = this->font(); 
    font.setPointSize(m_fontSize);
    QFontMetrics metrics(font);
    setFixedWidth(metrics.width(text) + height());
    update();
}

void QRedDotWidget::setTextColor(const QColor& color)
{
    m_textColor = color;
    update();
}

void QRedDotWidget::setBgColor(const QColor& color)
{
    m_bgColor = color;
    update();
}

void QRedDotWidget::setFontSize(int size)
{
    m_fontSize = size;
    update();
}

void QRedDotWidget::paintEvent(QPaintEvent* event)
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
    painter.setPen(m_textColor);
    painter.drawText(rect(), Qt::AlignCenter, m_text);
}

QRedDotWidgetLab::QRedDotWidgetLab(QWidget* parent /*= nullptr*/)
{
    setObjectName("QRedDotWidgetLab");
    CreateUI();
    CreateData();
    CreateConnect();
}

void QRedDotWidgetLab::btnTxtConfirm()
{
    if (m_redDotWidget)
    {
        m_redDotWidget->setText(m_inputEdit->text());
    }
}

void QRedDotWidgetLab::CreateUI()
{
    m_inputEdit = new QLineEdit(this);
    m_inputEdit->setGeometry(10, 10, 400, 30);

    m_pBtnMake = new QPushButton(this);
    m_pBtnMake->setGeometry(420, 10, 80, 30);
    m_pBtnMake->setText(QString::fromLocal8Bit("提交"));

    m_redDotWidget = new QRedDotWidget(this);
    m_redDotWidget->setGeometry(10, 70, 200, 90);
}

void QRedDotWidgetLab::CreateData()
{
    if (m_redDotWidget)
    {
        m_redDotWidget->setText(QString::fromLocal8Bit("Hello World"));
    }
}

void QRedDotWidgetLab::CreateConnect()
{
    connect(
        m_pBtnMake,
        &QPushButton::released,
        this, 
        &QRedDotWidgetLab::btnTxtConfirm);
}
