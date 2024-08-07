#include "QListViewItemDelegate.h"
#include <QPainter>
#include <QListView>
#include <QEvent>
#include <QHoverEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QByteArray>
#include <QDataStream>
#include <QMimeData>
#include <QDrag>
#include <QStandardItemModel>

#include "QDragThumbnail.h"
#include "QListViewModel.h"

QListViewItemDelegate::QListViewItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
    CreateData();
    CreateConnection();
}

QListViewItemDelegate::~QListViewItemDelegate()
{
}

void QListViewItemDelegate::CreateData()
{
    m_pButtonTemplate = new QPushButton;
    m_pButtonTemplate->setStyleSheet(
        "QPushButton {"
        "    background-color: lightgray;" // 普通状态
        "    color: black;"
        "    border: 2px solid black;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: lightblue;" // 悬停状态
        "    color: white;"
        "}"
        "QPushButton:pressed {"
        "    background-color: lightgreen;" // 按下状态
        "    color: darkgreen;"
        "}"
    );
}

void QListViewItemDelegate::CreateConnection()
{
    QObject::connect(this, &QListViewItemDelegate::buttonClicked, this, &QListViewItemDelegate::onButtonClickedSlot);
}

void QListViewItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    QString iconPath = index.data(Qt::UserRole + 1).toString();
    QString titleText = index.data(Qt::UserRole + 2).toString();
    QString btnText = index.data(Qt::UserRole + 3).toString();
    QIcon icon(iconPath);

    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    painter->save();

    painter->drawPixmap(option.rect.topLeft(), icon.pixmap(32, 32));
    painter->drawText(option.rect.adjusted(36, 0, 0, 0), titleText);

    // 绘制按钮
    QStyleOptionButton buttonOption;
    buttonOption.rect = QRect(option.rect.right() - 120, option.rect.top(), 115, option.rect.height());
    buttonOption.icon = QIcon(iconPath);
    buttonOption.text = btnText;
    buttonOption.state = QStyle::State_Enabled;

    if (GetClickedIndex().isValid())
    {
        if (IsClickedIndex(index))
        {
            buttonOption.state |= QStyle::State_Sunken;
        }
    }
    else
    {
        // 检查鼠标状态
        if (IsEventOnBtn())
        {
            if (IsHoverIndex(index))
            {
                buttonOption.state |= QStyle::State_MouseOver;
            }
        }
    }
    

    // 使用隐藏按钮的样式绘制虚拟按钮
    QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter, m_pButtonTemplate);

    if (IsHoverIndex(index))
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(255, 0, 0, 50));  // 红色半透明背景
        painter->drawRect(option.rect);
    }

    if (IsDraging())
    {
        paintDrag(painter, option, index);
    }
    
    painter->restore();
}

QSize QListViewItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return GetItemSize();
}

