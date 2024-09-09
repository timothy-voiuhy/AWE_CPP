// browserWindow.cpp

#include "browserWindow.h"
#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include <string>
#include "config.h"
#include "atomfunctions.h"

BrowserWindow::BrowserWindow(const QString& link, QWidget *parent)
    : QMainWindow(parent), certificate_file(QString::fromStdString(CERTIFICATE_FILE)),
    downloadPath(QString::fromStdString(HOME_DIR + "/Downloads/")),
    browser_cache_path(QString::fromStdString(RUNDIR + "/web_cache")){

    if (!QDir(downloadPath).exists()) {
        QDir().mkdir(downloadPath);
    }

    setupUI();

    engine_profile = setupProfile();
    browser = new QWebEngineView();
    Page = new QWebEnginePage();
    // Page->setCertificateErrorPolicy(QWebEngineCertificateError::CertificateErrorPolicy::Automatic);
    // connect(Page, &QWebEnginePage::certificateError, this, &BrowserWindow::browserCertificateError);
    browser->setPage(Page);

    trustCertificate();

    connect(browser, &QWebEngineView::urlChanged, this, &BrowserWindow::handleUrlChange);
    connect(browser, &QWebEngineView::loadProgress, this, &BrowserWindow::handleLoadProgress);
    connect(browser, &QWebEngineView::loadFinished, this, &BrowserWindow::closeProgressBarWidget);

    lowerCentralLayout->addWidget(browser);

    if (link.isEmpty()) {
        browser->setUrl(QUrl("http://google.com/"));
    } else {
        searchUrlOnBrowser(link);
    }
}

BrowserWindow::~BrowserWindow() {
    delete Page;
}

void BrowserWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    centralWidgetLayout = new QVBoxLayout();
    centralWidget->setLayout(centralWidgetLayout);

    upperUrlHandlerLayout = new QHBoxLayout();
    centralWidgetLayout->addLayout(upperUrlHandlerLayout);

    lowerCentralLayout = new QHBoxLayout();
    centralWidgetLayout->addLayout(lowerCentralLayout);

    addUrlHandler();
}

void BrowserWindow::trustCertificate() {
    QFile cert_file(certificate_file);
    if (cert_file.open(QIODevice::ReadOnly)) {
        QByteArray cert_data = cert_file.readAll();
        QSslCertificate certificate(cert_data);
        QSslConfiguration ssl_config = QSslConfiguration::defaultConfiguration();
        ssl_config.addCaCertificate(certificate);
        QSslConfiguration::setDefaultConfiguration(ssl_config);
    }
}

void BrowserWindow::browserCertificateError(QWebEngineCertificateError &error) {
    error.acceptCertificate();
}

QWebEngineProfile* BrowserWindow::setupProfile() {
    QWebEngineProfile *profile = QWebEngineProfile::defaultProfile();
    profile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    profile->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/125.0.0.0 Safari/537.36");
    profile->setDownloadPath(downloadPath);
    // profile->settings()->setAttribute(QWebEngineSettings::ForceDarkMode, true);
    profile->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    profile->settings()->setAttribute(QWebEngineSettings::WebGLEnabled, true);
    profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
    profile->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    profile->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);
    profile->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    connect(profile, &QWebEngineProfile::downloadRequested, this, &BrowserWindow::handleDownload);
    return profile;
}

void BrowserWindow::handleDownload(QWebEngineDownloadRequest *download) {
    QString old_path = download->url().path();
    QString suffix = QFileInfo(old_path).suffix();
    QString path = QFileDialog::getSaveFileName(this, "Save File", old_path, "*." + suffix);
    if (!path.isEmpty()) {
        download->setDownloadFileName(path);
        download->accept();
    }
}

void BrowserWindow::closeProgressBarWidget() {
    browserProgressBar->setVisible(false);
}

void BrowserWindow::handleLoadProgress(int prog) {
    browserProgressBar->setVisible(true);
    browserProgressBar->setMinimum(0);
    browserProgressBar->setMaximum(100);
    browserProgressBar->setValue(prog);
}

void BrowserWindow::handleUrlChange() {
    QUrl _Qurl = browser->url();
    QString str_Url = _Qurl.toString();
    urlText->setText(str_Url);
}

void BrowserWindow::addUrlHandler() {
    urlLabel = new QLabel("Url:");
    urlText = new QLineEdit();
    searchButton = new QPushButton("search");
    clearButton = new QPushButton("X");

    clearButton->setToolTip("clear the search area");
    clearButton->setFixedWidth(32);

    connect(clearButton, &QPushButton::clicked, this, &BrowserWindow::urlTextClear);
    connect(searchButton, &QPushButton::clicked, this, &BrowserWindow::handleSearchButton);

    browserProgressBar = new QProgressBar();
    browserProgressBar->setVisible(false);
    browserProgressBar->setFormat("Loading");

    upperUrlHandlerLayout->addWidget(urlLabel);
    upperUrlHandlerLayout->addWidget(urlText);
    upperUrlHandlerLayout->addWidget(searchButton);
    upperUrlHandlerLayout->addWidget(clearButton);
    upperUrlHandlerLayout->addWidget(browserProgressBar);
}

void BrowserWindow::urlTextClear() {
    urlText->clear();
}

void BrowserWindow::searchUrlOnBrowser(const QString &link) {
    browser->setUrl(QUrl(QString::fromStdString(addHttpsScheme(link.toStdString()))));
    QUrl QbrowserURL = browser->url();
    QString strUrl = QbrowserURL.toString();
    urlText->setText(strUrl);
}

void BrowserWindow::handleSearchButton() {
    target_url = urlText->text();
    target_url = QString::fromStdString(addHttpsScheme(target_url.toStdString()));
    browser->setUrl(QUrl(target_url));
    QUrl QbrowserURL = browser->url();
    QString strUrl = QbrowserURL.toString();
    urlText->setText(strUrl);
}
