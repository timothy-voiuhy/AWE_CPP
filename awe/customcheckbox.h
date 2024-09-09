#ifndef CUSTOMCHECKBOX_H
#define CUSTOMCHECKBOX_H

#include <QCheckBox>
#include <QToolTip>
#include <QEnterEvent>

class CustomCheckBox : public QCheckBox
{
    Q_OBJECT

public:
    explicit CustomCheckBox(const QString &tooltipText, QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *event);

private:
    QString tooltipText_;
};

#endif // CUSTOMCHECKBOX_H
