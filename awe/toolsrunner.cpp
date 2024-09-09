#include "toolsrunner.h"
#include "mainwindow.h"
#include <QDebug>
#include <QProcess>
#include <QString>

ToolsRunner::ToolsRunner(const QString& workingDir,
                         const QString& subdomain,
                         const QString& tool,
                         QWidget* parent,
                         MainWindow* topParent,
                         TargetWindow* mainWindow)
    : QObject(parent),
      mainWindow(mainWindow),
      topParent(topParent),
      getAllUrlsRunnerPid(0),
      linkFinderRunnerPid(0),
      atomRunnerPid(0),
      workingDir(workingDir),
      subdomain(subdomain),
      tool(tool),
      parent(parent)
{
}

qint64 ToolsRunner::startTool(const QString& toolName, QProcess*& process)
{
    process = new QProcess(this);
    QString program;

    if (toolName == "getAllUrls") {
        program = "path_to_getAllUrls_executable";
    } else if (toolName == "LinkFinder") {
        program = "path_to_LinkFinder_executable";
    } else if (toolName == "Atom") {
        program = "path_to_Atom_executable";
    } else {
        return 0;
    }

    process->setWorkingDirectory(workingDir);
    process->start(program);

    if (!process->waitForStarted()) {
        qDebug() << "Failed to start" << toolName;
        return 0;
    }

    return process->processId();
}

qint64 ToolsRunner::runUrlToolsOnSd()
{
    QProcess* process = nullptr;
    qint64 pid = 0;

    if (!tool.isEmpty()) {
        qDebug() << "Running tool" << tool;
        pid = startTool(tool, process);
        if (pid == 0) {
            qDebug() << "Error running tool" << tool;
        }
        return pid;
    }

    qint64 g_pid = startTool("getAllUrls", process);
    qint64 l_pid = startTool("LinkFinder", process);
    qint64 a_pid = startTool("Atom", process);

    return g_pid; // Or handle multiple PIDs as needed
}
