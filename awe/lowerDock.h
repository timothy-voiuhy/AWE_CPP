#ifndef QTERMINAL_H
#define QTERMINAL_H

#include <QMainWindow>
#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QKeyEvent>
#include <QString>
#include <QProcess>
#include <QtCore/QRegularExpression>
#include <QObject>

class Qterminal : public QTextEdit {
    Q_OBJECT

public:
    Qterminal(QWidget* parent = nullptr);
    QString getCurrentWorkingDir();
    void keyPressEvent(QKeyEvent* e) override;
    void placeCursorAtEnd();

private:
    QString currentWorkingDir;
    QString terminalDefaultText;
    QRegularExpression plainStringRegexPattern;
};

class LowerDock : public QWidget {
public:
    LowerDock(QMainWindow* MainWindow, const QString& projectDirPath);
    QDockWidget* InitializeLowerDock();

private:
    QMainWindow* main_window;
    QString projectDirPath;
    QDockWidget* lowerDock;
    QWidget* lowerDockWidget;
    QVBoxLayout* lowerDockLayout;
    QTextEdit* terminalTextEdit;
};

#endif // QTERMINAL_H
