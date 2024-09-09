#include <QDir>
#include <QFile>
#include <QtCore/Qt>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QListView>
#include <QScrollArea>
#include <QPushButton>
#include <QCoreApplication>
#include <QAction>
#include <QFileDialog>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <QThread>
#include <QDebug>
#include <iostream>

#include "mainwindow.h"
#include "repeater.h"
#include "siteMapWindow.h"
#include "guiUtilities.h"
#include "threadrunners.h"
#include "config.h"
#include "net/socketswrapper.h"
#include "targetWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isSessionHandlerRunning(false), isProxyRunning(false), openProjectCount(0){
    socketIpc = new SocketIPC(this, true);
    socketIpc->start();

    openProjectCount = 0;

    setObjectName("mainWindow");
    programStateFile = QString("%1/programState/programState.txt").arg(QCoreApplication::applicationDirPath());

    // Initialize directories
    QDir dir(QFileInfo(programStateFile).absolutePath());
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    if (QFile::exists(programStateFile)) {
        QFile file(programStateFile);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray programStateBytes = file.readAll();
            // this->restoreState(programStateBytes);
        }
    }

    // Initialize the rest of the UI components
    setupUI();
    setupMenuBar();
    setupConnections();
}

MainWindow::~MainWindow() {
    saveProgramState();
}

void MainWindow::setupUI() {
    // Setup central widget
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    tabManager = new QTabWidget(centralWidget);

    mainLayout->addWidget(tabManager);
    setCentralWidget(centralWidget);

    repeaterWindow = new RepeaterWindow(this);
}

void MainWindow::addTargetWindow(){

}

void MainWindow::m_AddTargetTab()
{
}

void MainWindow::addTargetTab(const QString &directory)
{
    openProjectCount += 1;
    TargetWindow *targetWindowInstance = new TargetWindow(directory, proxyPort, this, openProjectCount, this);
    openMainWindows.append(targetWindowInstance);
    QDir dir_(directory);
    tabManager->addTab(targetWindowInstance, dir_.dirName());
    tabManager->setCurrentIndex(tabManager->indexOf(targetWindowInstance));
}

void MainWindow::closeTab(){
    int current_idx = tabManager->currentIndex();
    tabManager->currentWidget()->close();
    tabManager->removeTab(current_idx);
}

void MainWindow::setupMenuBar() {
    // Setup menu bar actions
    this->projectsAction = new QAction("Projects", this);
    connect(projectsAction, &QAction::triggered, this, &MainWindow::addProjectsTab);
    menuBar()->addAction(projectsAction); // menuBar() returns the menu bar for the main window

    // Add Target tab
    this->addTabAction = new QAction("AddTarget", this);
    connect(addTabAction, &QAction::triggered, this, &MainWindow::addTargetWindow);
    menuBar()->addAction(addTabAction);
    menuBar()->addSeparator();

    // Start Proxy tab
    this->startProxyAction = new QAction("StartProxy", this);
    connect(this->startProxyAction, &QAction::triggered, this, &MainWindow::startProxy);
    menuBar()->addAction(startProxyAction);

    // Repeater button
    this->repeaterAction = new QAction("Repeater", this);
    connect(this->repeaterAction, &QAction::triggered, this, &MainWindow::openRepeater);
    menuBar()->addAction(this->repeaterAction);

    // Sitemap button
    this->sitemapAction = new QAction("Sitemap", this);
    connect(this->sitemapAction, &QAction::triggered, this, &MainWindow::addSiteMapTab);
    menuBar()->addAction(this->sitemapAction);

    // Threads Monitor tab
    this->threadMonitorAction = new QAction("Threads", this);
    connect(this->threadMonitorAction, &QAction::triggered, this, &MainWindow::addThreadMonitorTab);
    menuBar()->addAction(this->threadMonitorAction);

    // Close Tab action
    this->closeTabAction = new QAction("CloseTab", this);
    connect(this->closeTabAction, &QAction::triggered, this, &MainWindow::closeTab);
    menuBar()->addAction(this->closeTabAction);
    menuBar()->addSeparator();
}

