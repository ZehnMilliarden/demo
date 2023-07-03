#pragma once

#include "QTextEdit"

#include "public/qt_demo/StyleWidget.h"

class QMakeDownEditLab
    : public qt_demo::StyleWidget
{
    Q_OBJECT;
    Q_DISABLE_COPY(QMakeDownEditLab);

public:
    explicit QMakeDownEditLab(QWidget* parent = nullptr);
    ~QMakeDownEditLab();

protected:
    void CreateUI();
    void CreateData();
    void CreateConnect();

    void AppendMdText(const QString& strTxt);

private:
    QTextEdit* m_pEditText = nullptr;
};