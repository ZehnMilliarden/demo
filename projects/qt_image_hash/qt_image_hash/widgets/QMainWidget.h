#pragma once

#include <QWidget>

#include "QLeftWidget.h"
#include "QRightWidget.h"

class QMainWidget
    : public QWidget
{
    Q_OBJECT;
public:
    explicit QMainWidget(QWidget* parent = nullptr);
    ~QMainWidget();

private:
    void InitUi();
    void MoveToDesktopCenter();
    void AddLayout();
    QWidget* AddLeftLayout(QLayout* pLayout);
    QWidget* AddRightLayout(QLayout* pLayout);

private:
    QLeftWidget* m_pLeftWidget = nullptr;
    QRightWidget* m_pRightWidget = nullptr;
};