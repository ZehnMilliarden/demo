#pragma once

#include <QWidget>

class QRightWidget
    : public QWidget
{
    Q_OBJECT;
public:
    explicit QRightWidget(QWidget* parent = nullptr);
    ~QRightWidget();

protected:
    void InitUi();
    void AddLayout();
};