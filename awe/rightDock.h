#ifndef RIGHTDOCK_H
#define RIGHTDOCK_H

#include <QMainWindow>
#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QTextDocument>

class ScintillaEditor;

class RightDock : public QWidget
{
    Q_OBJECT

public:
    RightDock(QMainWindow* mainWindow, const QString& projectsDir, QWidget* parent = nullptr);
    QDockWidget *InitializeDock();


private:
    QMainWindow* MainWindow;
    QString projectsDirPath;
    // QString rightDockNotePadOpenfile;
    // QString notesFile;
    // QString notesFilePath;
    QWidget* rightDockWidget;
    QVBoxLayout* rightDockLayoutMainLayout;
    QTabWidget* tabManager;
    ScintillaEditor* rightDockNotePad;
    QList<ScintillaEditor*> openEditors;
    QDockWidget *rightDock;
};

#endif // RIGHTDOCK_H
