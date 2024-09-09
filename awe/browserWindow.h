#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineCore/QWebEnginePage>
#include <QtWebEngineCore/QWebEngineSettings>
#include <QtWebEngineCore/QWebEngineProfile>
#include <QtWebEngineCore/QWebEngineCertificateError>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QFileDialog>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslConfiguration>
#include <QUrl>
#include <QFileInfo>
#include <QAction>
#include "../../../../../../../usr/include/x86_64-linux-gnu/qt6/QtWebEngineCore/qwebenginedownloadrequest.h"

std::string addHttpsScheme(const std::string &link);

class BrowserWindow : public QMainWindow {
    Q_OBJECT

public:
    QWebEnginePage *Page;
    explicit BrowserWindow(const QString& link = QString(), QWidget *parent = nullptr);
    ~BrowserWindow();

private slots:
    void handleUrlChange();
    void handleLoadProgress(int prog);
    void handleSearchButton();
    void handleDownload(QWebEngineDownloadRequest *download);
    void browserCertificateError(QWebEngineCertificateError &error);
    void closeProgressBarWidget();
    void urlTextClear();

private:
    void setupUI();
    void trustCertificate();
    QWebEngineProfile* setupProfile();
    void addUrlHandler();
    void searchUrlOnBrowser(const QString &link);

    QString certificate_file;
    QString downloadPath;
    QString browser_cache_path;
    QString target_url;

    QWebEngineView *browser;
    QWebEngineProfile *engine_profile;
    QVBoxLayout *centralWidgetLayout;
    QHBoxLayout *upperUrlHandlerLayout;
    QHBoxLayout *lowerCentralLayout;
    QLabel *urlLabel;
    QLineEdit *urlText;
    QPushButton *searchButton;
    QPushButton *clearButton;
    QProgressBar *browserProgressBar;
};

#endif // BROWSERWINDOW_H
