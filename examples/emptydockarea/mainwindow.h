#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include <QWidgetAction>

#include "DockAreaWidget.h"
#include "DockManager.h"
#include "DockWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class CMainWindow;
}
QT_END_NAMESPACE

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget* parent = nullptr);
    ~CMainWindow();

protected:
    virtual void closeEvent(QCloseEvent* event) override;

private:
    QAction* SavePerspectiveAction = nullptr;
    QWidgetAction* PerspectiveListAction = nullptr;
    QComboBox* PerspectiveComboBox = nullptr;

    Ui::CMainWindow* ui;

    ads::CDockManager* DockManager;
    ads::CDockAreaWidget* StatusDockArea;
    ads::CDockWidget* TimelineDockWidget;

    void createPerspectiveUi();

private slots:
    void savePerspective();
};
#endif  // MAINWINDOW_H
