#include "QMainRightWidget.h"

#include <QLabel>

#include "widgets/sub_widgets/QTreeWidgetLab.h"

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
    : StyleWidget(parent)
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
    AddSubWidget(QString::fromLocal8Bit("Ê÷ÐÎ¿Ø¼þ"), new QTreeWidgetLab(this));
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
