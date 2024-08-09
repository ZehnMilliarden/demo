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
        QString GetIcon() const { return m_iconPath; }
        void SetIcon(const QString& path) { m_iconPath  = path; }
        QString GetTitleText() const  { return m_titleText; }
        void SetTitleText(const QString& text) { m_titleText = text; }
        QString GetBtnText() const  { return m_btnText; }
        void SetBtnText(const QString& text) { m_btnText = text; }
        QString GetToolTipText() const  { return m_toolTipText; }
        void SetToolTipText(const QString& text) { m_toolTipText = text; }
        QVariant GetVarData() const { return m_varData; }
        void SetVarData(const QVariant& data) { m_varData = data; }

    private:
        QString m_iconPath;
        QString m_titleText;
        QString m_btnText;
        QString m_toolTipText;
        QVariant m_varData;
    };

public:
    explicit QListViewModel(QObject* parent = nullptr);
    virtual ~QListViewModel();

public:
    bool insertItem(int row, std::shared_ptr<QListItemData>& pItem);
    bool insertItems(int row, const std::vector<std::shared_ptr<QListItemData>>& lstItems);
    void removeItem(int row);
    void removeItems(int row, int nCount);
    void swapItem(int nOrigalRow, int nTargetRow);
    int  GetItemCount() const;

public Q_SLOTS:
    void updateData(const QModelIndex& index, int role, const QVariant& data);

public: // QAbstractListModel
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    std::list<std::shared_ptr<QListItemData>> m_listData;
};