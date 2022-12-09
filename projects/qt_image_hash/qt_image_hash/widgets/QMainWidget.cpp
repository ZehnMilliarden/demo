#include "QMainWidget.h"

#include <Windows.h>
#include <QScreen>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

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
    setFixedSize(QSize(500,380));

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
    connect(m_pLeftWidget, &QLeftWidget::idClicked, m_pRightWidget, &QRightWidget::btnClicked);

    QFunctionList qFunctionList;
    m_pRightWidget->GetWidgetsId(qFunctionList);

    for (auto it : qFunctionList)
    {
        m_pLeftWidget->AddButton(it.first, it.second);
    }
}

QWidget* QMainWidget::AddLeftLayout(QLayout* pLayout)
{
    if (!pLayout)
    {
        return nullptr;
    }

    if (m_pLeftWidget)
    {
        return static_cast<QWidget*>(m_pLeftWidget);
    }

    m_pLeftWidget = new QLeftWidget;
    layout()->addWidget(m_pLeftWidget);

    return static_cast<QWidget*>(m_pLeftWidget);
}

QWidget* QMainWidget::AddRightLayout(QLayout* pLayout)
{
    if (!pLayout)
    {
        return nullptr;
    }

    if (!m_pRightWidget)
    {
        static_cast<QWidget*>(m_pRightWidget);
    }

    m_pRightWidget = new QRightWidget;
    layout()->addWidget(m_pRightWidget);

    return static_cast<QWidget*>(m_pRightWidget);
}
