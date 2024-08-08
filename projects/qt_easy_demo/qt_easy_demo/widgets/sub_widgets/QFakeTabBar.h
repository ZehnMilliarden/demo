#pragma once

#include <QWidget>
#include <QListView>
#include <QHBoxLayout>

#include "QListViewItemDelegate.h"
#include "QListViewModel.h"

class QFakeTabBar : public QWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QFakeTabBar);

    class QLimitePrivateSignal {};

public:
    explicit QFakeTabBar(QWidget* parent = nullptr);
    virtual ~QFakeTabBar();

Q_SIGNALS:
    void currentChanged(int nIndex, QLimitePrivateSignal);
    void tabCloseRequested(int nIndex, QLimitePrivateSignal);
    void tabMoved(int from, int to, QLimitePrivateSignal);

private Q_SLOTS:
    void currentChangedSlot(const QModelIndex& current, const QModelIndex& previous);
    void tabCloseRequestedSlot(const QModelIndex& index);
    void tabMovedSlot(const QModelIndex& from, const QModelIndex& to);

protected:
    void RegisterMetaType();
    void CreateUI();
    void CreateData();
    void CreateConnect();

public:
    void addTab(const QString& strTitle);
    void addTab(const QString& strTitle, int nOffset);
    void addTabs(std::vector<QString>& vctTitle);
    void addTabs(std::vector<QString>& vctTitle, int nOffset);
    int  GetTabCount();
    bool SetCurrentIndex(int index);

private:
    QListView* m_pListView = nullptr;
    QHBoxLayout* m_pTabBarListLayout = nullptr;
    QWidget* m_pOptWidget = nullptr;

    QListViewItemDelegate* m_pDelegate = nullptr;
    QListViewModel* m_pModel = nullptr;
};