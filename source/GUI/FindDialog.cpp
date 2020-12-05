#include "FindDialog.h"


// --------------------------------------------------------------------------------------------


FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}


// --------------------------------------------------------------------------------------------


FindDialog::~FindDialog()
{
    delete ui;
}


// --------------------------------------------------------------------------------------------
