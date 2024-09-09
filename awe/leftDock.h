#ifndef LEFTDOCK_H
#define LEFTDOCK_H

#include <QObject>
#include <QDockWidget>
#include <QString>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QMap>
#include <QProcess>
#include <QFile>
#include <filesystem>
#include <fstream>
#include "mainwindow.h"

namespace fs = std::filesystem;

class LeftDock;
class TargetWindow;

class UrlFetcher : public QThread
{
    Q_OBJECT

public:
    explicit UrlFetcher(const QMap<QString, QStringList>& subdomainUrlDict,
                       const QString& workingDir,
                       QWidget* parent,
                       LeftDock* dictParent,
                       MainWindow* topParent,
                       TargetWindow* mainWindow);
    std::vector<std::string> atomGuiGetUrls(const std::string &subdomain, const std::string &workingDir,
                                            QWidget *parent,
                                            MainWindow *top_parent, TargetWindow *mainWindow);
    ~UrlFetcher() override;

signals:
    void UrlFetcherFinished();
    void processFinishedExecution(MainWindow* windowInstance, const QString& tool);

protected:
    void run() override;

private slots:
    void closeThread();
    void processFinishedExecutionSlot(QWidget* windowInstance, const QString& tool);

private:
    TargetWindow* m_mainWindow;
    MainWindow* m_topParent;
    QMap<QString, QString> m_subdomainUrlDict;
    QMap<QString, QList<QString>> m_subdomainsUrlDict_;
    QString m_workingDir;
    QString m_subdomainsUrlDictFile;
    LeftDock* m_dictParent;
    int m_receivedSignals;

    void writeSubdomainsUrlDictToFile();
};

class LeftDock : public QWidget{
    Q_OBJECT

public:
    bool UrlFetcherRunning_;
    bool whois_displaying_;
    bool location_table_drawn_;
    QString projectDirPath_;
    QString SubdomainUrlDict_file_;
    QString amass_data_json_file_;
    QString amassDataJsonFile;
    QStandardItemModel*  subdomainsModel;
    QList<QVariantMap> locationTableItemDicts;
    QLineEdit *searchLineEdit;
    QTextBrowser *whoisTextWidget;
    UrlFetcher *UrlGetter;
    QDockWidget *leftDock_;
    QWidget *leftDockWidget_;
    QMap<QString, QStringList> SubdomainUrlDict;
    MainWindow *topParent;

    LeftDock(TargetWindow *main_window, const QString& project_dir_path, MainWindow* top_parent);
    ~LeftDock();
signals:
    void openLinkInBrw(const QString& link);

public slots:
    void InitializeLeftDock();
    void draw_location_table();
    void searchLocationTable();
    void run_whois();
    void displayWhoisResults(QObject* parent, const QString& objectName);
    void handleWhoisProcessFinished();
    void handleError(QProcess::ProcessError error);

private:
    void setupUI();
    void showSbdUrlTree();
    void updateModel();
    void hideGenInfo();
    void UrlsScan();
    void openLinkInBrowser(const QModelIndex &index);

    // Member Variables
    TargetWindow* mainWindow;
    QDockWidget* leftDock;
    QFormLayout* infoshowLayout;
    QVBoxLayout* leftDockLayout;
    QFrame* general_information_frame;
    QTableWidget* locationTable;
    QPushButton* searchButton;
    QLineEdit* searchField;
    QLabel* searchLabel;
    QCheckBox* searchExactMatchCheckBox;
    QProcess* whoisProcess;
    QCheckBox *infoShowCheckBox;
    QString SubdomainUrlDictFile;
    bool locationTableDrawn;
};

#endif // LEFTDOCK_H
