#include "QMakeDownEditLab.h"

QMakeDownEditLab::QMakeDownEditLab(QWidget* parent /*= nullptr*/)
    : qt_demo::StyleWidget(parent)
{
    setObjectName("QMakeDownEditLab");
    CreateUI();
    CreateData();
    CreateConnect();
}

QMakeDownEditLab::~QMakeDownEditLab()
{

}

void QMakeDownEditLab::CreateUI()
{
    m_pEditText = new QTextEdit(this);
    m_pEditText->setReadOnly(true);
    m_pEditText->setMinimumWidth(500);
}

void QMakeDownEditLab::CreateData()
{
    AppendMdText("# Markdown Title\n\n");
    AppendMdText("This is a **bold** text.\n");
    AppendMdText("This is an *italic* text.\n");
    AppendMdText("This is a link.\n");
    AppendMdText(QString::fromLocal8Bit("颜色名 | 十六进制颜色值 |  rgb颜色  "));
    AppendMdText(QString::fromLocal8Bit("-|-|-"));
    AppendMdText(QString::fromLocal8Bit("黑色（black) | 000000 | 	rgb(0, 0, 0) |"));
    AppendMdText(QString::fromLocal8Bit("蓝色（blue） | 0000FF | rgb(0, 0, 255) |"));
}

void QMakeDownEditLab::CreateConnect()
{

}

void QMakeDownEditLab::AppendMdText(const QString& strTxt)
{
    if (m_pEditText)
    {
        QString res = m_pEditText->toMarkdown();
        res.append(strTxt);
        m_pEditText->setMarkdown(res);
    }
}
