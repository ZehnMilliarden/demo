#include "QFakeTabBar.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QTimer>
#include <QToolTip>
#include <QCursor>
#include <QDebug>

QFakeTabBar::QFakeTabBar(QWidget* parent)
    : QWidget(parent)
{
    RegisterMetaType();
    CreateUI();
    CreateConnect();
    CreateData();
}

QFakeTabBar::~QFakeTabBar()
{
}

void QFakeTabBar::addTab(const QString& strTitle)
{
    addTab(strTitle, -1);
}

void QFakeTabBar::addTab(const QString& strTitle, int nOffset)
{
    if (m_pModel)
    {
        std::shared_ptr<QListViewModel::QListItemData> itemData
            = std::make_shared<QListViewModel::QListItemData>(
                QString::fromLocal8Bit(":/icon/res/icon/test.ico"),
                strTitle,
                QString::fromLocal8Bit("X"));
        m_pModel->insertItem(nOffset, itemData);
    }
}

void QFakeTabBar::addTabs(std::vector<QString>& vctTitle)
{
    addTabs(vctTitle, -1);
}

void QFakeTabBar::addTabs(std::vector<QString>& vctTitle, int nOffset)
{
    std::vector<std::shared_ptr<QListViewModel::QListItemData>> vctItems;
    for (auto title : vctTitle)
    {
        std::shared_ptr<QListViewModel::QListItemData> itemData
            = std::make_shared<QListViewModel::QListItemData>(
                QString::fromLocal8Bit(":/icon/res/icon/test.ico"),
                title,
                QString::fromLocal8Bit("X"));
        vctItems.push_back(itemData);
    }
    m_pModel->insertItems(nOffset, vctItems);
}

int QFakeTabBar::GetTabCount() const
{
    return m_pModel->GetItemCount();
}

bool QFakeTabBar::SetCurrentIndex(int index)
{
    const int nTabCount = GetTabCount();
    if (nTabCount <= 0 || nTabCount <= index)
    {
        return false;
    }

    if (m_pListView)
    {
        QModelIndex modelIndex = m_pModel->index(index);
        m_pListView->setCurrentIndex(modelIndex);
        m_pListView->scrollTo(modelIndex, QAbstractItemView::EnsureVisible);
        return true;
    }

    return false;
}

int QFakeTabBar::GetCurrentIndex() const {
    return m_pListView->currentIndex().row();
}

QRect QFakeTabBar::tabRect(int index) const {
    return m_pListView->visualRect(m_pListView->model()->index(index, 0));
}

int QFakeTabBar::count() const {
    return GetTabCount();
}

QVariant QFakeTabBar::tabData(int index) const {
    return m_pModel->data(m_pModel->index(index), Qt::UserRole + 5);
}

void QFakeTabBar::setTabData(int index, QVariant qVar) {
    m_pModel->setData(m_pModel->index(index), qVar, Qt::UserRole + 5);
}

int QFakeTabBar::tabAt(const QPoint& point) const {
    if (m_pListView)
    {
        QModelIndex modelIndex = m_pListView->indexAt(point);
        if (modelIndex.isValid())
        {
            return modelIndex.row();
        }
    }
    return -1;
}

void QFakeTabBar::removeTab(int index) {
    if (m_pModel) {
        m_pModel->removeItem(index);
    }
}

QSize QFakeTabBar::tabSizeHint(int index) const {
    return QSize(100, 50);
}

void QFakeTabBar::SetBtnCloseTipText(const QString& strText)
{
    m_strBtnCloseTipText = strText;
}

void QFakeTabBar::SetTabToolTipText(int nIndex, const QString& strText)
{
    if (m_pModel)
    {
        m_pModel->setData(m_pModel->index(nIndex), strText, Qt::UserRole + 4);
    }
}

void QFakeTabBar::currentChangedSlot(const QModelIndex& current, const QModelIndex& previous)
{
    emit currentChanged(current.row(), QLimitePrivateSignal());
}

void QFakeTabBar::tabCloseRequestedSlot(const QModelIndex& index)
{
    emit tabCloseRequested(index.row(), QLimitePrivateSignal());
}

void QFakeTabBar::tabMovedSlot(const QModelIndex& from, const QModelIndex& to)
{
    emit tabMoved(from.row(), to.row(), QLimitePrivateSignal());
}

