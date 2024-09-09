#ifndef TOOLSRUNNER_H
#define TOOLSRUNNER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QList>

#include "targetWindow.h"

class MainWindow;  // Forward declaration of MainWindow class

class ToolsRunner : public QObject
{
    Q_OBJECT

public:
    explicit ToolsRunner(const QString& workingDir,
                         const QString& subdomain,
                         const QString& tool = QString(),
                         QWidget* parent = nullptr,
                         MainWindow* topParent = nullptr,
                         TargetWindow* mainWindow = nullptr);

    qint64 runUrlToolsOnSd();

private:
    TargetWindow* mainWindow;
    MainWindow* topParent;
    qint64 getAllUrlsRunnerPid;
    qint64 linkFinderRunnerPid;
    qint64 atomRunnerPid;
    QString workingDir;
    QString subdomain;
    QString tool;
    QObject* parent;

    qint64 startTool(const QString& toolName, QProcess*& process);
};

#endif // TOOLSRUNNER_H
