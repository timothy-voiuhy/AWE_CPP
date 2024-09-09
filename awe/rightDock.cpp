#include <QMainWindow>
#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QTextDocument>
#include "scieditor.h"
#include "texteditor.h"
#include "syntaxHighlighter.h" 

#include "rightDock.h"

RightDock::RightDock(QMainWindow* mainWindow, const QString& projectsDir, QWidget* parent)
    : QWidget(parent), MainWindow(mainWindow), projectsDirPath(projectsDir) 
{
    InitializeDock();
}

QDockWidget *RightDock::InitializeDock()
{
    rightDock = new QDockWidget("Take Your Notes, Edit Files", MainWindow);
    rightDockWidget = new QWidget();
    rightDock->setWidget(rightDockWidget);

    rightDockLayoutMainLayout = new QVBoxLayout;
    rightDockWidget->setLayout(rightDockLayoutMainLayout);

    tabManager = new QTabWidget;
    rightDockNotePad = new ScintillaEditor();
    tabManager->addTab(rightDockNotePad, "notepad");
    // rightDockNotePad->show();

    rightDockLayoutMainLayout->addWidget(tabManager);
    rightDockNotePad->show();

    MainWindow->addDockWidget(Qt::RightDockWidgetArea, rightDock);
    return rightDock;
}
