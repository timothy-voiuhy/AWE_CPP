#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>
#include <QString>
#include <QList>

class MessageBox : public QMessageBox
{
    Q_OBJECT

public:
    enum IconType {
        Information,
        Warning,
        Critical,
        Question
    };

    enum ButtonType {
        NoButton,
        Ok,
        Cancel,
        Save,
        Open,
        Yes,
        No,
        Abort,
        Retry,
        Ignore,
        Close,
        Discard,
        Help,
        Apply,
        Reset
    };

    MessageBox(const QString &windowTitle = QString(),
               const QString &text = QString(),
               IconType icon = Information,
               ButtonType button = NoButton,
               const QList<ButtonType> &buttons = QList<ButtonType>(),
               QWidget *parent = nullptr);

private:
    void setupMessageBox();
    
    QString windowTitle_;
    QString text_;
    IconType icon_;
    ButtonType button_;
    QList<ButtonType> buttons_;
};

#endif // MESSAGEBOX_H
