#pragma once

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <chrono>
#include <iostream>

class QOffscreenWidget
    : public QWidget
{
public:
    explicit QOffscreenWidget(QWidget* parent = nullptr);
    ~QOffscreenWidget();

    void showWindow(bool bShow);
    bool isShowWindow() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void renderOffScreen();

private:
    void createNestedLayouts(QLayout* parentLayout, int depth);

private:
    QTimer* m_pRenderTimer = nullptr;
    QPixmap* m_renderedContent = nullptr;
    bool m_bRenderToPixmap = true;
    QVBoxLayout* m_pLayout = nullptr;
    bool m_bCacl = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};