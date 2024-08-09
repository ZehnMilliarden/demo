#include "QFakeTabWidget.h"

#include <QLabel>

QFakeTabWidget::QFakeTabWidget(QWidget* parent)
    : QWidget(parent)
{
    RegisterMetaType();
    CreateUI();
    CreateData();
    CreateConnect();
}

QFakeTabWidget::QFakeTabWidget(QFakeTabBar* pTabBar, QWidget* parent) 
    : QWidget(parent) 
    , m_pTabBar(pTabBar)
{
    RegisterMetaType();
    CreateUI();
    CreateData();
    CreateConnect();
}

QFakeTabWidget::~QFakeTabWidget()
{
}

void QFakeTabWidget::RegisterMetaType() {
    qRegisterMetaType<QLimitePrivateSignal>("QFakeTabWidget::QLimitePrivateSignal");
}

void QFakeTabWidget::CreateUI()
{
    m_pMainLayout = new QVBoxLayout(this);
    setLayout(m_pMainLayout);
    if (!m_pTabBar)
    {
        m_pTabBar = new QFakeTabBar(this);
        m_pTabBar->setFixedHeight(35);
    }
    m_pMainLayout->addWidget(m_pTabBar);

    m_pShowWidget = new QWidget(this);
    m_pMainLayout->addWidget(m_pShowWidget);
    m_pShowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_pShowLayout = new QVBoxLayout(m_pShowWidget);
    m_pShowWidget->setLayout(m_pShowLayout);
}

void QFakeTabWidget::CreateData()
{
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

int QFakeTabWidget::GetCurrentIndex() const {
    if (m_pTabBar)
    {
        return m_pTabBar->GetCurrentIndex();
    }

    return -1;
}

QWidget* QFakeTabWidget::GetCurrentWidget() const {

    int nIndex = GetCurrentIndex();
    if (nIndex >= 0)
    {
        return m_vctWidgets[nIndex];
    }

    return nullptr;
}

void QFakeTabWidget::RemoveTab(int index) {
    if (m_pTabBar) {
        m_pTabBar->removeTab(index);
    }

    if (m_funcWidgetDestroy) {
        m_funcWidgetDestroy(index, m_vctWidgets[index]);
    }

    m_vctWidgets.erase(m_vctWidgets.begin() + index);
}

void QFakeTabWidget::currentChangedSlot(int index)
{
    if (m_vctWidgets.size() <= index)
    {
        m_vctWidgets.resize(index);
    }

    ClearMainLayout();

    QWidget* pWidget = widget(index);
    if (pWidget)
    {
        pWidget->show();
        m_pShowLayout->addWidget(pWidget);
    }
}

void QFakeTabWidget::tabCloseRequestedSlot(int index)
{
    emit tabCloseRequested(index, QLimitePrivateSignal());
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
            widget->hide();
        }
        delete item;
    }
}

QFakeTabBar* QFakeTabWidget::tabBar() const {
    return m_pTabBar;
}

int QFakeTabWidget::count() const {
    return m_pTabBar->count();
}

void QFakeTabWidget::setWidgetCreater(std::function<QWidget* (int index)> func) {
    m_funcWidgetCreater = func;
}

void QFakeTabWidget::setWidgetDestroy(std::function<bool(int index, QWidget*)> func) {
    m_funcWidgetDestroy = func;
}

QWidget* QFakeTabWidget::widget(int index) {
    if (!m_vctWidgets[index]) {
        if (m_funcWidgetCreater) {
            m_vctWidgets[index] = m_funcWidgetCreater(index);
        }
    }
    return m_vctWidgets[index];
}