#include "QLeftWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

QLeftWidget::QLeftWidget(QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    InitUi();
}

QLeftWidget::~QLeftWidget()
{

}

bool QLeftWidget::AddButton(const QString& txt, const int nButtonId)
{
    if (!m_pButtonGroup)
    {
        return false;
    }

    QPushButton* pPushButton = new QPushButton();
    pPushButton->setText(txt);
    m_pButtonGroup->addButton(pPushButton, nButtonId);
    layout()->addWidget(pPushButton);
    return true;
}

void QLeftWidget::InitUi()
{
    setObjectName("QLeftWidget");
    setFixedSize(QSize(100, 350));
    AddLayout();
}

void QLeftWidget::AddLayout()
{
    QVBoxLayout* pVSubLeftBoxLayout = new QVBoxLayout;
    pVSubLeftBoxLayout->setContentsMargins(0, 10, 0, 0);
    pVSubLeftBoxLayout->setSpacing(10);
    pVSubLeftBoxLayout->setAlignment(Qt::AlignTop);
    setLayout(pVSubLeftBoxLayout);

    m_pButtonGroup = new QButtonGroup;
    m_pButtonGroup->setExclusive(true);

    connect(m_pButtonGroup, &QButtonGroup::idClicked, this, &QLeftWidget::idClicked);
}
