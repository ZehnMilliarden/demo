#include "QRightWidget.h"

#include <QDebug>
#include <QPixmap>
#include <QFileDialog>

#define ADD_NEW_FUCTION(txt) \
    m_vctStackedWidget.push_back(std::pair<QString, int>(QString::fromLocal8Bit(txt), m_vctStackedWidget.size()));

QRightWidget::QRightWidget(QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    InitUi();
}

QRightWidget::~QRightWidget()
{

}

bool QRightWidget::GetWidgetsId(QFunctionList& vctStackedWidgetIds)
{
    if (m_vctStackedWidget.size() <= 0)
    {
        return false;
    }

    vctStackedWidgetIds = m_vctStackedWidget;
    return true;
}

void QRightWidget::btnClicked(int nId)
{
    m_pLabelText->setText(m_vctStackedWidget[nId].first);
}

void QRightWidget::btnRunClicked(bool checked)
{
    if (!m_pHashCalcute)
    {
        qDebug() << "HashCalcute is null";
    }

    switch (m_pHashCalcute->GetCurrentStep())
    {
    case QHashCalcute::None:

    {
        QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "C:/Users/CF/Pictures/Saved Pictures",
            tr("images(*.png *.jpg *.bmp)"));

        if (!fileName.isEmpty())
        {
            m_pHashCalcute->LoadFile(fileName);
            m_pBtnCalcute->setText(QString::fromLocal8Bit("縮放"));
        }
    }
        
        break;
    case QHashCalcute::Load:
        m_pHashCalcute->Scale();
        m_pBtnCalcute->setText(QString::fromLocal8Bit("灰度"));
        break;
    case QHashCalcute::ScaleSize:
        m_pHashCalcute->Gray();
        m_pBtnCalcute->setText(QString::fromLocal8Bit("二值"));
        break;
    case QHashCalcute::GrayImg:
    {
        m_pHashCalcute->Binaryzation();
        m_pBtnCalcute->setText(QString::fromLocal8Bit("清除"));
        QString strHashValue = m_pHashCalcute->GetHash();
        strHashValue.insert(32, "\n");
        m_pImageHashShow->setText(strHashValue);
    }
        break;
    case QHashCalcute::Binary:
        m_pHashCalcute->Clear();
        m_pBtnCalcute->setText(QString::fromLocal8Bit("重新加载"));
        m_pImageHashShow->setText("");
        break;
    default:
        break;
    }

    QImage img = *(m_pHashCalcute->GetPixmap().get());
    m_pImageShow->setPixmap(QPixmap::fromImage(img));
}

void QRightWidget::InitUi()
{
    AddFunctions();
    AddLayout();
    InitConnection();
}

void QRightWidget::AddFunctions()
{
    ADD_NEW_FUCTION("均值Hash");
    ADD_NEW_FUCTION("差异Hash");
    ADD_NEW_FUCTION("感知Hash");
}

void QRightWidget::AddLayout()
{
    m_pLabelText = new QLabel(this);
    m_pLabelText->setGeometry(5, 10, 100, 25);
    m_pLabelText->setText(m_vctStackedWidget[0].first);
    m_pLabelText->setObjectName("tips_algorithm_name");
    m_pLabelText->setAlignment(Qt::AlignCenter);

    m_pBtnCalcute = new QPushButton(this);
    m_pBtnCalcute->setGeometry(110,10,120,25);
    m_pBtnCalcute->setText(QString::fromLocal8Bit("加载图片"));

    m_pHashCalcute = new QHashCalcute(this);

    m_pImageShow = new QLabel(this);
    m_pImageShow->setGeometry(5, 40, 300, 300);

    m_pImageHashShow = new QLabel(this);
    m_pImageHashShow->setMargin(0);
    m_pImageHashShow->setGeometry(5, 340, 370, 35);
    m_pImageHashShow->setObjectName("tips_imagehash_value");
    m_pImageHashShow->setWordWrap(true);
    m_pImageHashShow->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void QRightWidget::InitConnection()
{
    connect(m_pBtnCalcute, &QPushButton::clicked, this, &QRightWidget::btnRunClicked);
}
