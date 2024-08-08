#pragma once

#include <QWidget>
#include <QVBoxLayout>

#include "QFakeTabBar.h"

class QFakeTabWidget : public QWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QFakeTabWidget);

public:
    explicit QFakeTabWidget(QWidget* parent = nullptr);
    virtual ~QFakeTabWidget();

protected:
    void CreateUI();
    void CreateData();
    void CreateConnect();

public:
    void AddTab(const QString& strTitle);
    void AddTabs(const std::vector<QString>& vctTitle);
    void SetCurrentIndex(int index);

protected Q_SLOTS:
    void currentChangedSlot(int index);
    void tabCloseRequestedSlot(int index);
    void tabMovedSlot(int from, int to);

private:
    void ClearMainLayout();

private:
    QFakeTabBar* m_pTabBar = nullptr;
    std::vector<QWidget*> m_vctWidgets;
    QWidget* m_pShowWidget = nullptr;
    QVBoxLayout* m_pMainLayout = nullptr;
    QVBoxLayout* m_pShowLayout = nullptr;
};