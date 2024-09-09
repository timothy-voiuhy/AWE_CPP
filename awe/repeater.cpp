#include "repeater.h"
#include "actionsWidget.h"

#include <QTextDocument>
#include <QFile>
#include <QTextStream>
#include <QDebug>

RepeaterWindow::RepeaterWindow(QWidget *parent)
    : QMainWindow(parent), parentWidget(parent), firstTabPresent(false), tabIndex(0)
{
    repeaterMainWidget = new QWidget(this);
    setCentralWidget(repeaterMainWidget);
    repeaterMainWidgetLayout = new QVBoxLayout(repeaterMainWidget);
    
    repeaterSplitter = new QSplitter(repeaterMainWidget);
    repeaterMainWidgetLayout->addWidget(repeaterSplitter);

    repeaterTabManager = new QTabWidget(repeaterSplitter);
    connect(repeaterTabManager, &QTabWidget::currentChanged, this, &RepeaterWindow::changeTabAttributes);
    repeaterSplitter->addWidget(repeaterTabManager);

    responseDir = QDir::homePath() + "/tmp/"; // Use a suitable path
    initializeResponseDir();
    createResponseFile();

    actionsWidget = new ActionsWidget(parentWidget, responseEditor);
    actionsWidget->setMaximumWidth(400);
    repeaterSplitter->addWidget(actionsWidget);
}

void RepeaterWindow::initializeResponseDir()
{
    QDir dir(responseDir);
    if (!dir.exists()) {
        dir.mkpath(responseDir);
    }
}

void RepeaterWindow::createResponseFile()
{
    responseFile = responseDir + "/response.txt";
    QFile file(responseFile);
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly)) {
            file.close();
        }
    }
}

void RepeaterWindow::addReqResInstanceTabManager(const QString &request)
{
    instanceRepeaterSplitter = new QSplitter();
    repeaterTabManager->addTab(instanceRepeaterSplitter, "new");

    req_res_tabManager = new QTabWidget();
    instanceRepeaterSplitter->addWidget(req_res_tabManager);

    requestsEditorFrame = new QFrame();
    requestsEditorFrame->setBaseSize(QSize(650, 650));
    requestsEditorFrame->setMinimumWidth(750);
    requestsEditorLayout = new QVBoxLayout(requestsEditorFrame);

    requestsEditor = new RepeaterReqResTextEditor();
    if (!request.isEmpty()) {
        requestsEditor->setPlainText(request);
    }
    // SyntaxHighlighter highlighter(requestsEditor->document()); // Assuming highlighter is defined elsewhere
    requestsEditorLayout->addWidget(requestsEditor);

    repeaterSendReqButton = new QPushButton("send");
    connect(repeaterSendReqButton, &QPushButton::clicked, this, &RepeaterWindow::sendRepReqToProxy);
    requestsEditorLayout->addWidget(repeaterSendReqButton);

    responseEditor = new RepeaterReqResTextEditor();
    // SyntaxHighlighter highlighterResponse(responseEditor->document()); // Assuming highlighter is defined elsewhere
    req_res_tabManager->addTab(requestsEditorFrame, "request");
    req_res_tabManager->addTab(responseEditor, "response");

    QMap<QString, QWidget*> tabAttributes;
    tabAttributes["instanceRepeaterSplitter"] = instanceRepeaterSplitter;
    tabAttributes["req_res_tabManager"] = req_res_tabManager;
    tabAttributes["requestsEditorFrame"] = requestsEditorFrame;
    // tabAttributes["requestsEditorLayout"] = requestsEditorLayout;
    tabAttributes["requestsEditor"] = requestsEditor;
    tabAttributes["repeaterSendReqButton"] = repeaterSendReqButton;
    tabAttributes["responseEditor"] = responseEditor;

    repeaterTabs.append(tabAttributes);
    repeaterTabManager->setCurrentIndex(repeaterTabManager->indexOf(instanceRepeaterSplitter));
    firstTabPresent = true;

    if (repeaterTabManager->currentIndex() == 0) {
        emit repeaterTabManager->currentChanged(0);
    }
}

void RepeaterWindow::changeTabAttributes()
{
    int curIndex = repeaterTabManager->currentIndex();
    if (firstTabPresent) {
        instanceRepeaterSplitter = qobject_cast<QSplitter*>(repeaterTabs[curIndex]["instanceRepeaterSplitter"]);
        req_res_tabManager = qobject_cast<QTabWidget*>(repeaterTabs[curIndex]["req_res_tabManager"]);
        requestsEditorFrame = qobject_cast<QFrame*>(repeaterTabs[curIndex]["requestsEditorFrame"]);
        requestsEditorLayout = qobject_cast<QVBoxLayout*>(repeaterTabs[curIndex]["requestsEditorLayout"]);
        requestsEditor = qobject_cast<RepeaterReqResTextEditor*>(repeaterTabs[curIndex]["requestsEditor"]);
        repeaterSendReqButton = qobject_cast<QPushButton*>(repeaterTabs[curIndex]["repeaterSendReqButton"]);
        responseEditor = qobject_cast<RepeaterReqResTextEditor*>(repeaterTabs[curIndex]["responseEditor"]);
    }
}

void RepeaterWindow::sendRepReqToProxy()
{
    try {
        auto *guiProxyClient = new GuiProxyClient(requestsEditor->toPlainText(), parentWidget->property("proxy_port").toInt());
        connect(guiProxyClient, &GuiProxyClient::finished, this, &RepeaterWindow::updateResponseEditor);
        guiProxyClient->start();
        qDebug() << "Gui proxy client successfully reached out to proxy";
    }
    catch (const std::exception &e) {
        qCritical() << "Proxy Client failed to reach out to proxy" << e.what();
    }
}

void RepeaterWindow::updateResponseEditor()
{
    responseEditor->clear();
    QFile file(responseFile);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString response = stream.readAll();
        if (!response.isEmpty()) {
            responseEditor->setPlainText(response);
        } else {
            MessageBox noDataMessageBox("No data",
                                        "No data has been returned from the proxy, it seems either the proxy is off or you have no internet connection",
                                        MessageBox::IconType::Information,
                                        MessageBox::ButtonType::Ok);
            noDataMessageBox.exec();
        }
        file.close();
    }
}
