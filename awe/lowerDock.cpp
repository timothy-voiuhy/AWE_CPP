#include "lowerDock.h"
#include <QFont>
#include <QTextCursor>
#include <QDebug>
#include <QDir>
#include <typeinfo>

Qterminal::Qterminal(QWidget* parent) : QTextEdit(parent) {
    this->placeCursorAtEnd();
    QFont font = this->font();
    font.setPointSize(font.pointSize() + 1);
    this->setFont(font);

    currentWorkingDir = this->getCurrentWorkingDir().replace("\n", "").trimmed();
    terminalDefaultText = QString(">>>[%1]").arg(currentWorkingDir);
    this->setPlainText(terminalDefaultText);
    this->setAcceptRichText(false);
    plainStringRegexPattern = QRegularExpression("[a-zA-Z]+");
}

QString Qterminal::getCurrentWorkingDir() {
    QProcess process;
    process.start("pwd");
    process.waitForFinished();
    QString pwd = process.readAllStandardOutput().simplified();
    return pwd;
}

void Qterminal::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        QString lines = this->toPlainText();
        QStringList Lines = lines.split("\n");
        QString command = Lines.last().replace(terminalDefaultText, "").trimmed();
        
        if (!command.isEmpty()) {
            if (command == "clear") {
                this->clear();
                this->setPlainText(terminalDefaultText);
                this->placeCursorAtEnd();
            } else if (command.startsWith("cd ")) {
                QString precurrentWorkingDir = "";
                QString UserSetWorkingDir = command.split(" ")[1];
                if (UserSetWorkingDir.startsWith("/")) {
                    precurrentWorkingDir = currentWorkingDir + UserSetWorkingDir + "/";
                } else if (UserSetWorkingDir == "..") {
                    if (currentWorkingDir != "/") {
                        QStringList currentWorkingDirr = currentWorkingDir.split("/");
                        currentWorkingDirr.removeAll("");
                        currentWorkingDirr.removeLast();
                        precurrentWorkingDir = "/" + currentWorkingDirr.join("/") + "/";
                    }
                } else if (UserSetWorkingDir == ".") {
                    precurrentWorkingDir = currentWorkingDir;
                } else if (typeid(plainStringRegexPattern.match(UserSetWorkingDir)) == typeid(QString)) {
                    UserSetWorkingDir = "/" + UserSetWorkingDir + "/";
                    precurrentWorkingDir = currentWorkingDir + UserSetWorkingDir;
                }

                if (QDir(precurrentWorkingDir).exists()) {
                    currentWorkingDir = precurrentWorkingDir;
                }
                this->setPlainText(lines + "\n" + terminalDefaultText);
                this->placeCursorAtEnd();
            } else {
                QProcess commandResult;
                commandResult.setWorkingDirectory(currentWorkingDir);
                commandResult.start(command);
                commandResult.waitForFinished();
                QString commandOutput = commandResult.readAllStandardOutput();
                QString commandError = commandResult.readAllStandardError();

                QString totalLines = lines + "\n" + (commandOutput.isEmpty() ? commandError : commandOutput) + "\n" + terminalDefaultText;
                this->setPlainText(totalLines);
                this->placeCursorAtEnd();
            }
        }
    } else if (e->key() == Qt::Key_Backspace) {
        QString current_text = this->toPlainText();
        if (current_text.endsWith(terminalDefaultText[0])) {
            return;
        }
        QTextEdit::keyPressEvent(e);
    } else {
        QTextEdit::keyPressEvent(e);
    }
}

void Qterminal::placeCursorAtEnd() {
    QTextCursor cursor = this->textCursor();
    cursor.setPosition(this->toPlainText().length());
    this->setTextCursor(cursor);
}

LowerDock::LowerDock(QMainWindow* MainWindow, const QString& projectDirPath)
    : main_window(MainWindow), projectDirPath(projectDirPath) {}

QDockWidget* LowerDock::InitializeLowerDock() {
    lowerDock = new QDockWidget("Terminal");
    lowerDockWidget = new QWidget();
    lowerDockLayout = new QVBoxLayout();
    terminalTextEdit = new Qterminal();
    
    lowerDockLayout->addWidget(terminalTextEdit);
    lowerDockWidget->setLayout(lowerDockLayout);
    lowerDock->setWidget(lowerDockWidget);
    main_window->addDockWidget(Qt::BottomDockWidgetArea, lowerDock);

    return lowerDock;
}
