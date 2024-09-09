#ifndef ACTIONSWIDGET_H
#define ACTIONSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "encodingwidget.h"
#include "browserWindow.h"

class MainWindow;

class ActionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionsWidget(QWidget *topParent, QTextEdit *responseEditor = nullptr);

private slots:
    void renderPage();

private:
    QTextEdit *responseEditor;
    MainWindow *topParent;
    QVBoxLayout *actionWidgetLayout;
    QPushButton *renderButton;
    EncodingWidget *encodingWidget;
};

#endif // ACTIONSWIDGET_H
