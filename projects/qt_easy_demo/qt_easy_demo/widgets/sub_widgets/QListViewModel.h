#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <memory>

class QListViewModel : public QAbstractListModel
{
    Q_OBJECT;
    Q_DISABLE_COPY(QListViewModel);

public:
    class QListItemData
    {
    public:
        QListItemData(const QString& iconPath, const QString& title, const QString& btnText)
            : m_iconPath(iconPath)
            , m_titleText(title)
            , m_btnText(btnText)
        {
        }

    public:
        QString GetIcon() { return m_iconPath; }
        QString GetTitleText() { return m_titleText; }
        QString GetBtnText() { return m_btnText; }

    private:
        QString m_iconPath;
        QString m_titleText;
        QString m_btnText;
    };

public:
    explicit QListViewModel(QObject* parent = nullptr);
    virtual ~QListViewModel();

public:
    void addItem(std::shared_ptr<QListItemData>& lstItem);
    bool insertItem(int row, std::shared_ptr<QListItemData>& lstItem);
    void removeItem(int row);
    void swapItem(int nOrigalRow, int nTargetRow);
    int  GetItemCount();

protected: // QAbstractListModel
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    std::list<std::shared_ptr<QListItemData>> m_listData;
};