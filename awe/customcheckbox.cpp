#include "customcheckbox.h"

CustomCheckBox::CustomCheckBox(const QString &tooltipText, QWidget *parent)
    : QCheckBox(parent),
      tooltipText_(tooltipText)
{
    setToolTip(tooltipText_);
}

void CustomCheckBox::enterEvent(QEnterEvent *event)
{
    QToolTip::showText(mapToGlobal(rect().bottomRight()), tooltipText_);
    QCheckBox::enterEvent(event);
}
