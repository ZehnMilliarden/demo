#pragma once

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QLabel>

#include "public/qt_demo/StyleWidget.h"

// 参考资料1: http://c.biancheng.net/view/9420.html
// 参考资料2: https://www.cnblogs.com/dpsying/p/13622873.html

class QTreeWidgetLab
    : public StyleWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QTreeWidgetLab);

public:
    explicit QTreeWidgetLab(QWidget* parent = nullptr);
    ~QTreeWidgetLab();

protected:
    void CreateUI();
    void CreateData();
    void CreateConnect();

protected Q_SLOTS:
    void treeDoubleClicked(const QModelIndex& index);

private:
    QTreeView* m_pTreeView = nullptr;
    QStandardItemModel* m_pTreeDataModel = nullptr;
    QVBoxLayout* m_pMainLayout = nullptr;
    QLabel* m_pCurrentPath = nullptr;
};