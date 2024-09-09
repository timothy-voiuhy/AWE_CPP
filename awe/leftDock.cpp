#include <QMainWindow>
#include <QDockWidget>
#include <QDir>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFormLayout>
#include <QLabel>
// #include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QDockWidget>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QTreeView>
#include <QSplitter>
#include <QMessageBox>
#include <QTextBrowser>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QHeaderView>
#include <QFont>
#include <QFontMetrics>
#include <QFontDatabase>
#include <QProcess>

#include "leftDock.h"
#include "atomfunctions.h"
#include "targetWindow.h"

LeftDock::LeftDock(TargetWindow* main_window, const QString& project_dir_path, MainWindow* top_parent)
    : UrlFetcherRunning_(false), whois_displaying_(false), projectDirPath_(project_dir_path), 
    location_table_drawn_(false), topParent(top_parent), mainWindow(main_window){
    amass_data_json_file_ = projectDirPath_ + "/emcpData.json";
    SubdomainUrlDictFile = projectDirPath_ + "/subdomainsUrlDict.json";
    leftDock_ = new QDockWidget(tr("Target Information"), mainWindow);
    leftDockWidget_ = new QWidget();
    leftDock_->setWidget(leftDockWidget_);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, leftDock_);
    // Layout for left dock
    QVBoxLayout* left_dock_layout = new QVBoxLayout();
    leftDockWidget_->setLayout(left_dock_layout);

    QSplitter* left_dock_splitter = new QSplitter();
    left_dock_layout->addWidget(left_dock_splitter);

    // Upper widget in the splitter
    QWidget* left_dock_up_widget = new QWidget();
    QVBoxLayout* left_dock_up_widget_layout = new QVBoxLayout();
    left_dock_up_widget->setLayout(left_dock_up_widget_layout);
    left_dock_splitter->addWidget(left_dock_up_widget);

    // General information scroll area
    QScrollArea* gen_info_scroll_area = new QScrollArea();
    left_dock_up_widget_layout->addWidget(gen_info_scroll_area);
    QWidget* gen_info_scroll_bar_widget = new QWidget();
    QVBoxLayout* gen_info_scroll_bar_wid_layout = new QVBoxLayout();
    gen_info_scroll_bar_widget->setLayout(gen_info_scroll_bar_wid_layout);
    gen_info_scroll_area->setWidget(gen_info_scroll_bar_widget);
    gen_info_scroll_area->setWidgetResizable(true);

    // Hide or show general info checkbox
    infoshowLayout = new QFormLayout();
    QCheckBox* infoShowCheckBox = new QCheckBox();
    infoShowCheckBox->setChecked(true);
    connect(infoShowCheckBox, &QCheckBox::stateChanged, this, &LeftDock::hideGenInfo);
    infoshowLayout->addRow("hide info", infoShowCheckBox);
    gen_info_scroll_bar_wid_layout->addLayout(infoshowLayout);

    // General information frame and layout
    general_information_frame = new QFrame();
    gen_info_scroll_bar_wid_layout->addWidget(general_information_frame);
    QFormLayout* general_information_layout = new QFormLayout();
    general_information_frame->setLayout(general_information_layout);
    general_information_frame->setHidden(true);

    // Rows for static information
    QLabel* urlTargetName = new QLabel("main server: ");
    QLabel* urlName = new QLabel(mainWindow->property("main_server_name").toString());
    general_information_layout->addRow(urlTargetName, urlName);
    QLabel* numberOfSubdomains = new QLabel("nSubdomains");
    QLabel* nSubd = new QLabel("0");
    QLabel* amassSdCount = new QLabel(" =>Amass:");
    QLabel* amassSdCountLabel = new QLabel("0");
    QLabel* subdomainizerSdCount = new QLabel(" =>subdomainizer:");
    QLabel* subdomainizerSdCountLabel = new QLabel("0");
    QLabel* sublist3rSdCount = new QLabel(" =>sublist3r:");
    QLabel* sublist3rSdCountLabel = new QLabel("0");
    general_information_layout->addRow(numberOfSubdomains, nSubd);
    general_information_layout->addRow(amassSdCount, amassSdCountLabel);
    general_information_layout->addRow(subdomainizerSdCount, subdomainizerSdCountLabel);
    general_information_layout->addRow(sublist3rSdCount, sublist3rSdCountLabel);

    // Location elements
    QLabel* location_label = new QLabel("<b>Server Locations Table</b>");
    QHBoxLayout* location_menu_layout = new QHBoxLayout();
    QPushButton* generate_table_button = new QPushButton("Generate Table");
    connect(generate_table_button, &QPushButton::clicked, this, &LeftDock::draw_location_table);
    location_menu_layout->addWidget(generate_table_button);
    QLineEdit* search_line_edit = new QLineEdit();
    search_line_edit->setFixedWidth(250);
    location_menu_layout->addWidget(search_line_edit);
    QPushButton* search_button = new QPushButton("search");
    connect(search_button, &QPushButton::clicked, this, &LeftDock::searchLocationTable);
    location_menu_layout->addWidget(search_button);
    gen_info_scroll_bar_wid_layout->addWidget(location_label, Qt::AlignLeft);
    gen_info_scroll_bar_wid_layout->addLayout(location_menu_layout);

    // Table for displaying server locations
    QTableWidget* location_table = new QTableWidget();
    gen_info_scroll_bar_wid_layout->addWidget(location_table);

    // Whois section
    QLabel* whois_label = new QLabel("<b>Whois Results<b/>");
    gen_info_scroll_bar_wid_layout->addWidget(whois_label, Qt::AlignLeft);
    QVBoxLayout* whois_layout = new QVBoxLayout();
    QPushButton* run_whois_button = new QPushButton("run whois");
    connect(run_whois_button, &QPushButton::clicked, this, &LeftDock::run_whois);
    whois_layout->addWidget(run_whois_button, Qt::AlignLeft);
    QTextBrowser* whois_text_widget = new QTextBrowser();
    whois_text_widget->setReadOnly(true);
    whois_layout->addWidget(whois_text_widget);
    gen_info_scroll_bar_wid_layout->addLayout(whois_layout);

    // Dynamic information for subdomains and URLs
    QWidget* left_dock_down_widget = new QWidget();
    QVBoxLayout* left_dock_down_widget_layout = new QVBoxLayout();
    left_dock_down_widget->setLayout(left_dock_down_widget_layout);
    left_dock_splitter->addWidget(left_dock_down_widget);

    // Layout for buttons
    QHBoxLayout* USlayout = new QHBoxLayout();
    left_dock_down_widget_layout->addLayout(USlayout);
    QPushButton* subdomainsButton = new QPushButton("SubdUrlTree");
    connect(subdomainsButton, &QPushButton::clicked, this, &LeftDock::showSbdUrlTree);
    USlayout->addWidget(subdomainsButton);
    QPushButton* urlsButton = new QPushButton("UrlsScan");
    connect(urlsButton, &QPushButton::clicked, this, &LeftDock::UrlsScan);
    USlayout->addWidget(urlsButton);

    // Tree view for subdomains and URLs
    subdomainsModel = new QStandardItemModel();
    subdomainsModel->setHorizontalHeaderLabels(QStringList() << "Subdomain:UrlsMapping");
    QTreeView* subdomainsTreeView = new QTreeView();
    subdomainsTreeView->setFont(QFont("Cascadia Code", 11));
    subdomainsTreeView->header()->setFont(QFont("Cascadia Code", 11));
    subdomainsTreeView->setModel(subdomainsModel);
    subdomainsTreeView->setAlternatingRowColors(true);
    subdomainsTreeView->setAnimated(true);
    subdomainsTreeView->setUniformRowHeights(true);
    subdomainsTreeView->setEditTriggers(QTreeView::NoEditTriggers);
    connect(subdomainsTreeView, &QTreeView::doubleClicked, this, &LeftDock::openLinkInBrowser);
    left_dock_down_widget_layout->addWidget(subdomainsTreeView);
}

