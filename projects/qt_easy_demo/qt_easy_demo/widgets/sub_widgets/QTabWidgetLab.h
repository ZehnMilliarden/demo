#pragma once

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QThread>
#include <list>
#include <mutex>
#include <Windows.h>
#include <QVector>
#include <QTabBar>
#include <QListView>

#include "public/qt_demo/StyleWidget.h"
#include "QFakeTabWidget.h"

namespace QTabWidgetLabSpace
{
    class QTabWidgetLab : public QWidget
    {
        Q_OBJECT;
        Q_DISABLE_COPY(QTabWidgetLab);

    public:
        explicit QTabWidgetLab(QWidget* parent = nullptr);
        ~QTabWidgetLab();

    protected:
        void CreateUI();
        void CreateData();
        void CreateConnect();

    protected Q_SLOTS:
        void clicked(bool checked = false);

    public Q_SLOTS:
        void AddNewTab();

    private:
        QFakeTabWidget* m_pTabWidget = nullptr;
        QPushButton* m_pMainBtn = nullptr;
        QVBoxLayout* m_pMainLayout = nullptr;
        QHBoxLayout* m_pOptLayout = nullptr;
    };
}
