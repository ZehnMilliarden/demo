#pragma once

#include "public/qt_demo/StyleWidget.h"

class RedDotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RedDotWidget(QWidget* parent = nullptr);
    void setText(const QString& text);
    void setTextColor(const QColor& color);
    void setBgColor(const QColor& color);
    void setFontSize(int size);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString m_text;
    QColor m_textColor;
    QColor m_bgColor;
    int m_fontSize = 12;
};