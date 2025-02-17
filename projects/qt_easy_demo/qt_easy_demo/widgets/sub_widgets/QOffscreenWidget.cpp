#include "QOffscreenWidget.h"

#include <QScrollArea>
#include <QDebug>

QOffscreenWidget::QOffscreenWidget(QWidget* parent)
    : QWidget(parent, Qt::Window | Qt::FramelessWindowHint)
{
    setAttribute(Qt::WA_DontShowOnScreen, true);
    // 仅作为示例，添加一些子QWidget
    m_pLayout = new QVBoxLayout(this);
    setLayout(m_pLayout);

    m_pLayout->addWidget(new QPushButton("Button 1", this));
    m_pLayout->addWidget(new QPushButton("Button 2", this));
    createNestedLayouts(m_pLayout, 50);
    renderOffScreen();
    show();
}

QOffscreenWidget::~QOffscreenWidget()
{
}

void QOffscreenWidget::showWindow(bool bShow)
{
    m_start = std::chrono::high_resolution_clock::now();
    m_bCacl = true;

    if (bShow)
    {
        show();
    }
    else
    {
        hide();
    }
}

bool QOffscreenWidget::isShowWindow() const
{
    return !testAttribute(Qt::WA_DontShowOnScreen);
}

void QOffscreenWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    if (m_renderedContent)
    {
        painter.drawPixmap(0, 0, *m_renderedContent);
    }

    if (!isShowWindow())
    {
        setAttribute(Qt::WA_DontShowOnScreen, false);
        hide();
    }
}

void QOffscreenWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
    if (m_bCacl)
    {
        qDebug() << duration << " milliseconds.\n";
        m_bCacl = false;
    }
}

void QOffscreenWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
}

void QOffscreenWidget::createNestedLayouts(QLayout* parentLayout, int depth)
{
    if (depth == 0) return;

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QWidget* scrollWidget = new QWidget();
    QVBoxLayout* currentLayout = new QVBoxLayout(scrollWidget);
    scrollWidget->setLayout(currentLayout);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    parentLayout->addWidget(scrollArea);

    for (int i = 0; i < 300; ++i) {
        QPushButton* button = new QPushButton(QString("Button %1").arg(i + 1));
        currentLayout->addWidget(button);
    }

    createNestedLayouts(currentLayout, depth - 1);
}

void QOffscreenWidget::renderOffScreen()
{
    m_renderedContent = new QPixmap(200, 200);
    m_renderedContent->fill(Qt::transparent);

    QPainter painter(m_renderedContent);
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 30));
    painter.drawText(m_renderedContent->rect(), Qt::AlignCenter, "Hello, World!");
}