#include "reqrestexteditor.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

ReqResTextEditor::ReqResTextEditor(QWidget* parent)
    : QTextEdit(parent)
{
    // Constructor implementation if needed
}

void ReqResTextEditor::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu* menu = createStandardContextMenu();

    QAction* sendToRepeaterAction = new QAction(tr("send to repeater"), this);
    connect(sendToRepeaterAction, &QAction::triggered, this, &ReqResTextEditor::sendRequestToRepeater);
    menu->addAction(sendToRepeaterAction);

    QAction* sendToDecoderAction = new QAction(tr("send to decoder"), this);
    connect(sendToDecoderAction, &QAction::triggered, this, &ReqResTextEditor::sendHighlightedToDecoder);
    menu->addAction(sendToDecoderAction);

    menu->exec(event->globalPos());
}

void ReqResTextEditor::sendHighlightedToDecoder()
{
    // Implement sending highlighted text to decoder
    // Example:
    // QString highlightedText = textCursor().selectedText();
    // emit sendToDecoderSignal(highlightedText);
}

void ReqResTextEditor::sendRequestToRepeater()
{
    emit sendToRepeaterSignal(toPlainText());
}
