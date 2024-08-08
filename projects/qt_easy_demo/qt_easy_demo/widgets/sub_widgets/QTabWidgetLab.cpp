
#include "QTabWidgetLab.h"

#include <QTimer>
#include <QEventLoop>
#include <QLabel>

namespace QTabWidgetLabSpace
{
    QTabWidgetLab::QTabWidgetLab(QWidget* parent)
        : QWidget(parent)
    {
        setObjectName("QTabWidgetLab");
        CreateUI();
        CreateData();
        CreateConnect();
    }

    QTabWidgetLab::~QTabWidgetLab()
    {
    }

    void QTabWidgetLab::CreateUI()
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        m_pMainLayout = new QVBoxLayout(this);
        m_pMainLayout->setContentsMargins(0, 0, 0, 0);
        m_pMainLayout->setSpacing(10);
        setLayout(m_pMainLayout);

        m_pTabWidget = new QFakeTabWidget(this);
        m_pMainLayout->addWidget(m_pTabWidget);
        m_pTabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


        QWidget* pOptWidget = new QWidget(this);
        m_pMainLayout->addWidget(pOptWidget);

        m_pOptLayout = new QHBoxLayout(pOptWidget);
        pOptWidget->setLayout(m_pOptLayout);
        m_pOptLayout->setContentsMargins(0, 0, 0, 0);
        m_pOptLayout->setSpacing(10);

        m_pMainBtn = new QPushButton(this);
        m_pMainBtn->setText(QString::fromLocal8Bit("ÐÂ½¨±êÇ©"));
        m_pOptLayout->addWidget(m_pMainBtn);
        m_pOptLayout->addStretch(1);
    }

    void QTabWidgetLab::CreateData()
    {
        
    }

    void QTabWidgetLab::CreateConnect()
    {
        QObject::connect(m_pMainBtn, &QPushButton::clicked, this, &QTabWidgetLab::clicked);
    }

    void QTabWidgetLab::clicked(bool checked /*= false*/)
    {
        AddNewTab();
    }

    void QTabWidgetLab::AddNewTab()
    {
        std::vector<QString> vctTitle;
        for (int i = 0; i < 10000; ++i)
        {
            vctTitle.push_back(QString::fromLocal8Bit("%1").arg(i));
        }
        m_pTabWidget->AddTabs(vctTitle);
    }
}