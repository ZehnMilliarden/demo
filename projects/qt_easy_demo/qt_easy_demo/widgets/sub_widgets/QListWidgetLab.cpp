#include "QListWidgetLab.h"

QListWidgetLab::QListWidgetLab(QWidget* parent /*= nullptr*/)
    : qt_demo::StyleWidget(parent)
{
    setObjectName("QListWidgetLab");
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

    m_pListView = new QListView(this);
    m_pMainLayout->addWidget(m_pListView);
}

void QListWidgetLab::CreateData()
{

}

void QListWidgetLab::CreateConnect()
{
    connect(m_pMainBtn, &QPushButton::clicked, this, &QListWidgetLab::clicked);
}

void QListWidgetLab::clicked(bool checked /*= false*/)
{

}