bool QListViewItemDelegate::eventFilter(QObject* pObject, QEvent* pEvent)
{
    if (pEvent->type() == QEvent::HoverEnter) {
        QHoverEvent* hoverEvent = dynamic_cast<QHoverEvent*>(pEvent);
        if (hoverEvent) {
            QListView* pListView = qobject_cast<QListView*>(pObject->parent());
            if (pListView) {
                QModelIndex index = pListView->indexAt(hoverEvent->pos());
                SetHoverIndex(index);
                pListView->viewport()->update();
            }
        }
    }
    else if (pEvent->type() == QEvent::HoverLeave) {
        QHoverEvent* hoverEvent = dynamic_cast<QHoverEvent*>(pEvent);
        if (hoverEvent) {
            QListView* pListView = qobject_cast<QListView*>(pObject->parent());
            if (pListView) {
                QModelIndex index = pListView->indexAt(hoverEvent->pos());
                pListView->viewport()->update();
            }
            SetHoverIndex(QModelIndex());
        }
    }
    else if (pEvent->type() == QEvent::MouseMove)
    {
        QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
        QListView* pListView = qobject_cast<QListView*>(pObject->parent());

        if (pMouseEvent && pListView)
        {
            SetSelectRow(pListView->currentIndex().row());
            if (!IsDragStartNull() && (pMouseEvent->buttons() & Qt::LeftButton))
            {
                onDragMouseMove(pMouseEvent, pListView);
            }
            else
            {
                onHoverMove(pMouseEvent, pListView);
            }
        }
    }
    else if (pEvent->type() == QEvent::MouseButtonPress)
    {
        QListView* pListView = qobject_cast<QListView*>(pObject->parent());
        if (pListView)
        {
            QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);

            QModelIndex index = pListView->indexAt(pMouseEvent->pos());
            QRect itemRect = pListView->visualRect(index);

            QRect btnRect = QRect(itemRect.right() - 120, itemRect.top(), 115, itemRect.height());

            m_dragStartPos = QPoint();

            if (!btnRect.contains(pMouseEvent->pos()))
            {
                if (pMouseEvent->buttons() & Qt::LeftButton)
                {
                    m_dragStartPos = pMouseEvent->pos();
                }
            }

            SetPressIndex(index);
            pListView->viewport()->update();
        }
    }
    else if (pEvent->type() == QEvent::MouseButtonRelease)
    {
        QListView* pListView = qobject_cast<QListView*>(pObject->parent());
        if (pListView)
        {
            QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
            QModelIndex index = pListView->indexAt(pMouseEvent->pos());
            pListView->setCurrentIndex(index);
            SetPressIndex(QModelIndex());
            SetClickedIndex(QModelIndex());
            pListView->viewport()->update();
        }
    }
    else if (pEvent->type() == QEvent::DragEnter)
    {
        if (!IsDragStartNull()) {
            QListView* pListView = qobject_cast<QListView*>(pObject->parent());
            QDragEnterEvent* pDragEnterEvent = dynamic_cast<QDragEnterEvent*>(pEvent);
            if (pListView && pDragEnterEvent)
            {
                onDragEnter(pDragEnterEvent, pListView);
                return true;
            }
        }
    }
    else if (pEvent->type() == QEvent::DragMove)
    {
        if (!IsDragStartNull())
        {
            QListView* pListView = qobject_cast<QListView*>(pObject->parent());
            QDragMoveEvent* pDragMoveEvent = dynamic_cast<QDragMoveEvent*>(pEvent);
            if (pListView && pDragMoveEvent)
            {
                onDragMove(pDragMoveEvent, pListView);
                return true;
            }
        }
    }
    else if (pEvent->type() == QEvent::DragLeave)
    {
        if (!IsDragStartNull())
        {
            QListView* pListView = qobject_cast<QListView*>(pObject->parent());
            QDragLeaveEvent* pDragLeaveEvent = dynamic_cast<QDragLeaveEvent*>(pEvent);
            if (pListView && pDragLeaveEvent)
            {
                onDragLeave(pDragLeaveEvent, pListView);
                return true;
            }
        }
    }
    else if (pEvent->type() == QEvent::Drop)
    {
        QListView* pListView = qobject_cast<QListView*>(pObject->parent());
        QDropEvent* pDropEvent = dynamic_cast<QDropEvent*>(pEvent);

        if (pListView && pDropEvent)
        {
            onDropEvent(pDropEvent, pListView);
            return true;
        }
    }

    return QStyledItemDelegate::eventFilter(pObject, pEvent);
}

bool QListViewItemDelegate::editorEvent(QEvent* pEvent, QAbstractItemModel* pModel, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    if (pEvent->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
        QRect buttonRect = QRect(option.rect.right() - 120, option.rect.top(), 115, option.rect.height());
        if (buttonRect.contains(mouseEvent->pos())) {
            SetClickedIndex(index);
        }
    }
    else if (pEvent->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
        QRect buttonRect = QRect(option.rect.right() - 120, option.rect.top(), 115, option.rect.height());
        if (buttonRect.contains(mouseEvent->pos())) {
            emit buttonClicked(index);
        }
        SetClickedIndex(QModelIndex());
    }
    else if (pEvent->type() == QEvent::MouseMove 
        || pEvent->type() == QEvent::Leave)
    {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
        QRect buttonRect = QRect(option.rect.right() - 120, option.rect.top(), 115, option.rect.height());
        if (buttonRect.contains(mouseEvent->pos())) {
            SetEventOnBtn(true);
        }
        else {
            SetEventOnBtn(false);
        }
    }
    else if (pEvent->type() == QEvent::Wheel)
    {
        QWheelEvent* pWheelEvent = dynamic_cast<QWheelEvent*>(pEvent);
        QRect buttonRect = QRect(option.rect.right() - 120, option.rect.top(), 115, option.rect.height());
        if (buttonRect.contains(pWheelEvent->pos())) {
            SetEventOnBtn(true);
        }
        else {
            SetEventOnBtn(false);
        }
    }
    else if (pEvent->type() == QEvent::Enter)
    {
        QEnterEvent* pEnterEvent = dynamic_cast<QEnterEvent*>(pEvent);
        QRect buttonRect = QRect(option.rect.right() - 120, option.rect.top(), 115, option.rect.height());
        if (buttonRect.contains(pEnterEvent->pos())) {
            SetEventOnBtn(true);
        }
        else {
            SetEventOnBtn(false);
        }
    }
    else if (pEvent->type() == QEvent::Enter)
    {
        QEnterEvent* pEnterEvent = dynamic_cast<QEnterEvent*>(pEvent);
        QRect buttonRect = QRect(option.rect.right() - 120, option.rect.top(), 115, option.rect.height());
        if (buttonRect.contains(pEnterEvent->pos())) {
            SetEventOnBtn(true);
        }
        else {
            SetEventOnBtn(false);
        }
    }

    return QStyledItemDelegate::editorEvent(pEvent, pModel, option, index);
}

