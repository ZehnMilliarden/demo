#include "QTreeWidgetLab.h"

#include <QHeaderView>
#include <QDebug>

QTreeWidgetLab::QTreeWidgetLab(QWidget* parent /*= nullptr*/)
    : StyleWidget(parent)
{
    setObjectName("QTreeWidgetLab");
    CreateUI();
    CreateData();
    CreateConnect();
}

QTreeWidgetLab::~QTreeWidgetLab()
{

}

void QTreeWidgetLab::CreateUI()
{
    m_pMainLayout = new QVBoxLayout(this);
    setLayout(m_pMainLayout);

    m_pCurrentPath = new QLabel(this);
    m_pMainLayout->addWidget(m_pCurrentPath);
    m_pCurrentPath->setObjectName("QTreeWidgetLabCurrentPath");

    m_pTreeView = new QTreeView(this);
    m_pMainLayout->addWidget(m_pTreeView);
    m_pTreeView->setEditTriggers(QTreeView::NoEditTriggers);
    m_pTreeView->setSelectionBehavior(QTreeView::SelectRows);
    m_pTreeView->setSelectionMode(QTreeView::SingleSelection);
    m_pTreeView->setAlternatingRowColors(true);

    m_pTreeDataModel = new QStandardItemModel(m_pTreeView);
    m_pTreeView->setModel(m_pTreeDataModel);
    m_pTreeDataModel->setColumnCount(2);
    m_pTreeDataModel->setHeaderData(0, Qt::Horizontal, QStringLiteral("标题一"));
    m_pTreeDataModel->setHeaderData(1, Qt::Horizontal, QStringLiteral("标题二"));
}

void QTreeWidgetLab::CreateData()
{
    for (int i = 0; i < 5; i++)
    {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(QString::number(i)));
        items.append(new QStandardItem(QString::number(i)));
        m_pTreeDataModel->appendRow(items);
    }
}

void QTreeWidgetLab::CreateConnect()
{
    connect(m_pTreeView, &QTreeView::doubleClicked, this, &QTreeWidgetLab::treeDoubleClicked);
}

void QTreeWidgetLab::treeDoubleClicked(const QModelIndex& index)
{
    QStandardItem* itemDetail = m_pTreeDataModel->itemFromIndex(index.siblingAtColumn(1));
    QStandardItem* item = m_pTreeDataModel->itemFromIndex(index.siblingAtColumn(0));
    if (item)
    {
        QString strTxt;
        if (itemDetail)
        {
            strTxt.append(itemDetail->text());
        }

        item->removeRows(0, item->rowCount());

        for (int i = 0; i < 5; i++)
        {
            QList<QStandardItem*> items;
            items.append(new QStandardItem(QString::number(i)));
            items.append(new QStandardItem(strTxt+QString::number(i)));
            item->appendRow(items);
        }
    }
}
