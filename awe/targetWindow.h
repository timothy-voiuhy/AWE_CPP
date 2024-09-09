#ifndef TARGETWINDOW_H
#define TARGETWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QMenuBar>
#include <QAction>
#include <QCloseEvent>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkProxyFactory>
#include <QIcon>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QFileDialog>

#include "browserWindow.h"
#include "leftDock.h"
#include "lowerDock.h"
#include "rightDock.h"
#include "networkWindow.h"
#include "testtargetwindow.h"

class TargetWindow : public QMainWindow {
    Q_OBJECT

public:
    // TestTargetWindow testWindow;
    TargetWindow(const QString &project_dir_path, int proxy_port, MainWindow *top_parent, int index, QWidget *parent = nullptr);
    void openNewBrowserTab_();
    ~TargetWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void showdevTools();
    void getMainSeverName();
    void openNewBrowserTab(const QString &link = QString());
    void closeBrowserTab();
    void HandleProxy();
    void enableProxy(bool use_default = false);
    void OpenTestTargetWindow();
    void OpenNetworkWindow();
    void AddTopMenu();
    void openBrowserSettingsWindow();
    void LowerDockClick();
    void ViewTarget();
    void ViewTerminal();
    void OpenProject();
    void ViewNotepad();
    void LoadProject();

private:
    QString main_server_name;
    QString projectDirPath;
    MainWindow *topParent;
    int projectIndex;
    bool proxy_status;
    int proxy_port;
    int current_tab_index;

    QNetworkProxy proxy;
    QMainWindow *browserMainWindow;
    QTabWidget *browserTabWidget;
    QMenuBar *targetwindowMenu;

    QAction *NetworkAction;
    QAction *newBrowserTabAction;
    QAction *closeTabAction;
    QAction *devToolsAction;
    QAction *handleProxyAction;
    QAction *testTargetAction;

    LeftDock *leftDock;
    RightDock *rightDock;
    LowerDock *lowerDock;
    // NetworkWindow *networkWindow;
    // TestTargetWindow *testWindow;
    BrowserWindow *browserWindow;
    QMainWindow *BrowserSettingsWindow;
    QWidget *BrowserSettingsWindowWidget;
    QVBoxLayout *BrowserSettingsWindowLayout;
    QFormLayout *enableProxyLayout;
    QCheckBox *enableProxyCheckBox;
    QLineEdit *proxyHostNameLineEdit;
    QLineEdit *proxyPortNameLineEdit;
    QPushButton *proxyDoneButton;
};

#endif // TARGETWINDOW_H
