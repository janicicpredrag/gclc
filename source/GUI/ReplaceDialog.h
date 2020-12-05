#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QTextDocument>
#include "ui_ReplaceDialog.h"

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ReplaceDialog(QWidget *parent = 0);
    ~ReplaceDialog();
    QString getFindText() { return ui->findEdit->text(); }
    void setFindText(QString s) { ui->findEdit->setText(s); }

    QString getReplaceText() { return ui->replaceEdit->text(); }
    void setReplaceText(QString s) { ui->replaceEdit->setText(s); }

    bool caseSensitive() { return ui->caseSensitive->checkState(); }
    bool matchWholeWord() { return ui->matchWholeWord->checkState(); }

    void setFlags(QTextDocument::FindFlags flags) {
        if (flags & QTextDocument::FindCaseSensitively )
            ui->caseSensitive->setChecked(true);
        if (flags & QTextDocument::FindWholeWords )
            ui->matchWholeWord->setChecked(true);
    };

private:
    Ui::ReplaceDialog *ui;
};

#endif // REPLACEDIALOG_H