LeftDock::~LeftDock() {
    delete whoisProcess;
    delete locationTable;
    delete leftDockLayout;
    delete leftDock;
}

void LeftDock::setupUI() {
    QWidget* dockWidgetContents = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(dockWidgetContents);

    layout->addWidget(searchLabel);
    layout->addWidget(searchField);
    layout->addWidget(searchButton);
    layout->addWidget(searchExactMatchCheckBox);
    layout->addWidget(locationTable);

    connect(searchButton, &QPushButton::clicked, this, &LeftDock::searchLocationTable);

    leftDock->setWidget(dockWidgetContents);
}

void LeftDock::showSbdUrlTree(){
    
}

void LeftDock::InitializeLeftDock() {
    // Initialize the dock widget with loaded data
    // loadSubdomainData();
    // draw_location_table();
}

void LeftDock::draw_location_table() {
    if (!locationTableDrawn) {
        QFile dataFile(amassDataJsonFile);
        if (dataFile.exists()) {
            locationTable->setColumnCount(6);
            locationTable->setFixedHeight(800);
            dataFile.open(QIODevice::ReadOnly | QIODevice::Text);
            QByteArray data = dataFile.readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
            QJsonObject jsonObject = jsonDoc.object();
            // Create a list to store all values from the JSON object
            QList<QVariant> allValues;
            // Iterate through the JSON object to collect all values
            for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
                allValues.append(it.value().toVariant());
            }
            // Convert the list of values to a QJsonArray if needed
            QJsonArray domainInfoDicts = allValues[0].toJsonArray();


            QStringList subdomains, managers;
            QList<QList<QVariant>> dataList;

            for (const QJsonValue& domainInfodict : domainInfoDicts) {
                QList<QVariant> domainInfoList;
                if(domainInfodict.isObject()){
                    QString domainName = domainInfodict["subdomain"].toString();
                    subdomains.append(domainName);
                    domainInfoList.append(domainName);
        

                    QString nameRecord = domainInfodict["namerecord"].toString();
                    domainInfoList.append(nameRecord);

                    QString ipsStr;
                    QJsonArray ipsArray = domainInfodict["ip"].toArray();
                    for (int i = 0; i < ipsArray.size(); ++i) {
                        ipsStr += ipsArray.at(i).toString() + (i == ipsArray.size() - 1 ? "" : "\n");
                    }
                    domainInfoList.append(ipsStr);

                    QString netblock = domainInfodict["netblock"].toString();
                    domainInfoList.append(netblock);

                    QString asn = domainInfodict["asn"].toString();
                    domainInfoList.append(asn);

                    QString manager = domainInfodict["manager"].toString();
                    managers.append(manager);
                    domainInfoList.append(manager);
                    dataList.append(domainInfoList);
                }
            }

            locationTable->setRowCount(domainInfoDicts.size());

            int longestLength = 0;
            for (const QString& domainName : subdomains) {
                if (domainName.length() > longestLength) {
                    longestLength = domainName.length();
                }
            }

            int columnWidth = 200;
            for (int columnIndex = 0; columnIndex < 6; ++columnIndex) {
                if (columnIndex != 4 && columnIndex != 1) {
                    locationTable->setColumnWidth(columnIndex, columnWidth);
                }
            }
            locationTable->setColumnWidth(4, 80);  // ASN column
            locationTable->setColumnWidth(1, 120); // name record column
            locationTable->setHorizontalHeaderLabels({"Domain Name", "Name Record", "IP(s)", "Netblock", "ASN", "Manager"});

            for (int row = 0; row < dataList.size(); ++row) {
                QVariantMap rowWidgetItemsDict;
                rowWidgetItemsDict["index"] = row;
                rowWidgetItemsDict["subdomain"] = dataList[row][0].toString();
                QList<QTableWidgetItem*> rowTableWidgetItems;

                for (int col = 0; col < dataList[row].size(); ++col) {
                    QTableWidgetItem* item = new QTableWidgetItem(dataList[row][col].toString());
                    rowTableWidgetItems.append(item);
                    locationTable->setItem(row, col, item);
                }
                rowWidgetItemsDict["WidgetItems"] = QVariant::fromValue(rowTableWidgetItems);
                locationTableItemDicts.append(rowWidgetItemsDict);
            }
            locationTableDrawn = true;
        } else {
            QMessageBox mb;
            mb.setText("It seems Amass has not been run yet. \nDo you want to run it?");
            mb.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            int ret = mb.exec();
            if (ret == QMessageBox::Ok) {
                // mainWindow->openTestTargetWindow();
                // mainWindow->testWindow->tabManager->setCurrentIndex(1);
            }
        }
    } else {
        int ret = QMessageBox::information(this, tr("Table Already Drawn"), tr("Table has already been drawn."), QMessageBox::Ok);
    }
}