void MainWindow::setupConnections() {
    connect(socketIpc, &SocketIPC::processFinishedExecution, this, &MainWindow::finishedProcess);
    connect(this, &MainWindow::newProjectCreated, this, []() {
        // Handle new project created
    });
}

void MainWindow::openRepeater() {
    tabManager->addTab(repeaterWindow, "repeater");
}

void MainWindow::finishedProcess(QWidget *windowInstance, const QString &tool) {
    qDebug() << tool << "finished execution";
}

void MainWindow::addThreadMonitorTab()
{
    // Implementation for adding thread monitor tab
}

void MainWindow::addProjectsTab() {
    // Create and set up main tab widget and layout
    mainTabWidget = new QWidget(this);
    mainTabLayout = new QVBoxLayout(mainTabWidget);
    this->tabManager->setLayout(mainTabLayout);

    // Recent projects label
    recentProjectsLabel = new QLabel("<b>Recent Projects</b>", this);
    mainTabLayout->addWidget(recentProjectsLabel, 0, Qt::AlignCenter);

    addProjects();

    // Open bar frame
    openBarFrame = new QFrame(this);
    openBarLayout = new QHBoxLayout(openBarFrame);

    // Chosen project directory input
    choosenProjectDir = new QLineEdit(this);
    choosenProjectDir->setFixedWidth(400);
    openBarLayout->addWidget(choosenProjectDir);

    // Open project button
    openProjectButton = new QPushButton("Open", this);
    openProjectButton->setFixedWidth(50);
    connect(openProjectButton, &QPushButton::clicked, this, &MainWindow::openChoosenProject);
    openBarLayout->addWidget(openProjectButton);

    openBarFrame->setLayout(openBarLayout);
    mainTabLayout->addWidget(openBarFrame, 0, Qt::AlignCenter);

    // Add target button
    buttonAddTab = new QPushButton("Add Target", this);
    buttonAddTab->setFixedWidth(120);
    connect(buttonAddTab, &QPushButton::clicked, this, &MainWindow::addTargetWindow);
    mainTabLayout->addWidget(buttonAddTab);
    mainTabLayout->setAlignment(buttonAddTab, Qt::AlignCenter);

    mainTabWidget->setLayout(mainTabLayout);

    // Add the tab widget to the tab manager
    tabManager->addTab(mainTabWidget, "Projects");

    // Add stretch
    mainTabLayout->addStretch();
}

void MainWindow::saveProgramState() {
    QByteArray state = saveState(); // Define saveState() in your class
    QFile file(programStateFile);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(state);
    }
}

void MainWindow::startSessionHandler() {
    if (!isSessionHandlerRunning) {
        qDebug() << "Starting Session Handler";
        this->sessionHandler = new SessionHandlerRunner(this);
        this->sessionHandler->start();
        isSessionHandlerRunning = true;
    } else {
        QMessageBox::information(this, "Information", "The session handler is already running");
    }
}

void MainWindow::openChoosenProject(){
    QString dir_name  = QString("%1").arg(QString::fromStdString(DEFAULT_WORKSPACE_DIR))+choosenProjectDir->text();
    QDir dir_(dir_name);
    if(dir_.exists()){
        qDebug() << "opening: " << dir_name;
    }else{
        dir_.mkdir(QString::fromStdString(DEFAULT_WORKSPACE_DIR));
    }
    addTargetTab(dir_name);
}

void MainWindow::startProxy() {
    if (!isProxyRunning) {
        proxyPort = QRandomGenerator::global()->bounded(8000, 10000);
        proxyHostname = "127.0.0.1";
        qDebug() << "Starting proxy";
        proxy_ = new AtomProxy(proxyPort, this);
        proxy_->start();
        isProxyRunning = true;
    } else {
        QMessageBox::information(this, "Information", "The proxy is already running");
    }
}

