#include "QRightWidget.h"

#include <QDebug>
#include <QHBoxLayout>

#define ADD_NEW_FUCTION(txt) \
    m_vctStackedWidget.push_back(std::pair<QString, int>(QString::fromLocal8Bit(txt), m_vctStackedWidget.size()));

QRightWidget::QRightWidget(QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    InitUi();
}

QRightWidget::~QRightWidget()
{

}

bool QRightWidget::GetWidgetsId(QFunctionList& vctStackedWidgetIds)
{
    if (m_vctStackedWidget.size() <= 0)
    {
        return false;
    }

    vctStackedWidgetIds = m_vctStackedWidget;
    return true;
}

void QRightWidget::btnClicked(int nId)
{
    m_pLabelText->setText(m_vctStackedWidget[nId].first);
}

void QRightWidget::InitUi()
{
    AddLayout();
    AddFunctions();

    m_pLabelText = new QLabel(this);
    m_pLabelText->setGeometry(5, 10, 100, 100);
    m_pLabelText->setText(m_vctStackedWidget[0].first);
    m_pLabelText->setObjectName("tips_algorithm_name");
    m_pLabelText->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void QRightWidget::AddFunctions()
{
    ADD_NEW_FUCTION("均值Hash");
    ADD_NEW_FUCTION("差异Hash");
    ADD_NEW_FUCTION("感知Hash");
}

void QRightWidget::AddLayout()
{

}
