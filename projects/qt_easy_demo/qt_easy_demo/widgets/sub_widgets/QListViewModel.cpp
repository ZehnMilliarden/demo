#include "QListViewModel.h"

QListViewModel::QListViewModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

QListViewModel::~QListViewModel()
{
}

void QListViewModel::addItem(std::shared_ptr<QListItemData>& lstItem)
{
    beginInsertRows(QModelIndex(), m_listData.size(), m_listData.size());
    m_listData.push_back(lstItem);
    endInsertRows();
}

bool QListViewModel::insertItem(int row, std::shared_ptr<QListItemData>& lstItem)
{
    beginInsertRows(QModelIndex(), m_listData.size(), m_listData.size());
    std::list<std::shared_ptr<QListItemData>>::iterator iter = m_listData.begin();
    std::advance(iter, row);
    m_listData.insert(iter, lstItem);
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

void QListViewModel::swapItem(int nOrigalRow, int nTargetRow)
{
    beginInsertRows(QModelIndex(), m_listData.size(), m_listData.size());
    auto it1 = std::next(m_listData.begin(), nOrigalRow);
    auto it2 = std::next(m_listData.begin(), nTargetRow);
    std::iter_swap(it1, it2);
    endInsertRows();
}

int QListViewModel::GetItemCount()
{
    return m_listData.size();
}

int QListViewModel::rowCount(const QModelIndex& parent) const
{
    return m_listData.size();
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
