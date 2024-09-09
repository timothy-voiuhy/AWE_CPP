#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QModelIndex>
#include <QtCore/Qt>
#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>
#include <QtGui/QFont>
#include <QtWidgets/QMainWindow>
#include <QStringListModel>
#include <QListView>
#include <QThread>
#include <sys/socket.h>
#include <string>

#include "repeater.h"
#include "siteMapWindow.h"
#include "guiUtilities.h"
#include "threadrunners.h"
#include "config.h"
#include "net/socketswrapper.h"

using namespace std;

class TargetWindow;

class ThreadMonitor : public QMainWindow{
    Q_OBJECT
public:
    ThreadMonitor(QWidget *parent = nullptr);
    ~ThreadMonitor();
};

class ThreadMon : public QWidget{
    Q_OBJECT
};

class ProxyInterceptWindow;

class SocketIPC : public QThread{
    Q_OBJECT
    signals:
        void processFinishedExecution(QWidget *parent, const QString &name);
    public:
        Socket *sock;
        bool create_server;
        int port;
        int sockfd;
        struct sockaddr_in addr;
        string server_ip;
        string client_ip;
        void createServerConnection();
        void createClientConnection(string client_ip);
        void sendFinishedMessage(string message);
        explicit SocketIPC(QObject *parent  = nullptr, bool create_server = false);
        explicit  SocketIPC(QObject *parent = nullptr);
        ~SocketIPC();

        void runServer();

    protected:
        void run() override;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupUI();
    void setupMenuBar();
    void setupConnections();

    SocketIPC *socketIpc;
    RepeaterWindow *repeaterWindow;
    SiteMapWindow *siteMapWindow;
    ThreadMonitor *threadMonitor;
    SessionHandlerRunner *sessionHandler;
    AtomProxy *proxy_;

    QString programStateFile;
    QString defaultWorkspaceDir;
    QStringListModel *dirsModel;
    QTabWidget *tabManager;
    QWidget *mainTabWidget;
    QVBoxLayout *mainTabLayout;
    QLabel *recentProjectsLabel;
    QFrame *openBarFrame;
    QHBoxLayout *openBarLayout;
    QLineEdit *choosenProjectDir;
    QPushButton *openProjectButton;
    QListView *dirListView;
    QPushButton *buttonAddTab;
    QScrollArea *dirsProjectsScrollArea;
    QList<TargetWindow *> openMainWindows;
    int openProjectCount;
    bool isProxyRunning;
    int proxyPort;
    QString proxyHostname;
    bool isSessionHandlerRunning;

signals:
    void newProjectCreated(QMainWindow *window);
    void projectClosed(QMainWindow *window, int code);
    void threadStarted(QWidget *widget, const QString &name);

private slots:
    void openRepeater();
    void finishedProcess(QWidget *windowInstance, const QString &tool);
    void addThreadMonitorTab();
    void addProjectsTab();
    void saveProgramState();
    void startSessionHandler();
    void openChoosenProject();
    void startProxy();
    void addRepeaterInstanceTab(const QString &request = QString());
    void addSiteMapTab();
    void openChosenProject();
    void addProjects();
    void openProject(const QModelIndex &index);
    void projectDirClicked(const QModelIndex &index);
    void addTargetWindow();
    void m_AddTargetTab();
    void addTargetTab(const QString &directory = QString());
    void closeTab();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QAction *projectsAction;
    QAction *addTabAction;
    QAction *startProxyAction;
    QAction *repeaterAction;
    QAction *sitemapAction;
    QAction *threadMonitorAction;
    QAction *closeTabAction;

};
#endif // MAINWINDOW_H