void LeftDock::searchLocationTable()
{
    if (locationTableDrawn) {
        QString userSearchDomain = searchLineEdit->text().trimmed();
        if (userSearchDomain.isEmpty()) {
            QMessageBox::information(this, tr("Empty Input"), tr("Please enter a domain to search."), QMessageBox::Ok);
            return;
        }

        bool gotMatch = false;

        // Iterate over each row in the location table
        for (const auto &rowDict : locationTableItemDicts) {
            QString subdomain = rowDict["subdomain"].toString();

            if (subdomain == userSearchDomain) {
                auto item = rowDict["WidgetItems"].toList().at(0).value<QTableWidgetItem*>();
                int rowIndex = rowDict["index"].toInt();
                locationTable->scrollToItem(item);
                locationTable->selectRow(rowIndex);
                gotMatch = true;
                break; // Exit loop after finding the first match
            }
        }

        if (!gotMatch) {
            QMessageBox::information(this, "No Results Found",
                                     "No subdomain found that matches the item you searched.");
        }
    } else {
        QMessageBox::StandardButton response;
        response = QMessageBox::question(this, "Information",
                                         "The location table has not been drawn.\nDo you want to generate the table?",
                                         QMessageBox::Ok | QMessageBox::Cancel);

        if (response == QMessageBox::Ok) {
            draw_location_table();
        }
    }
}

