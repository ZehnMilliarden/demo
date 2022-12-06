#include "QMainWidget.h"

#include <Windows.h>
#include <QScreen>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "QLeftWidget.h"
#include "QRightWidget.h"

QMainWidget::QMainWidget(QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    InitUi();
}

QMainWidget::~QMainWidget()
{

}

void QMainWidget::InitUi()
{
    setObjectName("QMainWidget");
    setWindowTitle("ImageHash");
    setFixedSize(QSize(500,350));

    MoveToDesktopCenter();
    AddLayout();
}

void QMainWidget::MoveToDesktopCenter()
{
    int nWidth = QGuiApplication::screenAt(QPoint(0, 0))->geometry().width();
    int nHeight = QGuiApplication::screenAt(QPoint(0, 0))->geometry().height();

    int nXPos = (nWidth - width()) / 2;
    int nYPos = (nHeight - height()) / 2;
    ::MoveWindow((HWND)winId(), nXPos, nYPos, width(), height(), FALSE);
    setGeometry(nXPos, nYPos, width(), height());
}

void QMainWidget::AddLayout()
{
    QHBoxLayout* pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setContentsMargins(0, 0, 0, 0);
    pHBoxLayout->setSpacing(0);
    setLayout(pHBoxLayout);

    AddLeftLayout(static_cast<QLayout*>(pHBoxLayout));
    AddRightLayout(static_cast<QLayout*>(pHBoxLayout));
}

void QMainWidget::AddLeftLayout(QLayout* pLayout)
{
    if (!pLayout)
    {
        return;
    }

    QLeftWidget* pLeftWidget = new QLeftWidget;
    pLayout->addWidget(pLeftWidget);

    pLeftWidget->AddButton(QString::fromLocal8Bit("均值Hash"));
    pLeftWidget->AddButton(QString::fromLocal8Bit("差异Hash"));
    pLeftWidget->AddButton(QString::fromLocal8Bit("感知Hash"));
}

void QMainWidget::AddRightLayout(QLayout* pLayout)
{
    if (!pLayout)
    {
        return;
    }

    QRightWidget* pRightWidget = new QRightWidget;
    pLayout->addWidget(pRightWidget);
}