void QListViewItemDelegate::paintDrag(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QListView* dragView = qobject_cast<QListView*>(option.styleObject);
    int theDragRow = GetDragRow();
    int theSelectedRow = GetSelectRow();
    int UpRow = GetHighLiteRow();
    int DownRow = UpRow + 1;
    int rowCount = dragView->model()->rowCount() - 1;
    QRect rect = option.rect;

    const int nPOLYGON = 4;
    const int nWIDTH = 1;

    //只绘制空隙,而不绘制DropIndicator(这种情况是当拖拽行非选中行时，需要在选中行上面或者下面绘制空隙用来显示DropIndicator)
    if (index.row() == theSelectedRow && theDragRow != theSelectedRow) {
        if (index.row() == UpRow && index.row() != theDragRow - 1) {
            int offset = 3;
            QPolygon trianglePolygon_bottomLeft;
            trianglePolygon_bottomLeft << QPoint(rect.bottomLeft().x(), rect.bottomLeft().y() - (offset + nWIDTH) + 1);
            trianglePolygon_bottomLeft << QPoint(rect.bottomLeft().x(), rect.bottomLeft().y() - (offset + nWIDTH + nPOLYGON) + 1);
            trianglePolygon_bottomLeft << QPoint(rect.bottomLeft().x() + nPOLYGON, rect.bottomLeft().y() - (offset + nWIDTH) + 1);

            QPolygon trianglePolygon_bottomRight;
            trianglePolygon_bottomRight << QPoint(rect.bottomRight().x() + 1, rect.bottomRight().y() - (offset + nWIDTH) + 1);
            trianglePolygon_bottomRight << QPoint(rect.bottomRight().x() + 1, rect.bottomRight().y() - (offset + nWIDTH + nPOLYGON) + 1);
            trianglePolygon_bottomRight << QPoint(rect.bottomRight().x() - nPOLYGON + 1, rect.bottomRight().y() - (offset + nWIDTH) + 1);

            painter->setBrush(QColor(245, 245, 247));
            painter->drawPolygon(trianglePolygon_bottomLeft);
            painter->drawPolygon(trianglePolygon_bottomRight);
            painter->drawRect(rect.bottomLeft().x(), rect.bottomLeft().y() - (offset + nWIDTH) + 1, rect.width(), offset + nWIDTH);
            painter->drawRect(rect.bottomLeft().x(), rect.bottomLeft().y() - (offset + nWIDTH) + 1, rect.width(), offset + nWIDTH);
        }
        else if (index.row() == DownRow && index.row() != theDragRow + 1) {
            int offset = 3;

            QPolygon trianglePolygon_topLeft;
            trianglePolygon_topLeft << QPoint(rect.topLeft().x(), rect.topLeft().y() + offset + nWIDTH);
            trianglePolygon_topLeft << QPoint(rect.topLeft().x(), rect.topLeft().y() + offset + nWIDTH + nPOLYGON);
            trianglePolygon_topLeft << QPoint(rect.topLeft().x() + nPOLYGON, rect.topLeft().y() + offset + nWIDTH);

            QPolygon trianglePolygon_topRight;
            trianglePolygon_topRight << QPoint(rect.topRight().x() + 1, rect.topRight().y() + offset + nWIDTH);
            trianglePolygon_topRight << QPoint(rect.topRight().x() + 1, rect.topRight().y() + offset + nWIDTH + nPOLYGON);
            trianglePolygon_topRight << QPoint(rect.topRight().x() - nPOLYGON + 1, rect.topRight().y() + offset + nWIDTH);

            painter->setBrush(QColor(245, 245, 247));
            painter->drawPolygon(trianglePolygon_topLeft);
            painter->drawPolygon(trianglePolygon_topRight);
            painter->drawRect(rect.topLeft().x(), rect.topLeft().y(), rect.width(), offset + nWIDTH);
        }
    }

    //绘制DropIndicator
    if (index.row() == UpRow && index.row() != theDragRow - 1 && index.row() != theDragRow) {
        painter->setBrush(QColor(66, 133, 244));

        if (UpRow == rowCount) {
            //到达尾部,三角形向上移动一个WIDTH的距离,以使分隔符宽度*2
            QPolygon trianglePolygon_bottomLeft;
            trianglePolygon_bottomLeft << QPoint(rect.bottomLeft().x(), rect.bottomLeft().y() - (nPOLYGON + nWIDTH) + 1 - nWIDTH);
            trianglePolygon_bottomLeft << QPoint(rect.bottomLeft().x(), rect.bottomLeft().y() - nWIDTH + 1 - nWIDTH);
            trianglePolygon_bottomLeft << QPoint(rect.bottomLeft().x() + nPOLYGON, rect.bottomLeft().y() - nWIDTH + 1 - nWIDTH);

            QPolygon trianglePolygon_bottomRight;
            trianglePolygon_bottomRight << QPoint(rect.bottomRight().x() + 1, rect.bottomRight().y() - (nPOLYGON + nWIDTH) + 1 - nWIDTH);
            trianglePolygon_bottomRight << QPoint(rect.bottomRight().x() + 1, rect.bottomRight().y() - nWIDTH + 1 - nWIDTH);
            trianglePolygon_bottomRight << QPoint(rect.bottomRight().x() - nPOLYGON + 1, rect.bottomRight().y() - nWIDTH + 1 - nWIDTH);

            painter->drawRect(rect.bottomLeft().x(), rect.bottomLeft().y() - 2 * nWIDTH + 1, rect.width(), 2 * nWIDTH);  //rect
            painter->drawPolygon(trianglePolygon_bottomLeft);
            painter->drawPolygon(trianglePolygon_bottomRight);
        }
        else {
            //正常情况,组成上半部分(+1是根据实际情况修正)
            QPolygon trianglePolygon_bottomLeft;
            trianglePolygon_bottomLeft << QPoint(rect.bottomLeft().x(), rect.bottomLeft().y() - (nPOLYGON + nWIDTH) + 1);
            trianglePolygon_bottomLeft << QPoint(rect.bottomLeft().x(), rect.bottomLeft().y() - nWIDTH + 1);
            trianglePolygon_bottomLeft << QPoint(rect.bottomLeft().x() + nPOLYGON, rect.bottomLeft().y() - nWIDTH + 1);

            QPolygon trianglePolygon_bottomRight;
            trianglePolygon_bottomRight << QPoint(rect.bottomRight().x() + 1, rect.bottomRight().y() - (nPOLYGON + nWIDTH) + 1);
            trianglePolygon_bottomRight << QPoint(rect.bottomRight().x() + 1, rect.bottomRight().y() - nWIDTH + 1);
            trianglePolygon_bottomRight << QPoint(rect.bottomRight().x() - nPOLYGON + 1, rect.bottomRight().y() - nWIDTH + 1);

            painter->drawRect(rect.bottomLeft().x(), rect.bottomLeft().y() - nWIDTH + 1, rect.width(), nWIDTH);  //rect
            painter->drawPolygon(trianglePolygon_bottomLeft);
            painter->drawPolygon(trianglePolygon_bottomRight);
        }
    }
    else if (index.row() == DownRow && index.row() != theDragRow + 1 && index.row() != theDragRow) {
        painter->setBrush(QColor(66, 133, 244));

        if (DownRow == 0) {
            //reach the head
            QPolygon trianglePolygon_topLeft;
            trianglePolygon_topLeft << QPoint(rect.topLeft().x(), rect.topLeft().y() + (nPOLYGON + nWIDTH) + nWIDTH);
            trianglePolygon_topLeft << QPoint(rect.topLeft().x(), rect.topLeft().y() + nWIDTH + nWIDTH);
            trianglePolygon_topLeft << QPoint(rect.topLeft().x() + nPOLYGON, rect.topLeft().y() + nWIDTH + nWIDTH);

            QPolygon trianglePolygon_topRight;
            trianglePolygon_topRight << QPoint(rect.topRight().x() + 1, rect.topRight().y() + (nPOLYGON + nWIDTH) + nWIDTH);
            trianglePolygon_topRight << QPoint(rect.topRight().x() + 1, rect.topRight().y() + nWIDTH + nWIDTH);
            trianglePolygon_topRight << QPoint(rect.topRight().x() - nPOLYGON + 1, rect.topRight().y() + nWIDTH + nWIDTH);

            painter->drawRect(rect.topLeft().x(), rect.topLeft().y(), rect.width(), 2 * nWIDTH);  //rect
            painter->drawPolygon(trianglePolygon_topLeft);
            painter->drawPolygon(trianglePolygon_topRight);
        }
        else {
            //normal
            QPolygon trianglePolygon_topLeft;
            trianglePolygon_topLeft << QPoint(rect.topLeft().x(), rect.topLeft().y() + (nPOLYGON + nWIDTH));
            trianglePolygon_topLeft << QPoint(rect.topLeft().x(), rect.topLeft().y() + nWIDTH);
            trianglePolygon_topLeft << QPoint(rect.topLeft().x() + nPOLYGON, rect.topLeft().y() + nWIDTH);

            QPolygon trianglePolygon_topRight;
            trianglePolygon_topRight << QPoint(rect.topRight().x() + 1, rect.topRight().y() + (nPOLYGON + nWIDTH));
            trianglePolygon_topRight << QPoint(rect.topRight().x() + 1, rect.topRight().y() + nWIDTH);
            trianglePolygon_topRight << QPoint(rect.topRight().x() - nPOLYGON + 1, rect.topRight().y() + nWIDTH);

            painter->drawRect(rect.topLeft().x(), rect.topLeft().y(), rect.width(), nWIDTH);  //rect
            painter->drawPolygon(trianglePolygon_topLeft);
            painter->drawPolygon(trianglePolygon_topRight);
        }
    }

    //高亮拖拽行(使拖拽行的样式和选中相同)
    if (index.row() == theDragRow && theDragRow != theSelectedRow) {

        painter->setBrush(QColor(180, 0, 0));
        painter->drawRect(rect.topLeft().x(), rect.topLeft().y(), 4, rect.height());

        painter->setBrush(QColor(230, 231, 234));
        painter->drawRect(rect.topLeft().x() + 4, rect.topLeft().y(), rect.width() - 4, rect.height());

        //opt.state is used to hightlight the font
        QStyleOptionViewItem opt(option);
        opt.state |= QStyle::State_Selected;
        QStyledItemDelegate::paint(painter, opt, index);
    }

    return;
}

