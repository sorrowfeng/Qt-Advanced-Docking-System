#include "mainwindow.h"

#include <QCalendarWidget>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QStatusBar>
#include <QTableWidget>
#include <QToolBar>
#include <QTreeView>
#include <QWidgetAction>

#include "DockAreaTabBar.h"
#include "DockAreaTitleBar.h"
#include "DockAreaWidget.h"
#include "DockComponentsFactory.h"
#include "FloatingDockContainer.h"
#include "ui_mainwindow.h"

using namespace ads;

CMainWindow::CMainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setConfigFlag(CDockManager::PerspectivesWithOutCentralWidget,
                                true);
    CDockManager::setConfigFlag(CDockManager::FluentUIDarkStyleSheet, true);
    CDockManager::setAutoHideConfigFlags({CDockManager::DefaultAutoHideConfig});

    DockManager = new CDockManager(this);

    // Set central widget
    QPlainTextEdit* w = new QPlainTextEdit();
    w->setPlaceholderText("This is the central editor. Enter your text here.");
    CDockWidget* CentralDockWidget =
        DockManager->createDockWidget("CentralWidget");
    CentralDockWidget->setWidget(w);
    auto* CentralDockArea = DockManager->setCentralWidget(CentralDockWidget);
    CentralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);

    // create other dock widgets
    QTableWidget* table = new QTableWidget();
    table->setColumnCount(3);
    table->setRowCount(10);
    for (int row = 0; row < table->rowCount(); ++row)
        for (int col = 0; col < table->columnCount(); ++col)
            table->setItem(row, col, new QTableWidgetItem(QString("Text %1.%2").arg(row).arg(col)));

    CDockWidget* TableDockWidget = DockManager->createDockWidget("Table 1");
    TableDockWidget->setWidget(table);
    TableDockWidget->setMinimumSizeHintMode(
        CDockWidget::MinimumSizeHintFromDockWidget);
    TableDockWidget->resize(250, 150);
    TableDockWidget->setMinimumSize(200, 150);
    auto TableArea = DockManager->addDockWidget(
        DockWidgetArea::LeftDockWidgetArea, TableDockWidget);
    ui->menuView->addAction(TableDockWidget->toggleViewAction());

    table = new QTableWidget();
    table->setColumnCount(5);
    table->setRowCount(50);
    for (int row = 0; row < table->rowCount(); ++row)
        for (int col = 0; col < table->columnCount(); ++col)
            table->setItem(row, col, new QTableWidgetItem(QString("Text %1.%2").arg(row).arg(col)));

    TableDockWidget = DockManager->createDockWidget("Table 2");
    TableDockWidget->setWidget(table);
    TableDockWidget->setMinimumSizeHintMode(
        CDockWidget::MinimumSizeHintFromDockWidget);
    TableDockWidget->resize(250, 150);
    TableDockWidget->setMinimumSize(200, 150);
    DockManager->addDockWidget(DockWidgetArea::BottomDockWidgetArea,
                               TableDockWidget, TableArea);
    ui->menuView->addAction(TableDockWidget->toggleViewAction());

    table = new QTableWidget();
    table->setColumnCount(3);
    table->setRowCount(10);
    for (int row = 0; row < table->rowCount(); ++row)
        for (int col = 0; col < table->columnCount(); ++col)
            table->setItem(row, col, new QTableWidgetItem(QString("Text %1.%2").arg(row).arg(col)));

    CDockWidget* PropertiesDockWidget =
        DockManager->createDockWidget("Properties");
    PropertiesDockWidget->setWidget(table);
    PropertiesDockWidget->setMinimumSizeHintMode(
        CDockWidget::MinimumSizeHintFromDockWidget);
    PropertiesDockWidget->resize(250, 150);
    PropertiesDockWidget->setMinimumSize(200, 150);
    DockManager->addDockWidget(DockWidgetArea::RightDockWidgetArea,
                               PropertiesDockWidget, CentralDockArea);
    ui->menuView->addAction(PropertiesDockWidget->toggleViewAction());

    createPerspectiveUi();

    QStatusBar* statusBar = new QStatusBar(this);
    QLabel* statusLabel = new QLabel(this);
    statusLabel->setText(tr("NULL INFO"));
    statusBar->addWidget(statusLabel);
    this->setStatusBar(statusBar);
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::createPerspectiveUi()
{
    SavePerspectiveAction = new QAction("Create Perspective", this);
    connect(SavePerspectiveAction, SIGNAL(triggered()), SLOT(savePerspective()));
    ChangeThemeAction = new QAction("Change theme", this);
    connect(ChangeThemeAction, &QAction::triggered, this,
            &CMainWindow::changeTheme);
    PerspectiveListAction = new QWidgetAction(this);
    PerspectiveComboBox = new QComboBox(this);
    PerspectiveComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    PerspectiveComboBox->setSizePolicy(QSizePolicy::Preferred,
                                       QSizePolicy::Preferred);
    connect(PerspectiveComboBox, SIGNAL(currentTextChanged(const QString&)),
            DockManager, SLOT(openPerspective(const QString&)));
    PerspectiveListAction->setDefaultWidget(PerspectiveComboBox);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(PerspectiveListAction);
    ui->toolBar->addAction(SavePerspectiveAction);
    ui->toolBar->addAction(ChangeThemeAction);
}

void CMainWindow::changeTheme()
{
    if (CDockManager::testConfigFlag(CDockManager::FluentUILightStyleSheet))
    {
        CDockManager::setConfigFlag(CDockManager::FluentUIDarkStyleSheet, true);
        CDockManager::setConfigFlag(CDockManager::FluentUILightStyleSheet, false);
    }
    else
    {
        CDockManager::setConfigFlag(CDockManager::FluentUIDarkStyleSheet, false);
        CDockManager::setConfigFlag(CDockManager::FluentUILightStyleSheet, true);
    }
    DockManager->loadStyleSheet();
}

void CMainWindow::savePerspective()
{
    QString PerspectiveName =
        QInputDialog::getText(this, "Save Perspective", "Enter unique name:");
    if (PerspectiveName.isEmpty())
    {
        return;
    }

    DockManager->addPerspective(PerspectiveName);
    QSignalBlocker Blocker(PerspectiveComboBox);
    PerspectiveComboBox->clear();
    PerspectiveComboBox->addItems(DockManager->perspectiveNames());
    PerspectiveComboBox->setCurrentText(PerspectiveName);
}

//============================================================================
void CMainWindow::closeEvent(QCloseEvent* event)
{
    // Delete dock manager here to delete all floating widgets. This ensures
    // that all top level windows of the dock manager are properly closed
    DockManager->deleteLater();
    QMainWindow::closeEvent(event);
}
