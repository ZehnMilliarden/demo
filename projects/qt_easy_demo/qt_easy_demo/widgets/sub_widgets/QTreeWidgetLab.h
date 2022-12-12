#pragma once

#include <QWidget>

class QTreeWidgetLab
    : public QWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QTreeWidgetLab);

public:
    explicit QTreeWidgetLab(QWidget* parent = nullptr);
    ~QTreeWidgetLab();

protected:
    void CreateUI();

private:

};