void QListViewItemDelegate::onHoverMove(QMouseEvent* pMouseEvent, QListView* pListView)
{
    QModelIndex index = pListView->indexAt(pMouseEvent->pos());
    SetHoverIndex(index);
    pListView->viewport()->update();
}

void QListViewItemDelegate::onDragMouseMove(QMouseEvent* pMouseEvent, QListView* pListView)
{
    QModelIndex theDragIndex = pListView->indexAt(m_dragStartPos);
    m_nDragIndex = theDragIndex.row();
    QString strIconPath = theDragIndex.data(Qt::UserRole + 1).toString();
    QString strTitleText = theDragIndex.data(Qt::UserRole + 2).toString();
    QString strBtnText = theDragIndex.data(Qt::UserRole + 3).toString();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << strIconPath << strTitleText << strBtnText;

    QMimeData* mimeData = new QMimeData;
    mimeData->setData(GetMimeDataType(), itemData);

    QDragThumbnail* pDragThumbnail = new QDragThumbnail(pListView);
    pDragThumbnail->setupthumbnail(QIcon(strIconPath), strTitleText);
    QPixmap pixmap = pDragThumbnail->grab();

    QDrag* pDrag = new QDrag(pListView);
    pDrag->setMimeData(mimeData);
    pDrag->setPixmap(pixmap);
    pDrag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
    const Qt::DropAction emDropAction = pDrag->exec(Qt::MoveAction);

    if (emDropAction == Qt::MoveAction)
    {
        int theRemoveRow = -1;
        if (m_nInsertIndex < m_nDragIndex) theRemoveRow = m_nDragIndex + 1;
        else theRemoveRow = m_nDragIndex;
        pListView->model()->removeRow(theRemoveRow);
    }
}

