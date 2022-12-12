
#include "QMainLeftWidget.h"

#include <QPushButton>

QMainLeftWidget::QMainLeftWidget(QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    InitUi();
    InitConnect();
}

QMainLeftWidget::~QMainLeftWidget()
{

}

void QMainLeftWidget::AddButton(const QString& txt, int nBtnId)
{
    QPushButton* pButton = new QPushButton(this);
    pButton->setText(txt);
    m_pMainLayout->addWidget(pButton);
    m_pButtonGroup->addButton(pButton, nBtnId);
}

void QMainLeftWidget::InitConnect()
{
    connect(
        m_pButtonGroup, 
        &QButtonGroup::idClicked,
        this, 
        &QMainLeftWidget::btnClick);
}

void QMainLeftWidget::InitUi()
{
    m_pButtonGroup = new QButtonGroup(this);
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->setAlignment(Qt::AlignTop);
    setLayout(m_pMainLayout);
}

