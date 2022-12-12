#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include "widgets/main_widgets/QMainLeftWidget.h"
#include "widgets/main_widgets/QMainRightWidget.h"

class QMainWidget
    : public QWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QMainWidget);

public Q_SLOTS:
    void OnTitleChange(const QString& title);

public:
    explicit QMainWidget(QWidget* parent = nullptr);
    ~QMainWidget();

protected:
    void InitUI();
    void CreateUI();
    void InitConnect();
    void InitFunctionWidget();
    void MoveToDesktopCenter();

private:
    QHBoxLayout* m_pMainLayout = nullptr;
    QMainLeftWidget* m_pMainLeftWidget = nullptr;
    QMainRightWidget* m_pMainRightWidget = nullptr;
};