void LeftDock::run_whois() {
    QDir dir_(projectDirPath_);
    QString domain = dir_.dirName();
    QString program = "whois";
    QStringList arguments;
    arguments << domain;

    // whoisProcess->start(program, arguments);
}

void LeftDock::handleWhoisProcessFinished() {
    QString output = whoisProcess->readAllStandardOutput();
    QMessageBox::information(nullptr, "Whois Results", output);
}

void LeftDock::handleError(QProcess::ProcessError error) {
    QMessageBox::critical(nullptr, "Whois Error", "An error occurred while running the whois command.");
}

void LeftDock::displayWhoisResults(QObject* parent, const QString& objectName)
{
    if (!whois_displaying_) {
        QString whoisResultsFilename = QDir(projectDirPath_).filePath("whois_results");
        if (objectName == "whois runner") {
            QFile file(whoisResultsFilename);
            if (file.open(QIODevice::ReadOnly)) {
                QByteArray whoisTextResults = file.readAll();
                file.close();

                if (!whoisTextResults.isEmpty()) {
                    whoisTextWidget->setBaseSize(QSize(800, 600));
                    whoisTextWidget->setText(QString::fromUtf8(whoisTextResults));
                    whois_displaying_ = true;
                } else {
                    QMessageBox::information(this, "Information",
                                             "Whois returned no information.\nCheck the domain name or the internet connection.",
                                             QMessageBox::Ok);
                }
            } else {
                QMessageBox::warning(this, "Error",
                                     "Failed to open whois results file.",
                                     QMessageBox::Ok);
            }
        } else {
            QMessageBox::information(this, "Information",
                                     "Whois has already been run.",
                                     QMessageBox::Ok);
        }
    }
}

void LeftDock::updateModel()
{
    // Clear the existing model data
    subdomainsModel->clear();

    // Set the horizontal header labels for the model
    QStringList headers;
    headers << "Subdomain:UrlsMapping";
    subdomainsModel->setHorizontalHeaderLabels(headers);

    // Iterate over the SubdomainUrlDict to populate the model
    for (auto it = SubdomainUrlDict.constBegin(); it != SubdomainUrlDict.constEnd(); ++it) {
        const QString& subdomain = it.key();
        const QStringList& urls = it.value();

        // Create a new parent item for the subdomain
        QStandardItem* parentItem = new QStandardItem(subdomain);
        subdomainsModel->appendRow(parentItem);

        // Add URL items as children of the parent item
        for (const QString& url : urls) {
            QStandardItem* urlItem = new QStandardItem(url);
            parentItem->appendRow(urlItem);
        }
    }
}

void LeftDock::hideGenInfo()
{
    // Check the state of the checkbox
    if (infoShowCheckBox->isChecked()) {
        // Hide the general information frame if the checkbox is checked
        general_information_frame->setHidden(true);
    } else {
        // Show the general information frame if the checkbox is not checked
        general_information_frame->setHidden(false);
    }
}


