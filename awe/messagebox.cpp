#include "messagebox.h"

MessageBox::MessageBox(const QString &windowTitle,const QString &text,IconType icon,ButtonType button,const QList<ButtonType> &buttons,QWidget *parent)
    : QMessageBox(parent), windowTitle_(windowTitle),text_(text),icon_(icon),button_(button),buttons_(buttons)
{
    setupMessageBox();
}

void MessageBox::setupMessageBox()
{
    setWindowTitle(windowTitle_);
    setText(text_);

    switch (icon_) {
        case Information:
            setIcon(QMessageBox::Information);
            break;
        case Warning:
            setIcon(QMessageBox::Warning);
            break;
        case Critical:
            setIcon(QMessageBox::Critical);
            break;
        case Question:
            setIcon(QMessageBox::Question);
            break;
    }

    switch (button_) {
        case Ok:
            setStandardButtons(QMessageBox::Ok);
            break;
        case Cancel:
            setStandardButtons(QMessageBox::Cancel);
            break;
        default:
            // Do nothing for NoButton
            break;
    }

    for (ButtonType buttonType : buttons_) {
        switch (buttonType) {
            case Ok:
                addButton(QMessageBox::Ok);
                break;
            case Cancel:
                addButton(QMessageBox::Cancel);
                break;
            case Save:
                addButton(QMessageBox::Save);
                break;
            case Open:
                addButton(QMessageBox::Open);
                break;
            case Yes:
                addButton(QMessageBox::Yes);
                break;
            case No:
                addButton(QMessageBox::No);
                break;
            case Abort:
                addButton(QMessageBox::Abort);
                break;
            case Retry:
                addButton(QMessageBox::Retry);
                break;
            case Ignore:
                addButton(QMessageBox::Ignore);
                break;
            case Close:
                addButton(QMessageBox::Close);
                break;
            case Discard:
                addButton(QMessageBox::Discard);
                break;
            case Help:
                addButton(QMessageBox::Help);
                break;
            case Apply:
                addButton(QMessageBox::Apply);
                break;
            case Reset:
                addButton(QMessageBox::Reset);
                break;
            default:
                // Handle other cases or ignore
                break;
        }
    }
}
