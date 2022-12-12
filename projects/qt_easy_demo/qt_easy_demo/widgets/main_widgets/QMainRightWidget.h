#pragma once

#include <QWidget>
#include <QStackedLayout>

using FunctionList = std::vector< std::pair<QString, QWidget*> >;
using FunctionListDeclare = std::vector<QString>;

class QMainRightWidget
    : public QWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QMainRightWidget);

public Q_SLOTS:
    void btnClicked(int nId);

Q_SIGNALS:
    void onWidgetChanged(const QString& title);

public:
    explicit QMainRightWidget(QWidget* parent = nullptr);
    ~QMainRightWidget();

public:
    void GetWidgetDeclare(FunctionListDeclare& dec);

protected:
    void CreateUI();
    void AddSubWidgets();
    void AddSubWidget(const QString& txt, QWidget* pWidget);

public:
    QStackedLayout* m_pMainLayout = nullptr;
    FunctionList m_vctWidgets;
};