void LeftDock::UrlsScan()
{
    // Check if the file exists
    QFile file(SubdomainUrlDictFile);
    if (file.exists()) {
        // Open and read the file
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString jsonData = in.readAll();
            file.close();

            // Parse JSON data
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData.toUtf8());
            if (jsonDoc.isObject()) {
                QJsonObject jsonObject = jsonDoc.object();
                SubdomainUrlDict.clear();

                // Convert JSON object to QMap<QString, QStringList>
                for (auto key : jsonObject.keys()) {
                    QJsonValue value = jsonObject.value(key);
                    if (value.isArray()) {
                        QStringList urls;
                        QJsonArray jsonArray = value.toArray();
                        for (const QJsonValue& jsonValue : jsonArray) {
                            if (jsonValue.isString()) {
                                urls.append(jsonValue.toString());
                            }
                        }
                        SubdomainUrlDict.insert(key, urls);
                    }
                }
                // Emit dataChanged signal and update model
                emit subdomainsModel->dataChanged(QModelIndex(), QModelIndex());
                updateModel();
            }
        }
    } else {
        if (!UrlFetcherRunning_) {
            UrlFetcherRunning_ = true;

            // Create and configure UrlFetcher
            UrlGetter = new UrlFetcher(SubdomainUrlDict, projectDirPath_, topParent, this, topParent, mainWindow);
            connect(UrlGetter, &UrlFetcher::UrlFetcherFinished, this, &LeftDock::updateModel);
            UrlGetter->start();
        } else {
            // Show message box if another UrlFetcher is already running
            QMessageBox UrlFetcherMessageBox;
            UrlFetcherMessageBox.setWindowTitle("Information");
            UrlFetcherMessageBox.setText("There is a thread of UrlFetcher still running\nCannot open another thread");
            UrlFetcherMessageBox.setIcon(QMessageBox::Information);
            UrlFetcherMessageBox.setStandardButtons(QMessageBox::Ok);
            UrlFetcherMessageBox.exec();
        }
    }
}

void LeftDock::openLinkInBrowser(const QModelIndex& index)
{
    // Retrieve the text from the clicked item
    QString clickedLink = subdomainsModel->itemFromIndex(index)->text();

    // Emit the signal with the clicked link
    emit openLinkInBrw(clickedLink);
}

UrlFetcher::UrlFetcher(const QMap<QString, QStringList>& subdomainUrlDict,
                     const QString& workingDir,
                     QWidget* parent,
                     LeftDock* dictParent,
                     MainWindow* topParent,
                     TargetWindow* mainWindow)
    : QThread(parent),
    m_mainWindow(mainWindow),
    m_topParent(topParent),
    m_workingDir(workingDir),
    m_dictParent(dictParent),
    m_receivedSignals(0)
{
    m_topParent = topParent;
    setObjectName("UrlFetcher");
    m_subdomainsUrlDictFile = QDir(workingDir).filePath("subdomainsUrlDict.json");
    connect(m_topParent->socketIpc, &SocketIPC::processFinishedExecution, this, &UrlFetcher::processFinishedExecutionSlot);
    // m_mainWindow->threads().append(this);
    setTerminationEnabled(true);
    // emit m_topParent->ThreadStarted(m_mainWindow, objectName());
    connect(this, &QObject::destroyed, this, &UrlFetcher::closeThread);
}

UrlFetcher::~UrlFetcher()
{
    closeThread();
}

void UrlFetcher::closeThread()
{
    emit m_topParent->socketIpc->processFinishedExecution(m_mainWindow, objectName());
}

void UrlFetcher::processFinishedExecutionSlot(QWidget* windowInstance, const QString& tool)
{
    Q_UNUSED(windowInstance);
    Q_UNUSED(tool);
    m_receivedSignals++;
}

