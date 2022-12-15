#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "public/qt_demo/StyleWidget.h"

class QNormalWidgetLab
    : public StyleWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QNormalWidgetLab);

public:
    explicit QNormalWidgetLab(QWidget* parent = nullptr);
    ~QNormalWidgetLab();

protected:
    void CreateUI();
    void CreateData();
    void CreateConnect();

protected Q_SLOTS:
    void clicked(bool checked = false);

private:
    QVBoxLayout* m_pMainLayout = nullptr;
    QHBoxLayout* m_pCommandLayout = nullptr;
    QPushButton* m_pMainBtn = nullptr;
};