void QFakeTabBar::onToolTipTimerSlot()
{
    const bool bEventOnBtn = m_pDelegate->IsEventOnBtn();
    const QModelIndex indexHover = m_pDelegate->GetHoverIndex();
    if (indexHover != m_preHoverIndex || bEventOnBtn != m_bPreEventOnBtn)
    {
        if (bEventOnBtn)
        {
            if (!m_strBtnCloseTipText.isEmpty())
            {
                QToolTip::showText(QCursor::pos(), m_strBtnCloseTipText, this);
            }
        }
        else
        {
            QString strToolTip = m_pModel->data(indexHover, Qt::UserRole + 4).toString();
            if (!strToolTip.isEmpty())
            {
                QToolTip::showText(QCursor::pos(), strToolTip, this);
            }
        }
    }

    m_preHoverIndex = indexHover;
    m_bPreEventOnBtn = bEventOnBtn;
}

void QFakeTabBar::onHoverEnterSlot()
{
    if (m_pToolTipTimer && !m_pToolTipTimer->isActive())
    {
        m_pToolTipTimer->start(500);
    }
}

void QFakeTabBar::onHoverLeaveSlot()
{
    if (m_pToolTipTimer && m_pToolTipTimer->isActive())
    {
        m_pToolTipTimer->stop();
    }
}

void QFakeTabBar::onHoverEnterItemSlot(const QModelIndex& index)
{

}

void QFakeTabBar::onHoverLeaveItemSlot(const QModelIndex& index)
{
    QToolTip::hideText();
    m_preHoverIndex = QModelIndex();
    m_bPreEventOnBtn = false;
}

void QFakeTabBar::RegisterMetaType()
{
    qRegisterMetaType<QLimitePrivateSignal>("QFakeTabBar::QLimitePrivateSignal");
}

void QFakeTabBar::CreateUI()
{
    m_pTabBarListLayout = new  QHBoxLayout(this);
    setLayout(m_pTabBarListLayout);
    m_pTabBarListLayout->setContentsMargins(0, 0, 0, 0);

    m_pListView = new QListView(this);
    m_pTabBarListLayout->addWidget(m_pListView);
    m_pListView->setFlow(QListView::LeftToRight);
    m_pListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pListView->setAcceptDrops(true);
    m_pListView->setDragEnabled(true);
    m_pListView->setDragDropMode(QAbstractItemView::DragDrop);
    m_pListView->viewport()->setAttribute(Qt::WA_Hover);
    m_pListView->viewport()->setAttribute(Qt::WA_MouseTracking);
    m_pListView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pListView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_pListView->setResizeMode(QListView::Adjust);
    m_pListView->setUniformItemSizes(true);

    m_pOptWidget = new QWidget(this);
    m_pTabBarListLayout->addWidget(m_pOptWidget);

    m_pDelegate = new QListViewItemDelegate(this);
    m_pListView->setItemDelegate(m_pDelegate);
    m_pListView->viewport()->installEventFilter(m_pDelegate);
    m_pDelegate->SetSizeHintFunc([this](const QStyleOptionViewItem& option, const QModelIndex& index)->QSize {
        return tabSizeHint(index.row());
        });

    m_pModel = new QListViewModel(this);
    m_pListView->setModel(m_pModel);

    m_pToolTipTimer = new QTimer(this);
}

void QFakeTabBar::CreateData()
{

}

void QFakeTabBar::CreateConnect()
{
    //QObject::connect(m_pDelegate, &QListViewItemDelegate::onLineClicked, this, &QFakeTabBar::currentChangedSlot);
    QObject::connect(m_pListView->selectionModel(), &QItemSelectionModel::currentChanged, this, &QFakeTabBar::currentChangedSlot);
    QObject::connect(m_pDelegate, &QListViewItemDelegate::buttonClicked, this, &QFakeTabBar::tabCloseRequestedSlot);
    QObject::connect(m_pDelegate, &QListViewItemDelegate::onMoveItemTo, this, &QFakeTabBar::tabMovedSlot);
    QObject::connect(m_pToolTipTimer, &QTimer::timeout, this, &QFakeTabBar::onToolTipTimerSlot);
    QObject::connect(m_pDelegate, &QListViewItemDelegate::onHoverEnterSignal, this, &QFakeTabBar::onHoverEnterSlot, Qt::QueuedConnection);
    QObject::connect(m_pDelegate, &QListViewItemDelegate::onHoverLeaveSignal, this, &QFakeTabBar::onHoverLeaveSlot, Qt::QueuedConnection);
    QObject::connect(m_pDelegate, &QListViewItemDelegate::onHoverEnterItemSignal, this, &QFakeTabBar::onHoverEnterItemSlot, Qt::QueuedConnection);
    QObject::connect(m_pDelegate, &QListViewItemDelegate::onHoverLeaveItemSignal, this, &QFakeTabBar::onHoverLeaveItemSlot, Qt::QueuedConnection);
}