void QListViewItemDelegate::onDragEnter(QDragEnterEvent* pDragEnterEvent, QListView* pListView)
{
    SetDraging(true);
    pDragEnterEvent->setDropAction(Qt::MoveAction);
    pDragEnterEvent->accept();
}

void QListViewItemDelegate::onDragLeave(QDragLeaveEvent* pDragLeaveEvent, QListView* pListView)
{
    SetHighLiteRow(-2);

    pListView->update(pListView->model()->index(m_nOldHighlightedRow, 0));  //UpRow
    pListView->update(pListView->model()->index(m_nOldHighlightedRow + 1, 0));  //DownRow

    SetDraging(false);  //IsDraging(标志位)判断是否正在拖拽

    m_nInsertRow = -1;
    pDragLeaveEvent->accept();
}

void QListViewItemDelegate::onDragMove(QDragMoveEvent* pDragMoveEvent, QListView* pListView)
{
    int nOffset =GetItemSize().height() / 2 - 1;
    SetHighLiteRow(pListView->indexAt(pDragMoveEvent->pos() - QPoint(0, nOffset)).row());

    //offset() = 19 = 40 / 2 - 1，其中40是行高
    if (pDragMoveEvent->pos().y() >= nOffset) {

        if (m_nOldHighlightedRow != m_nHighlightedRow) {
            //刷新旧区域使dropIndicator消失
            pListView->update(pListView->model()->index(GetOldHighLiteRow(), 0));
            pListView->update(pListView->model()->index(GetOldHighLiteRow() + 1, 0));

            //刷新新区域使dropIndicator显示
            pListView->update(pListView->model()->index(GetHighLiteRow(), 0));
            pListView->update(pListView->model()->index(GetHighLiteRow() + 1, 0));
        }
        else {
            pListView->update(pListView->model()->index(GetHighLiteRow(), 0));
            pListView->update(pListView->model()->index(GetHighLiteRow() + 1, 0));
        }

        m_nInsertRow = GetHighLiteRow() + 1;
    }
    else {
        SetHighLiteRow(-1);
        pListView->update(pListView->model()->index(0, 0));
        pListView->update(pListView->model()->index(1, 0));
        m_nInsertRow = 0;
    }

    pDragMoveEvent->setDropAction(Qt::MoveAction);
    pDragMoveEvent->accept();
}

