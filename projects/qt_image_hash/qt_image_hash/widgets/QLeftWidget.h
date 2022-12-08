#pragma once

#include <QWidget>
#include <QButtonGroup>

class QLeftWidget
    : public QWidget
{
    Q_OBJECT;
public:
    explicit QLeftWidget(QWidget* parent = nullptr);
    ~QLeftWidget();

public:
    bool AddButton(const QString& txt, const int nButtonId);

Q_SIGNALS:
    void idClicked(int);

protected:
    void InitUi();
    void AddLayout();

private:
    QButtonGroup* m_pButtonGroup = nullptr;
};