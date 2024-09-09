#include "texteditor.h"
#include <QTextEdit>
#include <QKeyEvent>
#include <QTextBlock>

TextEditor::TextEditor(QWidget* parent)
    : QTextEdit(parent)
{
    setTabChangesFocus(true);
    setTabStopDistance(40);
    // Qt's QTextEdit does not have a direct equivalent for auto-indent,
    // so you might need to handle it manually.
    // Note: setFontWeight does not exist in QTextEdit, it would be handled through QFont.
}

void TextEditor::keyPressEvent(QKeyEvent* event)
{
    if (autoIndent && event->key() == Qt::Key_Return) {
        QTextCursor cursor = textCursor();
        QTextBlock block = cursor.block();
        QString text = block.text();
        int cursorPosition = cursor.positionInBlock();
        int leadingSpaces = text.length() - text.trimmed().length();

        if (cursorPosition > 0 && text.endsWith(":", Qt::CaseInsensitive) || text.endsWith("{")) {
            int indent = leadingSpaces + tabStopDistance() / fontMetrics().averageCharWidth();
            QTextEdit::keyPressEvent(event);
            cursor.insertText(QString(" ").repeated(indent));
            return;
        }
    } else if (autoIndent && event->key() == Qt::Key_Tab) {
        QTextCursor cursor = textCursor();
        QTextBlock block = cursor.block();
        QString text = block.text();
        int cursorPosition = cursor.positionInBlock();
        int leadingSpaces = text.length() - text.trimmed().length();

        if (cursorPosition > 0) {
            int indent = leadingSpaces + tabStopDistance() / fontMetrics().averageCharWidth();
            cursor.insertText(QString(" ").repeated(indent));
            return;
        }
    }

    QTextEdit::keyPressEvent(event);
}

void TextEditor::setAutoIndent(bool enabled)
{
    autoIndent = enabled;
}
