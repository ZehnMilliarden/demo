#include "QNormalWidgetLab.h"

QNormalWidgetLab::QNormalWidgetLab(QWidget* parent /*= nullptr*/)
    : qt_demo::StyleWidget(parent)
{
    setObjectName("QNormalWidgetLab");
    CreateUI();
    CreateData();
    CreateConnect();
}

QNormalWidgetLab::~QNormalWidgetLab()
{

}

void QNormalWidgetLab::CreateUI()
{
    m_pMainLayout = new QVBoxLayout(this);
    setLayout(m_pMainLayout);
    m_pMainLayout->setAlignment(Qt::AlignTop);

    m_pCommandLayout = new QHBoxLayout(this);
    m_pMainLayout->addLayout(m_pCommandLayout);
    m_pCommandLayout->setAlignment(Qt::AlignLeft);

    m_pMainBtn = new QPushButton(this);
    m_pCommandLayout->addWidget(m_pMainBtn);
    m_pMainBtn->setText(QString::fromLocal8Bit("≤‚ ‘÷¥––"));
}

void QNormalWidgetLab::CreateData()
{

}

void QNormalWidgetLab::CreateConnect()
{
    connect(m_pMainBtn, &QPushButton::clicked, this, &QNormalWidgetLab::clicked);
}

void QNormalWidgetLab::clicked(bool checked /*= false*/)
{

}
