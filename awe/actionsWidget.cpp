#include "actionsWidget.h"
#include "mainwindow.h"
#include <QTextEdit>

ActionsWidget::ActionsWidget(QWidget *topParent, QTextEdit *responseEditor)
    : QWidget(topParent), responseEditor(responseEditor)
{
    actionWidgetLayout = new QVBoxLayout(this);
    renderButton = new QPushButton("render", this);
    renderButton->setToolTip("Render the page in a browser");
    renderButton->setMaximumWidth(80);
    connect(renderButton, &QPushButton::clicked, this, &ActionsWidget::renderPage);
    actionWidgetLayout->addWidget(renderButton);

    encodingWidget = new EncodingWidget(topParent);
    actionWidgetLayout->addWidget(encodingWidget, Qt::AlignTop);
}

void ActionsWidget::renderPage()
{
    BrowserWindow *browserWindow = new BrowserWindow();
    QStringList text = responseEditor->toPlainText().split("\n\n");
    QString html;

    for (int i = 1; i < text.size(); ++i)
    {
        html += text[i];
        if (i != text.size() - 1)
        {
            html += "\n\n";
        }
    }

    browserWindow->Page->setHtml(html);
    // Add browserWindow to your tab manager here
    topParent->tabManager->addTab(browserWindow, "render");
}
