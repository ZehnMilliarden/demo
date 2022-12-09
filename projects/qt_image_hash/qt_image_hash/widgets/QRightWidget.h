#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "objects\QHashCalcute.h"

using QFunctionList = QList< std::pair<QString, int> >;

class QRightWidget
    : public QWidget
{
    Q_OBJECT;
public:
    explicit QRightWidget(QWidget* parent = nullptr);
    ~QRightWidget();

public:
    bool GetWidgetsId(QFunctionList& vctStackedWidgetIds);

public Q_SLOTS:
    void btnClicked(int nId);

private Q_SLOTS:
    void btnRunClicked(bool checked);

protected:
    void InitUi();
    void AddFunctions();
    void AddLayout();
    void InitConnection();

private:
    QFunctionList m_vctStackedWidget;
    QLabel* m_pLabelText = nullptr;
    QPushButton* m_pBtnCalcute = nullptr;
    QHashCalcute* m_pHashCalcute = nullptr;
    QLabel* m_pImageShow = nullptr;
    QLabel* m_pImageHashShow = nullptr;
    int m_nCurrentStep = 0;
};