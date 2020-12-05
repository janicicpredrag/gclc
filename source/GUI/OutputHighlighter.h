#ifndef OutputHighlighter_H
#define OutputHighlighter_H

#include <QSyntaxHighlighter>
#include <QString>


class OutputHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    OutputHighlighter(QTextDocument *parent = 0);

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

#endif // OutputHighlighter_H
