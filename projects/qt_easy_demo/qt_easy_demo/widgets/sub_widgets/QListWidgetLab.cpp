#include "QListWidgetLab.h"
#include <QPainter>
#include <memory>
#include <QScrollBar>

QListWidgetLab::QListWidgetLab(QWidget* parent /*= nullptr*/)
    : qt_demo::StyleWidget(parent)
{
    QWidget::setObjectName("QListWidgetLab");
    CreateUI();
    CreateData();
    CreateConnect();
}

QListWidgetLab::~QListWidgetLab()
{

}

void QListWidgetLab::CreateUI()
{
    m_pMainLayout = new QVBoxLayout(this);
    setLayout(m_pMainLayout);
    m_pMainLayout->setAlignment(Qt::AlignTop);

    m_pCommandLayout = new QHBoxLayout(this);
    m_pMainLayout->addLayout(m_pCommandLayout);
    m_pCommandLayout->setAlignment(Qt::AlignLeft);

    m_pMainBtn = new QPushButton(this);
    m_pCommandLayout->addWidget(m_pMainBtn);
    m_pMainBtn->setText(QString::fromLocal8Bit("获取列表"));

    m_pScrollToTop = new QPushButton(this);
    m_pCommandLayout->addWidget(m_pScrollToTop);
    m_pScrollToTop->setText(QString::fromLocal8Bit("滚动到顶部"));

    m_pScrollToButtom = new QPushButton(this);
    m_pCommandLayout->addWidget(m_pScrollToButtom);
    m_pScrollToButtom->setText(QString::fromLocal8Bit("滚动到底部"));

    m_pListView = new QListView(this);
    m_pMainLayout->addWidget(m_pListView);

    updateButtonVisibility();
}

void QListWidgetLab::CreateData()
{
    m_pDelegate = new QListViewItemDelegate(this);
    m_pListView->setItemDelegate(m_pDelegate);
    m_pListView->viewport()->setAttribute(Qt::WA_Hover);
    m_pListView->viewport()->setAttribute(Qt::WA_MouseTracking);
    m_pListView->viewport()->installEventFilter(m_pDelegate);
    m_pModel = new QListViewModel(this);
    m_pListView->setModel(m_pModel);
    m_pListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pListView->setAcceptDrops(true);
    m_pListView->setDragEnabled(true);
    m_pListView->setDragDropMode(QAbstractItemView::DragDrop);

    addItemImpl(10);
}

void QListWidgetLab::CreateConnect()
{
    QObject::connect(m_pMainBtn, &QPushButton::clicked, this, &QListWidgetLab::btnClicked);
    QObject::connect(m_pScrollToTop, &QPushButton::clicked, this, &QListWidgetLab::btnClicked);
    QObject::connect(m_pScrollToButtom, &QPushButton::clicked, this, &QListWidgetLab::btnClicked);
    QObject::connect(m_pModel, &QListViewModel::rowsInserted, this, &QListWidgetLab::updateButtonVisibility);
    QObject::connect(m_pModel, &QListViewModel::rowsRemoved, this, &QListWidgetLab::updateButtonVisibility);
    QObject::connect(m_pListView->verticalScrollBar(), &QScrollBar::rangeChanged, this, &QListWidgetLab::updateButtonVisibility);
}

void QListWidgetLab::updateButtonVisibility()
{
    int nMax = m_pListView->verticalScrollBar()->maximum();
    int nMin = m_pListView->verticalScrollBar()->minimum();
    bool needScrollButtons = nMax > nMin;
    m_pScrollToTop->setVisible(needScrollButtons);
    m_pScrollToButtom->setVisible(needScrollButtons);
}

void QListWidgetLab::addItemImpl(int nCount)
{
    int nCurrentCount = m_pModel->GetItemCount();
    for (int i = 0; i < nCount; ++i)
    {
        std::shared_ptr<QListViewModel::QListItemData> itemData
            = std::make_shared<QListViewModel::QListItemData>(
                QString::fromLocal8Bit(":/icon/res/icon/test.ico"),
                QString::fromLocal8Bit("测试文案 %1").arg(i + nCurrentCount),
                QString::fromLocal8Bit("测试按钮 %1").arg(i + nCurrentCount));
        m_pModel->addItem(itemData);
    }
}

void QListWidgetLab::btnClicked(bool checked /*= false*/)
{
    const QObject* pSender = sender();
    if (pSender == m_pMainBtn)
    {
        addItemImpl(10);
    }
    else if (pSender == m_pScrollToTop)
    {
        m_pListView->verticalScrollBar()->setValue(m_pListView->verticalScrollBar()->value() - 1);
    }
    else if (pSender == m_pScrollToButtom)
    {
        m_pListView->verticalScrollBar()->setValue(m_pListView->verticalScrollBar()->value() + 1);
    }
    
}
