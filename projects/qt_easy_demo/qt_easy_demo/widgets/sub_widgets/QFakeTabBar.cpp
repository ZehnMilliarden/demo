#include "QFakeTabBar.h"

#include <QHBoxLayout>

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
    addTabs(vctTitle);
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

int QFakeTabBar::GetTabCount()
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

void QFakeTabBar::RegisterMetaType()
{
    qRegisterMetaType<QLimitePrivateSignal>("QLimitePrivateSignal");
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

    m_pModel = new QListViewModel(this);
    m_pListView->setModel(m_pModel);
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
}