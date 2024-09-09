#include "syntaxHighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::yellow);
    // keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywords = {
        "class", "def", "if", "else", "elif", "for", "while", "try",
        "except", "finally", "import", "from", "as", "return", "raise"
    };

    for (const QString& keyword : keywords) {
        highlightRules.append(qMakePair(QRegularExpression("\\b" + keyword + "\\b"), keywordFormat));
    }

    QStringList parenthesesWords = { "\\(", "\\)", "\\{", "\\}", "\\[", "\\]" };

    QTextCharFormat parenthesesFormat;
    parenthesesFormat.setForeground(Qt::yellow);
    // parenthesesFormat.setFontWeight(QFont::Bold);

    for (const QString& parenthesis : parenthesesWords) {
        highlightRules.append(qMakePair(QRegularExpression(parenthesis), parenthesesFormat));
    }

    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::magenta);
    // commentFormat.setFontWeight(QFont::Bold);
    highlightRules.append(qMakePair(QRegularExpression("#.*"), commentFormat));

    QTextCharFormat selfFormat;
    selfFormat.setForeground(Qt::red);
    // selfFormat.setFontWeight(QFont::Bold);
    highlightRules.append(qMakePair(QRegularExpression("self(?=\\.)"), selfFormat));

    QTextCharFormat funcDeclFormat;
    funcDeclFormat.setForeground(Qt::magenta);
    // funcDeclFormat.setFontWeight(QFont::Bold);
    highlightRules.append(qMakePair(QRegularExpression("(?<=def\\s)\\w+(?=\\()"), funcDeclFormat));

    QTextCharFormat stringFormat;
    stringFormat.setForeground(Qt::cyan);
    // stringFormat.setFontWeight(QFont::Bold);
    highlightRules.append(qMakePair(QRegularExpression("(?<=\\\").*(?=\\\")"), stringFormat));

    QTextCharFormat urlFormat;
    urlFormat.setForeground(Qt::green);
    // urlFormat.setFontWeight(QFont::Medium);
    urlFormat.setFontUnderline(true);
    urlFormat.setUnderlineColor(Qt::cyan);
    highlightRules.append(qMakePair(QRegularExpression("http://.*|https://.*"), urlFormat));

    QTextCharFormat importsFormat;
    importsFormat.setForeground(Qt::red);
    // You can uncomment and use this if you need to handle specific imports formatting
    // QStringList importsKeywords = { "import", "from" };
    // for (const QString& keyword : importsKeywords) {
    //     highlightRules.append(qMakePair(QRegularExpression(QString("(?<=%1\\s).*(?=\\s)").arg(keyword)), importsFormat));
    // }
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
    for (const auto& rule : highlightRules) {
        QRegularExpression expression(rule.first);
        QRegularExpressionMatchIterator matchIterator = expression.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            int index = match.capturedStart();
            int length = match.capturedLength();
            setFormat(index, length, rule.second);
        }
    }
}
