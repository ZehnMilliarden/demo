#pragma once

#include <QWidget>
#include <QLabel>

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

protected:
    void InitUi();
    void AddFunctions();
    void AddLayout();

private:
    QFunctionList m_vctStackedWidget;
    QLabel* m_pLabelText = nullptr;
};