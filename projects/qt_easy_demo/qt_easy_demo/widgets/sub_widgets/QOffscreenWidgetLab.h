#pragma once

#include <QVBoxLayout>
#include <QPushButton>
#include "public/qt_demo/StyleWidget.h"
#include "QOffscreenWidget.h"

class QOffscreenWidgetLab
    : public qt_demo::StyleWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QOffscreenWidgetLab);

public:
    explicit QOffscreenWidgetLab(QWidget* parent = nullptr);
    ~QOffscreenWidgetLab();

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
    QOffscreenWidget* m_pOffscreenWidget = nullptr;
};