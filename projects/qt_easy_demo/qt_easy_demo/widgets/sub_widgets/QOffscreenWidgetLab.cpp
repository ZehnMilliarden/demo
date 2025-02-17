#include "QOffscreenWidgetLab.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>


QOffscreenWidgetLab::QOffscreenWidgetLab(QWidget* parent /*= nullptr*/)
    : qt_demo::StyleWidget(parent)
{
    setObjectName("QOffscreenWidgetLab");
    CreateUI();
    CreateData();
    CreateConnect();
}

QOffscreenWidgetLab::~QOffscreenWidgetLab()
{

}

void QOffscreenWidgetLab::CreateUI()
{
    m_pMainLayout = new QVBoxLayout(this);
    setLayout(m_pMainLayout);
    m_pMainLayout->setAlignment(Qt::AlignTop);

    m_pCommandLayout = new QHBoxLayout(this);
    m_pMainLayout->addLayout(m_pCommandLayout);
    m_pCommandLayout->setAlignment(Qt::AlignLeft);

    m_pMainBtn = new QPushButton(this);
    m_pCommandLayout->addWidget(m_pMainBtn);
    m_pMainBtn->setText(QString::fromLocal8Bit("²âÊÔÖ´ÐÐ"));

    m_pOffscreenWidget = new QOffscreenWidget();
    m_pOffscreenWidget->setFixedSize(400, 450);
}

void QOffscreenWidgetLab::CreateData()
{

}

void QOffscreenWidgetLab::CreateConnect()
{
    connect(m_pMainBtn, &QPushButton::clicked, this, &QOffscreenWidgetLab::clicked);
}

void QOffscreenWidgetLab::clicked(bool checked /*= false*/)
{
    if (m_pOffscreenWidget->isHidden() || !m_pOffscreenWidget->isShowWindow())
    {
        //m_pOffscreenWidget->setParent(this);
        //m_pMainLayout->addWidget(m_pOffscreenWidget);
        m_pOffscreenWidget->showWindow(true);
    }
    else
    {
        m_pOffscreenWidget->showWindow(false);
    }
}
