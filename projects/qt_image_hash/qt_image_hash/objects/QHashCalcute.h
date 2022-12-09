#pragma once

#include <QImage>
#include <QObject>
#include <memory>
#include <vector>

class QHashCalcute
    : public QObject
{
    Q_OBJECT
public:
    explicit QHashCalcute(QObject* parent = nullptr);
    ~QHashCalcute();

public:
    enum STEP
    {
        None = 0,
        Load,
        ScaleSize,
        GrayImg,
        Binary,
        Count,
    };

public:
    bool LoadFile(const QString& strImg);
    std::shared_ptr<QImage> GetPixmap();

    void Scale();
    void Gray();
    void Binaryzation();
    void Clear();
    QString GetHash();
    STEP GetCurrentStep();

private:
    int otsu();
    std::vector<int> Histogram();

private:
    std::vector<int> m_vctHash;
    QImage m_img;
    STEP m_step = STEP::None;
};
