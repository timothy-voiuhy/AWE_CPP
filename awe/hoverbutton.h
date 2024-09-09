#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QPushButton>
#include <QToolTip>
#include <QEnterEvent>

class HoverButton : public QPushButton
{
    Q_OBJECT

public:
    HoverButton(const QString &text, const QString &tooltipText, QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *event);

private:
    QString tooltipText_;
};

#endif // HOVERBUTTON_H
