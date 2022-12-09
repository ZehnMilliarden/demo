#include "QHashCalcute.h"
#include <QDebug>

QHashCalcute::QHashCalcute(QObject* parent /*= nullptr*/)
    : QObject(parent)
{

}

QHashCalcute::~QHashCalcute()
{

}

bool QHashCalcute::LoadFile(const QString& strImg)
{
    m_step = STEP::Load;
    return m_img.load(strImg);
}

std::shared_ptr<QImage> QHashCalcute::GetPixmap()
{
    std::shared_ptr<QImage> imgTemp = std::make_shared<QImage>();
    *imgTemp = m_img;
    *imgTemp = imgTemp->scaled(QSize(300,300), Qt::IgnoreAspectRatio);
    return imgTemp;
}

void QHashCalcute::Scale()
{
    m_step = STEP::ScaleSize;
    m_img = m_img.scaled(QSize(8,8), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void QHashCalcute::Gray()
{
    m_step = STEP::GrayImg;
    m_img = m_img.convertToFormat(QImage::Format_Grayscale8);
}

void QHashCalcute::Binaryzation()
{
    m_step = STEP::Binary;
    int avg = otsu();
    m_vctHash.clear();

    for (int i = 0; i != m_img.width(); ++i)
    {
        for (int j = 0; j != m_img.height(); ++j)
        {
            int rgb = qGray(m_img.pixel(i,j));
            bool bWight = rgb > avg;
            m_vctHash.push_back(bWight);
            m_img.setPixel(i,j, bWight ? qRgb(0xff, 0xff, 0xff) : qRgb(0x00,0x00,0x00));
        }
    }
}

void QHashCalcute::Clear()
{
    m_step = STEP::None;
    m_img = QImage();
}

QString QHashCalcute::GetHash()
{
    QString strHash;
    for (auto it : m_vctHash)
    {
        strHash.append(it > 0 ? '0' : '1');
    }

    return strHash;
}

QHashCalcute::STEP QHashCalcute::GetCurrentStep()
{
    return m_step;
}

std::vector<int> 
QHashCalcute::Histogram()
{
    unsigned char* graydata = m_img.bits();
    std::vector<int> hist(256);    // 256ɫ
    for (int i = 0; i != m_img.width(); ++i)
    {
        for (int j = 0; j != m_img.height(); ++j)
        {
            int index = int(*graydata);
            hist[index] += 1;
            graydata += 1;    // step
        }
    }
    graydata = NULL;
    return hist;
}

int QHashCalcute::otsu()
{
    std::vector<int> histogram = Histogram();
    int total = 0;
    for (int i = 0; i != histogram.size(); ++i)
    {
        total += histogram[i];
    }

    double sum = 0.0;
    for (unsigned int i = 1; i < histogram.size(); ++i)
        sum += i * histogram[i];
    double sumB = 0.0;
    double wB = 0.0;
    double wF = 0.0;
    double mB = 0.0;
    double mF = 0.0;
    double max = 0.0;
    double between = 0.0;
    double threshold = 0.0;
    for (int i = 0; i != 256; ++i)
    {
        wB += histogram[i];
        if (wB == 0)
            continue;
        wF = total - wB;
        if (wF == 0)
            break;
        sumB += i * histogram[i];
        mB = sumB / wB;
        mF = (sum - sumB) / wF;
        between = wB * wF * (mB - mF) * (mB - mF);
        if (between > max)
        {
            threshold = i;
            max = between;
        }
    }
    return threshold;
}

