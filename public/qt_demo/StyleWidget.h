#ifndef KStyleBaseWidget_H__
#define KStyleBaseWidget_H__

#include <QWidget>

class StyleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StyleWidget(QWidget* parent = nullptr);

public Q_SLOTS:

Q_SIGNALS:

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    Q_DISABLE_COPY(StyleWidget)
};

#endif //! KStyleBaseWidget_H__