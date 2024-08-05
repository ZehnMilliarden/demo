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

namespace QTabWidgetLabSpace
{
    class QFakeTabBar : public QWidget
    {
        Q_OBJECT;
        Q_DISABLE_COPY(QFakeTabBar);

    public:
        explicit QFakeTabBar(QWidget* parent = nullptr);
        virtual ~QFakeTabBar();

    Q_SIGNALS:
        void currentChanged(int nIndex);
        void tabCloseRequested(int nIndex);
        void tabMoved(int from, int to);

    protected:
        void CreateUI();
        void CreateData();
        void CreateConnect();

    public:
        void addTab(const QString& strTitle);

    private:
        QListView* m_pListView = nullptr;
    };

    class QFakeTabWidget : public QWidget
    {
        Q_OBJECT;
        Q_DISABLE_COPY(QFakeTabWidget);

    public:
        explicit QFakeTabWidget(QWidget* parent = nullptr);
        virtual ~QFakeTabWidget();

    protected:
        void CreateUI();
        void CreateData();
        void CreateConnect();

    public:
        void AddTab(const QString& strTitle);

    protected Q_SLOTS:
        void currentChangedSlot(int index);
        void tabCloseRequestedSlot(int index);
        void tabMovedSlot(int from, int to);

    private:
        QFakeTabBar* m_pTabBar = nullptr;
        std::vector<QWidget*> m_vctWidgets;
        QWidget* m_pShowWidget = nullptr;
    };

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
