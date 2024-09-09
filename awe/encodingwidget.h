#ifndef ENCODINGWIDGET_H
#define ENCODINGWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QToolTip>
#include <QByteArray>

class EncodingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EncodingWidget(QWidget *topParent = nullptr);

private slots:
    void encodeText();
    void decodeText();
    void setDecodeOption(const QString &item);

private:
    void addDecodeOptions();
    QByteArray decodeBase64(const QByteArray &text);
    QByteArray decodeUtf8(const QByteArray &text);
    QByteArray UrlDecode(const QByteArray &text);

    QVBoxLayout *encodingWidgetLayout;
    QHBoxLayout *upperLayout;
    QPushButton *decodeButton;
    QPushButton *encodeButton;
    QComboBox *dropDownMenu;
    QFormLayout *textsFormLayout;
    QTextEdit *textBox;
    QTextEdit *resultTextBox;
    QString decode_option;
};

#endif // ENCODINGWIDGET_H
