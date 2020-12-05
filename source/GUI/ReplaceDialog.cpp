#include "ReplaceDialog.h"
#include "FormHeaders/ui_ReplaceDialog.h"


// --------------------------------------------------------------------------------------------


ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    ui->findEdit->setFocus();
}


// --------------------------------------------------------------------------------------------


ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}


// --------------------------------------------------------------------------------------------
