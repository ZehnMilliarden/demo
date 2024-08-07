#pragma once

#include <QStyledItemDelegate>
#include <QPushButton>
#include <QListView>

class QListViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT;
private:
    class QLimitePrivateSiganl {};

public:
    explicit QListViewItemDelegate(QObject* parent = nullptr);
    virtual ~QListViewItemDelegate();

private:
    void CreateData();
    void CreateConnection();

protected: // QStyledItemDelegate
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool eventFilter(QObject* pObject, QEvent* pEvent) override;
    bool editorEvent(QEvent* pEvent, QAbstractItemModel* pModel, const QStyleOptionViewItem& option, const QModelIndex& index) override;

private:
    void paintDrag(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

Q_SIGNALS:
    void buttonClicked(const QModelIndex& index, QLimitePrivateSiganl);
    void onLineClicked(const QModelIndex& index, QLimitePrivateSiganl);
    void onItemSelected(const QModelIndex& index, QListView* pListView, QLimitePrivateSiganl);
    void onMoveItemTo(const QModelIndex& from, const QModelIndex& to, QLimitePrivateSiganl);
    
public Q_SLOTS:
    void onItemInsertedSlot(const QModelIndex& parent, int first, int last);
    void onItemRemovedSlot(const QModelIndex& parent, int first, int last);
    void onItemCountChanged(int nNewItemCount);

protected Q_SLOTS:
    void onButtonClickedSlot(const QModelIndex& index);

protected Q_SLOTS:
    void onUpdateSelectedRowSlot(const QModelIndex& index, QListView* pListView);

private:
    void onHoverMove(QMouseEvent* pMouseEvent, QListView* pListView);
    void onDragMouseMove(QMouseEvent* pMouseEvent, QListView* pListView);
    void onDragEnter(QDragEnterEvent* pDragEnterEvent, QListView* pListView);
    void onDragLeave(QDragLeaveEvent* pDragLeaveEvent, QListView* pListView);
    void onDragMove(QDragMoveEvent* pDragMoveEvent, QListView* pListView);
    void onDropEvent(QDropEvent* pDropEvent, QListView* pListView);

private:
    void SetDragRow(int nRow);
    int GetDragRow() const;
    void SetSelectRow(int nRow);
    int GetSelectRow() const;
    void SetHighLiteRow(int nRow);
    int GetHighLiteRow() const;
    int GetOldHighLiteRow() const;
    void SetDraging(bool bVal);
    bool IsDraging() const;
    QSize GetItemSize() const;
    void SetItemSize(const QSize& size);
    void UpdateItemSize(const int nNewItemCount);
    QString GetMimeDataType() const;
    void SetHoverIndex(const QModelIndex& index);
    QModelIndex GetHoverIndex() const;
    bool IsHoverIndex(const QModelIndex& index) const;
    void SetPressIndex(const QModelIndex& index);
    QModelIndex GetPressIndex() const;
    bool IsPressIndex(const QModelIndex& index) const;
    void SetEventOnBtn(const bool bVal);
    bool IsEventOnBtn() const;
    bool IsDragStartNull() const;
    void SetClickedIndex(const QModelIndex& index);
    QModelIndex GetClickedIndex() const;
    bool IsClickedIndex(const QModelIndex& index) const;

private:
    QModelIndex m_hoveredIndex;
    QModelIndex m_pressedIndex;
    QModelIndex m_clickedIndex;

    QPushButton* m_pButtonTemplate = nullptr;
    QPoint m_dragStartPos;
    int m_nDragIndex = -1;  // -1: not drag
    int m_nInsertIndex = -1; //
    int m_nHighlightedRow = -2;
    int m_nOldHighlightedRow = -2;
    int m_nInsertRow = -1;
    int m_nSelectedRow = -1;
    bool m_bIsDraging = false;
    QSize m_sizeItem = QSize(100, 50);

    bool m_bIsEventOnBtn = false;
};