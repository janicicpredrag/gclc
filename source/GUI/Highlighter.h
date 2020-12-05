#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QString>


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) /*Q_DECL_OVERRIDE*/;

private:
    struct HighlightingRule
    {
        QStringList keywords;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
};

#endif // HIGHLIGHTER_H

