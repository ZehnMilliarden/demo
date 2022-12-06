#pragma once

#include <QWidget>

class QLeftWidget
    : public QWidget
{
    Q_OBJECT;
public:
    explicit QLeftWidget(QWidget* parent = nullptr);
    ~QLeftWidget();

public:
    bool AddButton(const QString& txt);

protected:
    void InitUi();
    void AddLayout();
};