#include "QMainWidget.h"

#include <Windows.h>
#include <QScreen>
#include <QApplication>

void QMainWidget::OnTitleChange(const QString& title)
{
    setWindowTitle("qt_easy_demo - " + title);
}

QMainWidget::QMainWidget(QWidget* parent /*= nullptr*/)
    :QWidget(parent)
{
    setObjectName("qt_easy_demo_QMainWidget");
    setWindowTitle("qt_easy_demo");
    setFixedSize(QSize(700, 580));
    InitUI();
}

QMainWidget::~QMainWidget()
{

}

void QMainWidget::InitUI()
{
    CreateUI();
    InitFunctionWidget();
    InitConnect();
    MoveToDesktopCenter();
}

void QMainWidget::CreateUI()
{
    m_pMainLayout = new QHBoxLayout(this);
    setLayout(m_pMainLayout);

    m_pMainLeftWidget = new QMainLeftWidget(this);
    m_pMainRightWidget = new QMainRightWidget(this);
    m_pMainLayout->addWidget(m_pMainLeftWidget);
    m_pMainLayout->addWidget(m_pMainRightWidget);
}

void QMainWidget::InitConnect()
{
    connect(
        m_pMainLeftWidget, 
        &QMainLeftWidget::btnClick,
        m_pMainRightWidget,
        &QMainRightWidget::btnClicked
        );

    connect(
        m_pMainRightWidget, 
        &QMainRightWidget::onWidgetChanged,
        this,
        &QMainWidget::OnTitleChange);
}

void QMainWidget::InitFunctionWidget()
{
    FunctionListDeclare dec;
    m_pMainRightWidget->GetWidgetDeclare(dec);

    for (int i = 0; i < dec.size(); ++i)
    {
        m_pMainLeftWidget->AddButton(dec[i], i);
    }
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