void QListViewItemDelegate::onDropEvent(QDropEvent* pDropEvent, QListView* pListView)
{
    SetDraging(false); //IsDraging(标志位)判断是否正在拖拽
    SetHighLiteRow(-2);

    //刷新旧区域使dropIndicator消失
    pListView->update(pListView->model()->index(GetOldHighLiteRow(), 0));
    pListView->update(pListView->model()->index(GetOldHighLiteRow() + 1, 0));

    SetPressIndex(QModelIndex());
    SetClickedIndex(QModelIndex());

    if (m_nInsertRow == m_nDragIndex 
        || m_nInsertRow == m_nDragIndex + 1)
    {
        return;
    }

    QString text;
    QString iconPath;
    QString btnText;
    QByteArray byteArrayData = pDropEvent->mimeData()->data(GetMimeDataType());
    QDataStream dataStream(&byteArrayData, QIODevice::ReadOnly);
    dataStream >> iconPath >> text >> btnText;

    QListViewModel* pModel = static_cast<QListViewModel*>(pListView->model());
    std::shared_ptr<QListViewModel::QListItemData> pItemData
        = std::make_shared<QListViewModel::QListItemData>(
            iconPath,
            text,
            btnText);

    if (m_nDragIndex != -1)
    {
        pModel->removeItem(m_nDragIndex);
    }

    if (m_nDragIndex > m_nInsertRow)
    {
        pModel->insertItem(m_nInsertRow, pItemData);
    }
    else
    {
        pModel->insertItem(m_nInsertRow - 1, pItemData);
    }

    UpdateItemSize(pModel->GetItemCount());

    pDropEvent->setDropAction(Qt::MoveAction);
    pDropEvent->accept();
}

