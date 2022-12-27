#pragma once

#include <QListView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "public/qt_demo/StyleWidget.h"

class QListWidgetLab
    : public qt_demo::StyleWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QListWidgetLab);

public:
    explicit QListWidgetLab(QWidget* parent = nullptr);
    ~QListWidgetLab();

protected:
    void CreateUI();
    void CreateData();
    void CreateConnect();

protected Q_SLOTS:
    void clicked(bool checked = false);

private:
    QListView* m_pListView = nullptr;
    QPushButton* m_pMainBtn = nullptr;
    QVBoxLayout* m_pMainLayout = nullptr;
    QHBoxLayout* m_pCommandLayout = nullptr;
};