#pragma once

#include "public/qt_demo/StyleWidget.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class QRedDotWidget : public QWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QRedDotWidget);

public:
    explicit QRedDotWidget(QWidget* parent = nullptr);
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

class QRedDotWidgetLab : public QWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QRedDotWidgetLab);

public:
    explicit QRedDotWidgetLab(QWidget* parent = nullptr);

public Q_SLOTS:
    void btnTxtConfirm();

protected:
    void CreateUI();
    void CreateData();
    void CreateConnect();

private:
    QLineEdit* m_inputEdit = nullptr;
    QRedDotWidget* m_redDotWidget = nullptr;
    QPushButton* m_pBtnMake = nullptr;
};