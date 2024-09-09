#ifndef REPEATERWINDOW_H
#define REPEATERWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QPushButton>
#include <QFrame>
#include <QTextEdit>
#include <QQueue>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "actionsWidget.h"
#include "guiproxyclient.h"
#include "messagebox.h"
#include "RepeaterReqResTextEditor.h"

class RepeaterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RepeaterWindow(QWidget *parent = nullptr);

private slots:
    void addReqResInstanceTabManager(const QString &request = QString());
    void changeTabAttributes();
    void sendRepReqToProxy();
    void updateResponseEditor();

private:
    QWidget *repeaterMainWidget;
    QVBoxLayout *repeaterMainWidgetLayout;
    QSplitter *repeaterSplitter;
    QTabWidget *repeaterTabManager;
    QList<QMap<QString, QWidget*>> repeaterTabs;
    QSplitter *instanceRepeaterSplitter;
    QTabWidget *req_res_tabManager;
    QFrame *requestsEditorFrame;
    QVBoxLayout *requestsEditorLayout;
    RepeaterReqResTextEditor *requestsEditor;
    QPushButton *repeaterSendReqButton;
    RepeaterReqResTextEditor *responseEditor;
    int tabIndex;
    bool firstTabPresent;
    QQueue<QString> responseQueue;
    QString responseDir;
    QString responseFile;
    ActionsWidget *actionsWidget;
    QWidget *parentWidget;

    void initializeResponseDir();
    void createResponseFile();
};

#endif // REPEATERWINDOW_H
