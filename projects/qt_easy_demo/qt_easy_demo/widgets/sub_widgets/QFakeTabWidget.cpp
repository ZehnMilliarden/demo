#include "QFakeTabWidget.h"

#include <QLabel>

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

    m_pShowLayout = new QVBoxLayout(m_pShowWidget);
    m_pShowWidget->setLayout(m_pShowLayout);
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
    SetCurrentIndex(m_pTabBar->GetTabCount() - 1);
}

void QFakeTabWidget::AddTabs(const std::vector<QString>& vctTitle)
{
    for (auto title : vctTitle)
    {
        m_pTabBar->addTab(title);
    }
    SetCurrentIndex(m_pTabBar->GetTabCount() - 1);
}

void QFakeTabWidget::SetCurrentIndex(int index)
{
    if (m_pTabBar)
    {
        m_pTabBar->SetCurrentIndex(index);
    }
}

void QFakeTabWidget::currentChangedSlot(int index)
{
    if (m_vctWidgets.size() <= index)
    {
        m_vctWidgets.resize(index);
    }

    std::vector<QWidget*>::iterator iter = m_vctWidgets.begin();
    std::advance(iter, index);

    ClearMainLayout();

    QWidget*& pTargetWidget = *iter;
    if (!pTargetWidget)
    {
        
        pTargetWidget = new QWidget(this);
        QVBoxLayout* pLayout = new QVBoxLayout(pTargetWidget);
        pTargetWidget->setLayout(pLayout);
        QLabel* pName = new QLabel(pTargetWidget);
        pName->setText(QString::fromLocal8Bit("%1").arg(index));
    }

    pTargetWidget->show();
    m_pShowLayout->addWidget(pTargetWidget);
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

void QFakeTabWidget::ClearMainLayout()
{
    while (QLayoutItem* item = m_pShowLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->hide(); // 隐藏小部件
            //widget->deleteLater();
        }
        delete item; // 删除布局项
    }
}
