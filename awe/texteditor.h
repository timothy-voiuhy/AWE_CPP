#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QTextEdit>
#include <QKeyEvent>

class TextEditor : public QTextEdit
{
    Q_OBJECT

public:
    explicit TextEditor(QWidget* parent = nullptr);

    void setAutoIndent(bool enabled);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    bool autoIndent = false;
};

#endif // TEXTEDITOR_H
