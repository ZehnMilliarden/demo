#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QButtonGroup>

class QMainLeftWidget
    : public QWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QMainLeftWidget);

Q_SIGNALS:
    void btnClick(int);

public:
    explicit QMainLeftWidget(QWidget* parent = nullptr);
    ~QMainLeftWidget();

public:
    void AddButton(const QString& txt, int nBtnId);

protected:
    void InitConnect();
    void InitUi();

private:
    QButtonGroup* m_pButtonGroup = nullptr;
    QVBoxLayout* m_pMainLayout = nullptr;
};