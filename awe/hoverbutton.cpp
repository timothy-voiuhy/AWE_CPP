#include "hoverbutton.h"

HoverButton::HoverButton(const QString &text, const QString &tooltipText, QWidget *parent)
    : QPushButton(text, parent),
      tooltipText_(tooltipText)
{
    setToolTip(tooltipText_);
    // Uncomment the following line if you want the button to be flat
    // setFlat(true);
}

void HoverButton::enterEvent(QEnterEvent *event)
{
    QToolTip::showText(mapToGlobal(rect().bottomLeft()), tooltipText_);
    QPushButton::enterEvent(event);
}
