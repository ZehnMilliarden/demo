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
    AddSubWidget(QString::fromLocal8Bit("���οؼ�"), new QTreeWidgetLab(this));
    AddSubWidget(QString::fromLocal8Bit("��ͨ�ؼ�"), new QNormalWidgetLab(this));
    AddSubWidget(QString::fromLocal8Bit("�б�ؼ�"), new QListWidgetLab(this));
    AddSubWidget(QString::fromLocal8Bit("���ؼ�"), new QRedDotWidgetLab(this));
    AddSubWidget(QString::fromLocal8Bit("MD�﷨"), new QMakeDownEditLab(this));
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