void QListViewItemDelegate::SetDragRow(int nRow)
{
    m_nDragIndex = nRow;
}

int QListViewItemDelegate::GetDragRow() const
{
    return m_nDragIndex;
}

void QListViewItemDelegate::SetSelectRow(int nRow)
{
    m_nSelectedRow = nRow;
}

int QListViewItemDelegate::GetSelectRow() const
{
    return m_nSelectedRow;
}

void QListViewItemDelegate::SetHighLiteRow(int nRow)
{
    m_nOldHighlightedRow = m_nHighlightedRow;
    m_nHighlightedRow = nRow;
}

int QListViewItemDelegate::GetHighLiteRow() const
{
    return m_nHighlightedRow;
}

int QListViewItemDelegate::GetOldHighLiteRow() const
{
    return m_nOldHighlightedRow;
}

void QListViewItemDelegate::SetDraging(bool bVal)
{
    m_bIsDraging = bVal;
}

bool QListViewItemDelegate::IsDraging() const
{
    return m_bIsDraging;
}

QSize QListViewItemDelegate::GetItemSize() const
{
    return m_sizeItem;
}

void QListViewItemDelegate::SetItemSize(const QSize& size)
{
    m_sizeItem = size;
}

void QListViewItemDelegate::UpdateItemSize(const int nNewItemCount)
{
    if (nNewItemCount > 20)
    {
        SetItemSize(QSize(100, 20));
    }
    else if (nNewItemCount < 8)
    {
        SetItemSize(QSize(100, 50));
    }
    else
    {
        int nHeight = 30 / (nNewItemCount - 8);
        SetItemSize(QSize(100, nHeight + 20));
    }
}

QString QListViewItemDelegate::GetMimeDataType() const
{
    return QStringLiteral("QListViewItemDelegate/drag-icon-title-btntext");
}

void QListViewItemDelegate::SetHoverIndex(const QModelIndex& index)
{
    m_hoveredIndex = index;
}

QModelIndex QListViewItemDelegate::GetHoverIndex() const
{
    return m_hoveredIndex;
}

bool QListViewItemDelegate::IsHoverIndex(const QModelIndex& index) const
{
    bool bRet = m_hoveredIndex.isValid() && m_hoveredIndex == index;
    return bRet;
}

void QListViewItemDelegate::SetPressIndex(const QModelIndex& index)
{
    m_pressedIndex = index;
}

QModelIndex QListViewItemDelegate::GetPressIndex() const
{
    return m_pressedIndex;
}

bool QListViewItemDelegate::IsPressIndex(const QModelIndex& index) const
{
    bool bRet = m_pressedIndex.isValid() && m_pressedIndex == index;
    return bRet;
}

void QListViewItemDelegate::SetEventOnBtn(const bool bVal)
{
    m_bIsEventOnBtn = bVal;
}

bool QListViewItemDelegate::IsEventOnBtn() const
{
    return m_bIsEventOnBtn;
}

bool QListViewItemDelegate::IsDragStartNull() const
{
    return m_dragStartPos.isNull();
}

void QListViewItemDelegate::SetClickedIndex(const QModelIndex& index)
{
    m_clickedIndex = index;
}

QModelIndex QListViewItemDelegate::GetClickedIndex() const
{
    return m_clickedIndex;
}

bool QListViewItemDelegate::IsClickedIndex(const QModelIndex& index) const
{
    bool bRet = m_clickedIndex.isValid() && m_clickedIndex == index;
    return bRet;
}

void QListViewItemDelegate::onButtonClickedSlot(const QModelIndex& index)
{
    qDebug() << "btn : " << index << " is clicked";
}

void QListViewItemDelegate::onItemInsertedSlot(const QModelIndex& parent, int first, int last)
{
    
}

void QListViewItemDelegate::onItemRemovedSlot(const QModelIndex& parent, int first, int last)
{

}

void QListViewItemDelegate::onItemCountChanged(int nNewItemCount)
{
    UpdateItemSize(nNewItemCount);
}