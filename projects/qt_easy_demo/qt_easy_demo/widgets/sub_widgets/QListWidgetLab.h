#pragma once

#include <QListView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractListModel>
#include <QStyledItemDelegate>

#include "public/qt_demo/StyleWidget.h"
#include "QListViewItemDelegate.h"
#include "QListViewModel.h"

class QListWidgetLab
    : public qt_demo::StyleWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QListWidgetLab);

public:
    explicit QListWidgetLab(QWidget* parent = nullptr);
    virtual ~QListWidgetLab();

protected:
    void CreateUI();
    void CreateData();
    void CreateConnect();

protected Q_SLOTS:
    void btnClicked(bool checked = false);
    void updateButtonVisibility();

Q_SIGNALS:
    void onItemCountChangedSignal(int nNewItemCount);

private:
    void addItemImpl(int nCount);

private:
    QListView* m_pListView = nullptr;
    QPushButton* m_pMainBtn = nullptr;
    QPushButton* m_pScrollToTop = nullptr;
    QPushButton* m_pScrollToButtom = nullptr;
    QVBoxLayout* m_pMainLayout = nullptr;
    QHBoxLayout* m_pCommandLayout = nullptr;

    QListViewItemDelegate* m_pDelegate = nullptr;
    QListViewModel* m_pModel = nullptr;
};