#include "encodingwidget.h"
// #include <QBase64>
// #include <QtAlgorithms>
#include <QUrl>

EncodingWidget::EncodingWidget(QWidget *topParent)
    : QWidget(topParent), decode_option("base64")
{
    encodingWidgetLayout = new QVBoxLayout(this);
    upperLayout = new QHBoxLayout();
    encodingWidgetLayout->addLayout(upperLayout);

    decodeButton = new QPushButton("decode", this);
    decodeButton->setToolTip("Decode the chosen text using an appropriate decoding method");
    connect(decodeButton, &QPushButton::clicked, this, &EncodingWidget::decodeText);
    upperLayout->addWidget(decodeButton);

    encodeButton = new QPushButton("encode", this);
    encodeButton->setToolTip("Encode the text with the appropriate encoding type");
    connect(encodeButton, &QPushButton::clicked, this, &EncodingWidget::encodeText);
    upperLayout->addWidget(encodeButton);

    dropDownMenu = new QComboBox(this);
    addDecodeOptions();
    upperLayout->addWidget(dropDownMenu, Qt::AlignLeft);

    textsFormLayout = new QFormLayout();

    textBox = new QTextEdit(this);
    textBox->setWindowTitle("encoded text");
    textsFormLayout->addRow("i:", textBox);

    resultTextBox = new QTextEdit(this);
    resultTextBox->setWindowTitle("decoded text");
    textsFormLayout->addRow("o:", resultTextBox);

    encodingWidgetLayout->addLayout(textsFormLayout);
}

void EncodingWidget::encodeText()
{
    QString text = textBox->toPlainText();
    QByteArray encodedText;

    if (decode_option == "base64")
    {
        encodedText = QByteArray::fromBase64(text.toUtf8());
    }
    else if (decode_option == "utf-8")
    {
        encodedText = text.toUtf8();
    }
    else if (decode_option == "utf-32")
    {
        encodedText = text.toUtf8(); // Handle UTF-32 if needed
    }

    resultTextBox->clear();
    resultTextBox->setText(QString(encodedText));
}

void EncodingWidget::decodeText()
{
    QByteArray text = textBox->toPlainText().toUtf8();
    QByteArray decodedText;

    if (decode_option == "base64")
    {
        decodedText = decodeBase64(text);
    }
    else if (decode_option == "utf-8")
    {
        decodedText = decodeUtf8(text);
    }
    else if (decode_option == "url")
    {
        decodedText = UrlDecode(text);
    }

    resultTextBox->clear();
    resultTextBox->setText(QString(decodedText));
}

void EncodingWidget::addDecodeOptions()
{
    dropDownMenu->addItems({"base64", "url", "utf-8", "utf-32"});
    connect(dropDownMenu, &QComboBox::currentTextChanged, this, &EncodingWidget::setDecodeOption);
}

void EncodingWidget::setDecodeOption(const QString &item)
{
    decode_option = item;
}

QByteArray EncodingWidget::decodeBase64(const QByteArray &text)
{
    return QByteArray::fromBase64(text);
}

QByteArray EncodingWidget::decodeUtf8(const QByteArray &text)
{
    return text; // Assuming the text is already UTF-8 encoded
}

QByteArray EncodingWidget::UrlDecode(const QByteArray &text)
{
    return QByteArray::fromStdString(QUrl::fromPercentEncoding(text).toStdString());

}
