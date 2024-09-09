#include "targetWindow.h"

TargetWindow::TargetWindow(const QString &project_dir_path, int proxy_port, MainWindow *top_parent, int index, QWidget *parent)
    : QMainWindow(parent), projectDirPath(project_dir_path), proxy_port(proxy_port), topParent(top_parent), projectIndex(index), proxy_status(false) {

    setObjectName(projectDirPath);
    getMainSeverName();

    lowerDock = new LowerDock(this, projectDirPath);
    lowerDock->InitializeLowerDock()->setVisible(false);

    rightDock = new RightDock(this, projectDirPath);
    rightDock->InitializeDock()->setVisible(false);

    // the parent is the same as the target window for the leftDock
    leftDock = new LeftDock(this, projectDirPath, topParent);
    // connect(leftDock, &LeftDock::openLinkInBrw, this, &TargetWindow::openNewBrowserTab);
    // leftDock->InitializeLeftDock();

    QWidget *centralWidget = new QWidget();
    QVBoxLayout *centralWidgetLayout = new QVBoxLayout();

    browserMainWindow = new QMainWindow();
    centralWidgetLayout->addWidget(browserMainWindow);

    QWidget *browserMainWindowCentralWidget = new QWidget();
    browserMainWindow->setCentralWidget(browserMainWindowCentralWidget);

    QVBoxLayout *browserMainWindowLayout = new QVBoxLayout();
    browserMainWindowCentralWidget->setLayout(browserMainWindowLayout);

    browserTabWidget = new QTabWidget();
    browserMainWindowLayout->addWidget(browserTabWidget);
    openNewBrowserTab();

    centralWidget->setLayout(centralWidgetLayout);
    setCentralWidget(centralWidget);

    targetwindowMenu = browserMainWindow->menuBar();

    AddTopMenu();

    // Network Button
    NetworkAction = new QAction(QIcon::fromTheme("network-wired"), "network", this);
    connect(NetworkAction, &QAction::triggered, this, &TargetWindow::OpenNetworkWindow);
    targetwindowMenu->addAction(NetworkAction);

    // Add New Browser Tab
    newBrowserTabAction = new QAction("new", this);
    connect(newBrowserTabAction, &QAction::triggered, this, &TargetWindow::openNewBrowserTab_);
    targetwindowMenu->addAction(newBrowserTabAction);

    // Close Browser Tab
    closeTabAction = new QAction("close", this);
    connect(closeTabAction, &QAction::triggered, this, &TargetWindow::closeBrowserTab);
    targetwindowMenu->addAction(closeTabAction);

    // Developer Tools Button
    devToolsAction = new QAction("dev", this);
    connect(devToolsAction, &QAction::triggered, this, &TargetWindow::showdevTools);
    targetwindowMenu->addAction(devToolsAction);

    // Proxy Toggle Button
    handleProxyAction = new QAction("EnableProxy", this);
    connect(handleProxyAction, &QAction::triggered, this, &TargetWindow::HandleProxy);
    targetwindowMenu->addAction(handleProxyAction);

    // Test Target Button
    testTargetAction = new QAction("testTarget", this);
    connect(testTargetAction, &QAction::triggered, this, &TargetWindow::OpenTestTargetWindow);
    targetwindowMenu->addAction(testTargetAction);

    setWindowTitle("atom");
    centralWidgetLayout->addStretch();
}

TargetWindow::~TargetWindow() {}

void TargetWindow::showdevTools() {
    // Implementation for developer tools
}

void TargetWindow::getMainSeverName() {
    if (!projectDirPath.endsWith("/")) {
        main_server_name = projectDirPath.section("/", -1);
    }
}

void TargetWindow::closeEvent(QCloseEvent *event) {
    emit(this, topParent->projectClosed(this, projectIndex));
    QMainWindow::closeEvent(event);
}

void TargetWindow::openNewBrowserTab_(){
    openNewBrowserTab();
}

void TargetWindow::openNewBrowserTab(const QString &link) {
    QString tab_name = "new";
    BrowserWindow *BrowserWindow_ = nullptr;

    try {
        if (link.isEmpty()) {
            BrowserWindow_ = new BrowserWindow("google.com");
        } else {
            BrowserWindow_ = new BrowserWindow(link);
            if (link.startsWith("https") || link.startsWith("http")) {
                tab_name = link.section("//", 1, 1).section(".", 0, 0);
            } else {
                tab_name = link.section(".", 0, 0);
            }
        }
    } catch (...) {
        tab_name = "new";
    }

    browserTabWidget->addTab(BrowserWindow_, tab_name);
    browserTabWidget->setCurrentIndex(browserTabWidget->indexOf(BrowserWindow_));
}

void TargetWindow::closeBrowserTab() {
    current_tab_index = browserTabWidget->currentIndex();
    if (current_tab_index != 0) {
        browserTabWidget->removeTab(current_tab_index);
    }
}

void TargetWindow::HandleProxy() {
    if (!proxy_status) {
        enableProxy(true);
        handleProxyAction->setText("DisableProxy");
        proxy_status = true;
    } else {
        handleProxyAction->setText("EnableProxy");
        proxy_status = false;
        QNetworkProxyFactory::setUseSystemConfiguration(true);
    }
}

