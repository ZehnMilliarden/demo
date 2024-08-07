
#include "QTabWidgetLab.h"

#include <QTimer>
#include <QEventLoop>
#include <QLabel>

namespace QTabWidgetLabSpace
{

    QTabWidgetLab::QTabWidgetLab(QWidget* parent)
        : QWidget(parent)
    {
        setObjectName("QTabWidgetLab");
        CreateUI();
        CreateData();
        CreateConnect();
    }

    QTabWidgetLab::~QTabWidgetLab()
    {
    }

    void QTabWidgetLab::CreateUI()
    {
        m_pMainLayout = new QVBoxLayout(this);
        m_pMainLayout->setContentsMargins(0, 0, 0, 0);
        m_pMainLayout->setSpacing(10);
        setLayout(m_pMainLayout);

        m_pTabWidget = new QFakeTabWidget(this);
        m_pMainLayout->addWidget(m_pTabWidget);
        m_pTabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QWidget* pOptWidget = new QWidget(this);
        m_pMainLayout->addWidget(pOptWidget);

        m_pOptLayout = new QHBoxLayout(pOptWidget);
        pOptWidget->setLayout(m_pOptLayout);
        m_pOptLayout->setContentsMargins(0, 0, 0, 0);
        m_pOptLayout->setSpacing(10);

        m_pMainBtn = new QPushButton(this);
        m_pMainBtn->setText(QString::fromLocal8Bit("新建标签"));
        m_pOptLayout->addWidget(m_pMainBtn);
        m_pOptLayout->addStretch(1);
    }

    void QTabWidgetLab::CreateData()
    {

    }

    void QTabWidgetLab::CreateConnect()
    {
        QObject::connect(m_pMainBtn, &QPushButton::clicked, this, &QTabWidgetLab::clicked);
    }

    void QTabWidgetLab::clicked(bool checked /*= false*/)
    {
        AddNewTab();
    }

    void QTabWidgetLab::AddNewTab()
    {
        for (int i = 0; i < 10000; ++i)
        {
            m_pTabWidget->AddTab(QString::fromLocal8Bit("标签 %1").arg(i));
        }
    }


    QFakeTabWidget::QFakeTabWidget(QWidget* parent)
        : QWidget(parent)
    {
        CreateUI();
        CreateData();
        CreateConnect();
    }

    QFakeTabWidget::~QFakeTabWidget()
    {
    }

    void QFakeTabWidget::CreateUI()
    {
        m_pMainLayout = new QVBoxLayout(this);
        setLayout(m_pMainLayout);
        m_pTabBar = new QFakeTabBar(this);
        m_pTabBar->setFixedHeight(35);
        m_pMainLayout->addWidget(m_pTabBar);

        m_pShowWidget = new QWidget(this);
        m_pMainLayout->addWidget(m_pShowWidget);
        m_pShowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void QFakeTabWidget::CreateData()
    {
        m_vctWidgets.resize(100000);
    }

    void QFakeTabWidget::CreateConnect()
    {
        connect(m_pTabBar, &QFakeTabBar::currentChanged, this, &QFakeTabWidget::currentChangedSlot);
        connect(m_pTabBar, &QFakeTabBar::tabCloseRequested, this, &QFakeTabWidget::tabCloseRequestedSlot);
        connect(m_pTabBar, &QFakeTabBar::tabMoved, this, &QFakeTabWidget::tabMovedSlot);
    }

    void QFakeTabWidget::AddTab(const QString& strTitle)
    {
        m_pTabBar->addTab(strTitle);
    }

    void QFakeTabWidget::currentChangedSlot(int index)
    {
        if (m_vctWidgets.size() <= index)
        {
            m_vctWidgets.resize(index);
        }

        std::vector<QWidget*>::iterator iter = m_vctWidgets.begin();
        std::advance(iter, index);

        QWidget*& pTargetWidget = *iter;
        if (!pTargetWidget)
        {
            pTargetWidget = new QWidget(this);
            QVBoxLayout* pLayout = new QVBoxLayout(pTargetWidget);
            pTargetWidget->setLayout(pLayout);
            QLabel* pName = new QLabel(pTargetWidget);
            pName->setText(QString::fromLocal8Bit("标签 %1").arg(index));
            pLayout->addWidget(pName);
        }

        pTargetWidget->show();
    }

    void QFakeTabWidget::tabCloseRequestedSlot(int index)
    {
        std::vector<QWidget*>::iterator iter = m_vctWidgets.begin();
        std::advance(iter, index);
        QWidget*& pTargetWidget = *iter;
        pTargetWidget->deleteLater();
        pTargetWidget = NULL;
        m_vctWidgets.erase(iter);
    }

    void QFakeTabWidget::tabMovedSlot(int from, int to)
    {
        auto it1 = m_vctWidgets.begin();
        std::advance(it1, from);

        auto it2 = m_vctWidgets.begin();
        std::advance(it2, to);

        std::iter_swap(it1, it2);
    }

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
        if (m_pModel)
        {
            std::shared_ptr<QListViewModel::QListItemData> itemData
                = std::make_shared<QListViewModel::QListItemData>(
                    QString::fromLocal8Bit(":/icon/res/icon/test.ico"),
                    strTitle,
                    QString::fromLocal8Bit("X"));
            m_pModel->addItem(itemData);
        }
    }

    void QFakeTabBar::currentChangedSlot(const QModelIndex& index)
    {
        emit currentChanged(index.row(), QLimitePrivateSignal());
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
        QObject::connect(m_pDelegate, &QListViewItemDelegate::onLineClicked, this, &QFakeTabBar::currentChangedSlot);
        QObject::connect(m_pDelegate, &QListViewItemDelegate::buttonClicked, this, &QFakeTabBar::tabCloseRequestedSlot);
        QObject::connect(m_pDelegate, &QListViewItemDelegate::onMoveItemTo, this, &QFakeTabBar::tabMovedSlot);
    }
}