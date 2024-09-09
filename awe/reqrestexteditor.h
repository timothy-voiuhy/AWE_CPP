#ifndef REQRESTEXTEDITOR_H
#define REQRESTEXTEDITOR_H

#include <QTextEdit>
#include <QMenu>
#include <QAction>
#include <QObject>

class ReqResTextEditor : public QTextEdit
{
    Q_OBJECT

public:
    ReqResTextEditor(QWidget* parent = nullptr);

signals:
    void sendToRepeaterSignal(const QString& text);
    void sendToDecoderSignal(const QString& text);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    void sendHighlightedToDecoder();
    void sendRequestToRepeater();
};

#endif // REQRESTEXTEDITOR_H