void MainWindow::addRepeaterInstanceTab(const QString &request)
{
}

void MainWindow::addSiteMapTab()
{
}

void MainWindow::openChosenProject()
{
}

void MainWindow::addProjects()
{
    QString defaultWorkspaceDir = QString::fromStdString(DEFAULT_WORKSPACE_DIR); // Update this path
    QDir dir(defaultWorkspaceDir);
    QStringList availableDirs;

    foreach (const QString &entry, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        if (entry != "Proxy")
        {
            availableDirs << entry;
        }
    }

    dirsModel = new QStringListModel(availableDirs, this);
    dirListView = new QListView(this);
    dirListView->setFont(QFont("Cascadia Code", 11));
    dirListView->setEditTriggers(QListView::NoEditTriggers);
    dirListView->setModel(dirsModel);

    connect(dirListView, &QListView::clicked, this, &MainWindow::projectDirClicked);
    connect(dirListView, &QListView::doubleClicked, this, &MainWindow::openProject);

    dirsProjectsScrollArea = new QScrollArea(this);
    dirsProjectsScrollArea->setWidget(dirListView);
    dirsProjectsScrollArea->setWidgetResizable(true);
    dirsProjectsScrollArea->setFixedHeight(450);
    dirsProjectsScrollArea->setFixedWidth(450);

    this->mainTabLayout->addWidget(dirsProjectsScrollArea, 0, Qt::AlignCenter);
}

void MainWindow::openProject(const QModelIndex &index){
    projectDirClicked(index);
    openChoosenProject();
}

void MainWindow::projectDirClicked(const QModelIndex &index)
{
    QString clicked_dir = dirsModel->data(index, Qt::DisplayRole).toString();
    choosenProjectDir->setText(clicked_dir);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (QSysInfo::productType() == "windows") {
        system("taskkill /F /PID " + this->proxy_->processId());
        system("taskkill /F /PID " + this->sessionHandler->processId());
    } else if (QSysInfo::productType() == "linux") {
        // Use kill command on Unix-like systems
        system(("kill " + QString::number(proxy_->processId())).toStdString().c_str());
        system(("kill " + QString::number(sessionHandler->processId())).toStdString().c_str());
    }
    QMainWindow::closeEvent(event);
}


SocketIPC::SocketIPC(QObject *parent, bool create_server)
{
    this->server_ip  = "127.0.0.1";
    this->port = 57788;
    if(create_server == true){
        this->create_server = true;
    }else
    {
        this->create_server = false;
    }
    this->port = port;
}

SocketIPC::SocketIPC(QObject *parent){
    this->server_ip = "127.0.0.1";
    this->port = 57788;
}

void SocketIPC::createServerConnection(){
    this->sock = new Socket(true, "127.0.0.1", 57788);
    this->sock->netBind();
    this->sock->netListen();
    do
    {
        this->sock->netAccept();
        vector<string> response = this->sock->netReceive(1000);
        string process_object_name = response[0];
        int num_bytes = stoi(response[1]);
        if (process_object_name == "atomrunner"){
            // emit this->processFinishedExecution(this, "atomrunner");
        }
        else if(process_object_name == "getAllUrlsRunner"){
                // emit this->processFinishedExecution(this, "getAllUrlsRunner");
        }
    } while (1);
    
}

void SocketIPC::sendFinishedMessage(string process_object_name){
    this->sock->netSend(process_object_name.c_str());
}

void SocketIPC::createClientConnection(string client_ip){
    this->sock = new Socket(false,"127.0.0.1", 57788);
    this->sock->netConnect();
}

SocketIPC::~SocketIPC()
{
}

void SocketIPC::runServer(){
    this->createServerConnection();
}

void SocketIPC::run()
{
}

ThreadMonitor::ThreadMonitor(QWidget *parent)
{
}

ThreadMonitor::~ThreadMonitor()
{
}
