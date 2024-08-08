#include "QListViewModel.h"

QListViewModel::QListViewModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

QListViewModel::~QListViewModel()
{
}


bool QListViewModel::insertItems(int row, const std::vector<std::shared_ptr<QListItemData>>& lstItems)
{
    beginInsertRows(QModelIndex(), m_listData.size(), m_listData.size());
    std::list<std::shared_ptr<QListItemData>>::iterator iter = m_listData.begin();
    if (row != -1)
    {
        std::advance(iter, row);
    }
    else
    {
        iter = m_listData.end();
    }
    m_listData.insert(iter, lstItems.begin(), lstItems.end());
    endInsertRows();
    return true;
}

bool QListViewModel::insertItem(int row, std::shared_ptr<QListItemData>& pItem)
{
    beginInsertRows(QModelIndex(), m_listData.size(), m_listData.size());
    std::list<std::shared_ptr<QListItemData>>::iterator iter = m_listData.begin();
    if (row != -1)
    {
        std::advance(iter, row);
    }
    else
    {
        iter = m_listData.end();
    }
    m_listData.insert(iter, pItem);
    endInsertRows();
    return false;
}

void QListViewModel::removeItem(int row)
{
    beginInsertRows(QModelIndex(), m_listData.size(), m_listData.size());
    std::list<std::shared_ptr<QListItemData>>::iterator iter = m_listData.begin();
    std::advance(iter, row);
    if (iter != m_listData.end())
    {
        m_listData.erase(iter);
    }
    endInsertRows();
}

void QListViewModel::removeItems(int row, int nCount)
{
    beginInsertRows(QModelIndex(), m_listData.size(), m_listData.size());
    std::list<std::shared_ptr<QListItemData>>::iterator iterBegin = m_listData.begin();
    std::advance(iterBegin, row);
    std::list<std::shared_ptr<QListItemData>>::iterator iterEnd = iterBegin;
    std::advance(iterEnd, nCount);
    m_listData.erase(iterBegin, iterEnd);
    endInsertRows();
}

void QListViewModel::swapItem(int nOrigalRow, int nTargetRow)
{
    beginInsertRows(QModelIndex(), m_listData.size(), m_listData.size());
    auto it1 = std::next(m_listData.begin(), nOrigalRow);
    auto it2 = std::next(m_listData.begin(), nTargetRow);
    std::iter_swap(it1, it2);
    endInsertRows();
}

int QListViewModel::GetItemCount() const
{
    return m_listData.size();
}

int QListViewModel::rowCount(const QModelIndex& parent) const
{
    return GetItemCount();
}

QVariant QListViewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_listData.size())
    {
        return QVariant();
    }

    auto it = m_listData.begin();
    std::advance(it, index.row());
    std::shared_ptr<QListItemData> listItem = *it;

    switch (role) {
    case Qt::UserRole + 1:
        return listItem->GetIcon();
    case Qt::UserRole + 2:
        return listItem->GetTitleText();
    case Qt::UserRole + 3:
        return listItem->GetBtnText();
    default:
        return QVariant();
    }
}