void TargetWindow::enableProxy(bool use_default) {
    if (use_default) {
        proxy.setHostName("127.0.0.1");
        proxy.setPort(proxy_port);
        proxy.setType(QNetworkProxy::HttpProxy);
        QNetworkProxy::setApplicationProxy(proxy);
    } else {
        enableProxyCheckBox->setChecked(true);
        proxy.setHostName(proxyHostNameLineEdit->text());
        proxy.setPort(proxyPortNameLineEdit->text().toInt());
        proxy.setType(QNetworkProxy::HttpProxy);
        QNetworkProxy::setApplicationProxy(proxy);
        enableProxyCheckBox->setChecked(true);
        // LoadCA_Certificate();
    }
}

void TargetWindow::OpenTestTargetWindow()
{
}

void TargetWindow::OpenNetworkWindow()
{
}

// void TargetWindow::OpenTestTargetWindow() {
//     testWindow = new TestTargetWindow(projectDirPath, this, topParent);
//     testWindow->Initialize();
//     testWindow->setFixedSize(800, 600);
//     testWindow->show();
// }

// void TargetWindow::OpenNetworkWindow() {
//     networkWindow = new NetworkWindow();
//     networkWindow->IniatializeNetworkWindow();
//     networkWindow->resize(800, 600);
//     networkWindow->show();
// }

void TargetWindow::AddTopMenu() {
    QIcon MenuIcon("resources/icons/settings-icon-gear-3d-render-png.png");
    QMenu *centralWidgetMenu = new QMenu();
    centralWidgetMenu->setIcon(MenuIcon);

    QMenu *a_Open = centralWidgetMenu->addMenu("open");
    QAction *a_openProjects = a_Open->addAction("projects");
    connect(a_openProjects, &QAction::triggered, this, &TargetWindow::OpenProject);

    centralWidgetMenu->addSeparator();

    QMenu *a_viewMenu = centralWidgetMenu->addMenu("view");
    QAction *a_terminal = a_viewMenu->addAction("terminal");
    connect(a_terminal, &QAction::triggered, this, &TargetWindow::ViewTerminal);

    QAction *target = a_viewMenu->addAction("target");
    connect(target, &QAction::triggered, this, &TargetWindow::ViewTarget);

    QAction *notePad = a_viewMenu->addAction("notepad");
    connect(notePad, &QAction::triggered, this, &TargetWindow::ViewNotepad);

    centralWidgetMenu->addSeparator();

    QAction *BrowserSettings_action = centralWidgetMenu->addAction("Browser settings");
    connect(BrowserSettings_action, &QAction::triggered, this, &TargetWindow::openBrowserSettingsWindow);

    targetwindowMenu->addMenu(centralWidgetMenu);
}

void TargetWindow::openBrowserSettingsWindow() {
    BrowserSettingsWindow = new QMainWindow();
    BrowserSettingsWindowWidget = new QWidget();
    BrowserSettingsWindow->setCentralWidget(BrowserSettingsWindowWidget);

    BrowserSettingsWindowLayout = new QVBoxLayout();
    BrowserSettingsWindowWidget->setLayout(BrowserSettingsWindowLayout);

    QLabel *ProxyLabel = new QLabel("<b>Proxy Settings</b>");
    BrowserSettingsWindowLayout->addWidget(ProxyLabel);

    enableProxyLayout = new QFormLayout();
    enableProxyCheckBox = new QCheckBox();
    QLabel *enableProxyLabel = new QLabel("enable Proxy");
    enableProxyLayout->addRow(enableProxyLabel, enableProxyCheckBox);

    QLabel *proxyhostNameLabel = new QLabel("HostName:");
    proxyHostNameLineEdit = new QLineEdit();
    proxyHostNameLineEdit->setPlaceholderText("127.0.0.1");
    enableProxyLayout->addRow(proxyhostNameLabel, proxyHostNameLineEdit);

    QLabel *proxyPortLabel = new QLabel("Port:");
    proxyPortNameLineEdit = new QLineEdit();
    proxyPortNameLineEdit->setPlaceholderText("8081");
    enableProxyLayout->addRow(proxyPortLabel, proxyPortNameLineEdit);

    proxyDoneButton = new QPushButton("Yes");
    connect(proxyDoneButton, &QPushButton::clicked, this, &TargetWindow::enableProxy);
    enableProxyLayout->addRow("Do you want to set the proxy?", proxyDoneButton);

    BrowserSettingsWindowLayout->addLayout(enableProxyLayout);
    BrowserSettingsWindowLayout->addStretch();

    BrowserSettingsWindow->setFixedSize(600, 600);
    BrowserSettingsWindow->show();
}

void TargetWindow::LowerDockClick() {
    lowerDock->activateWindow();
}

void TargetWindow::ViewTarget() {
    leftDock->setVisible(true);
}

void TargetWindow::ViewTerminal() {
    lowerDock->setVisible(true);
}

void TargetWindow::OpenProject() {
    QFileDialog file_menu;
    QString filename = file_menu.getOpenFileName(this, "Open File", "/", "Text Files (*.txt)");
}

void TargetWindow::ViewNotepad() {
    rightDock->setVisible(true);
}

void TargetWindow::LoadProject() {
    // Implementation for loading the project
}
