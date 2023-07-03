#include "QMainRightWidget.h"

#include <QLabel>

#include "widgets/sub_widgets/QSubWidgetsList.h"

void QMainRightWidget::btnClicked(int nId)
{
    if (!m_pMainLayout)
    {
        return;
    }

    if (nId >= m_vctWidgets.size())
    {
        return;
    }

    m_pMainLayout->setCurrentWidget(m_vctWidgets[nId].second);

    emit onWidgetChanged(m_vctWidgets[nId].first);
}

QMainRightWidget::QMainRightWidget(QWidget* parent /*= nullptr*/)
    : qt_demo::StyleWidget(parent)
{
    setObjectName("QMainRightWidget");
    CreateUI();
    AddSubWidgets();
}

QMainRightWidget::~QMainRightWidget()
{

}

void QMainRightWidget::GetWidgetDeclare(FunctionListDeclare& dec)
{
    dec.clear();

    for (auto it : m_vctWidgets)
    {
        dec.push_back(it.first);
    }
}

void QMainRightWidget::CreateUI()
{
    m_pMainLayout = new QStackedLayout(this);
    setLayout(m_pMainLayout);
}

void QMainRightWidget::AddSubWidgets()
{
    AddSubWidget(QString::fromLocal8Bit("树形控件"), new QTreeWidgetLab(this));
    AddSubWidget(QString::fromLocal8Bit("普通控件"), new QNormalWidgetLab(this));
    AddSubWidget(QString::fromLocal8Bit("列表控件"), new QListWidgetLab(this));
    AddSubWidget(QString::fromLocal8Bit("红点控件"), new QRedDotWidgetLab(this));
    AddSubWidget(QString::fromLocal8Bit("MD语法"), new QMakeDownEditLab(this));
}

void QMainRightWidget::AddSubWidget(const QString& txt, QWidget* pWidget)
{
    if (!m_pMainLayout || !pWidget)
    {
        return;
    }

    m_vctWidgets.push_back(std::pair<QString, QWidget*>(txt, pWidget));
    m_pMainLayout->addWidget(pWidget);
}
