#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <functional>
#include "QFakeTabBar.h"

class QFakeTabWidget : public QWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QFakeTabWidget);

protected:
    class QLimitePrivateSignal {};

public:
    explicit QFakeTabWidget(QWidget* parent = nullptr);
    explicit QFakeTabWidget(QFakeTabBar* pTabBar, QWidget* parent = nullptr);
    virtual ~QFakeTabWidget();

protected:
    void RegisterMetaType();
    void CreateUI();
    void CreateData();
    void CreateConnect();

Q_SIGNALS:
    void tabCloseRequested(int index, QLimitePrivateSignal);

protected Q_SLOTS:
    void currentChangedSlot(int index);
    void tabCloseRequestedSlot(int index);
    void tabMovedSlot(int from, int to);

private:
    void ClearMainLayout();

public:
    void AddTab(const QString& strTitle);
    void AddTabs(const std::vector<QString>& vctTitle);
    void SetCurrentIndex(int index);
    int GetCurrentIndex() const;
    QWidget* GetCurrentWidget() const;
    void RemoveTab(int index);
    QFakeTabBar* tabBar() const;
    int count() const;
    void setWidgetCreater(std::function<QWidget* (int index)> func);
    void setWidgetDestroy(std::function<bool(int index, QWidget*)> func);
    QWidget* widget(int index);

private:
    QFakeTabBar* m_pTabBar = nullptr;
    std::vector<QWidget*> m_vctWidgets;
    QWidget* m_pShowWidget = nullptr;
    QVBoxLayout* m_pMainLayout = nullptr;
    QVBoxLayout* m_pShowLayout = nullptr;
    std::function<QWidget* (int index)> m_funcWidgetCreater = nullptr;
    std::function<bool(int index, QWidget*)> m_funcWidgetDestroy = nullptr;
};