void UrlFetcher::run()
{
    int i = 0;
    QSet<QString> successfulSubdomains;
    QSet<QString> errorSubdomains;

    qInfo() << "UrlFetcher to work on" << m_subdomainUrlDict.keys().size() << "subdomains";

    for (const QString& subdomain : m_subdomainUrlDict.keys()) {
        QString cleanSubdomain = subdomain.trimmed();
        qInfo() << "Running atomGuiGetUrls on:" << cleanSubdomain << ", Number:" << i;
        QVariant result = QVariant::fromValue(atomGuiGetUrls(cleanSubdomain.toStdString(), m_workingDir.toStdString(), m_dictParent, m_topParent, m_mainWindow));

        if (result.canConvert<QList<QVariant>>()) {
            QList<QVariant> resultList = result.toList();
            qInfo() << "Waiting for processes to close";

            while (true) {
                if (m_receivedSignals == resultList.size()) {
                    m_receivedSignals = 0;
                    break;
                }
            }

        } else if (result.canConvert<QList<QString>>()) {
            QList<QString> urlList = result.toStringList();
            m_subdomainsUrlDict_[cleanSubdomain] = urlList;
            m_dictParent->SubdomainUrlDict[cleanSubdomain] = urlList;
            // emit m_dictParent->modelUpdater()->dictChanged();
        }

        i++;

        if (QThread::idealThreadCount() <= 4) {
            sleep(5);
        } else if (QThread::idealThreadCount() > 4 && QThread::idealThreadCount() <= 8) {
            sleep(2);
        }
    }

    emit UrlFetcherFinished();
    qInfo() << "Worked on" << m_subdomainUrlDict.keys().size() << "subdomains";
    qInfo() << successfulSubdomains.size() << "Successful";
    qInfo() << errorSubdomains.size() << "Failed";
    qInfo() << "Failed are" << errorSubdomains.values().toList();

    writeSubdomainsUrlDictToFile();
    emit m_topParent->socketIpc->processFinishedExecution(m_mainWindow, objectName());
}

std::vector<std::string> UrlFetcher::atomGuiGetUrls(const std::string &subdomain, const std::string &workingDir, QWidget *parent,
                                        MainWindow *top_parent, TargetWindow *mainWindow) {
    std::string subdomain_cleaned = subdomain;
    subdomain_cleaned.erase(std::remove(subdomain_cleaned.begin(), subdomain_cleaned.end(), '\n'), subdomain_cleaned.end());
    subdomain_cleaned.erase(std::remove_if(subdomain_cleaned.begin(), subdomain_cleaned.end(), ::isspace), subdomain_cleaned.end());

    std::set<std::string> UrlsList_;
    std::vector<int> pids;
    std::string pathName = workingDir + "/getAllUrls_" + subdomain_cleaned + "Subdomains.txt";

    if (fs::exists(pathName)) {
        std::ifstream file(pathName);
        std::string url;
        while (std::getline(file, url)) {
            UrlsList_.insert(url);
        }
    } else {
        ToolsRunner ToolsRunner_(QString::fromStdString(workingDir), QString::fromStdString(subdomain_cleaned), "getAllUrls", parent, top_parent, mainWindow);
        int g_pid = ToolsRunner_.runUrlToolsOnSd();
        pids.push_back(g_pid);
    }

    std::string pathName0 = workingDir + "/linkFinder_" + subdomain_cleaned + "Subdomains.txt";

    if (!fs::exists(pathName0)) {
        ToolsRunner ToolsRunner_(QString::fromStdString(workingDir), QString::fromStdString(subdomain_cleaned), "LinkFinder", parent, top_parent, mainWindow);
        int l_pid = ToolsRunner_.runUrlToolsOnSd();
        pids.push_back(l_pid);
    }

    try {
        std::vector<std::string> atomSubdUrls = getAtomSubdUrls(subdomain_cleaned, workingDir);
        UrlsList_.insert(atomSubdUrls.begin(), atomSubdUrls.end());
    } catch (const std::exception &e) {
        ToolsRunner ToolsRunner_(QString::fromStdString(workingDir), QString::fromStdString(subdomain_cleaned), "Atom", parent, top_parent, mainWindow);
        int a_pid = ToolsRunner_.runUrlToolsOnSd();
        pids.push_back(a_pid);
    }

    if (UrlsList_.empty()) {
        // return std::vector<std::string>(pids.begin(), pids.end());
    } else {
        return std::vector<std::string>(UrlsList_.begin(), UrlsList_.end());
    }
}

void UrlFetcher::writeSubdomainsUrlDictToFile()
{
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(QVariant::fromValue(m_dictParent->SubdomainUrlDict));
    QFile file(m_subdomainsUrlDictFile);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonDoc.toJson());
        file.close();
    }
}
