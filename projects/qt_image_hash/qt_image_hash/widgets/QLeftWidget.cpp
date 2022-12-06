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

bool QLeftWidget::AddButton(const QString& txt)
{
    QPushButton* pPushButton = new QPushButton();
    pPushButton->setText(txt);
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
}
