#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QTextDocument>
#include "ui_FindDialog.h"

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();
    QString getText() { return ui->lineEdit->text(); }
    void setText(QString s) { ui->lineEdit->setText(s); }

    bool caseSensitive() { return ui->caseSensitive->checkState(); }
    bool matchWholeWord() { return ui->matchWholeWord->checkState(); }

    void setFlags(QTextDocument::FindFlags flags) {
        if (flags & QTextDocument::FindCaseSensitively )
            ui->caseSensitive->setChecked(true);
        if (flags & QTextDocument::FindWholeWords )
            ui->matchWholeWord->setChecked(true);
    };

private:

    Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H
