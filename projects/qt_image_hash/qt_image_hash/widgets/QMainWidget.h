#pragma once

#include <QWidget>

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
    void AddLeftLayout(QLayout* pLayout);
    void AddRightLayout(QLayout* pLayout);
};