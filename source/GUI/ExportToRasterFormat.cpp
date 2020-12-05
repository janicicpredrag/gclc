#include "ExportToRasterFormat.h"
#include "ui_ExportToRasterFormat.h"


// --------------------------------------------------------------------------------------------


ExportToRasterFormat::ExportToRasterFormat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportToRasterFormat)
{
    ui->setupUi(this);

    ui->startFrame->setText("1");
    ui->endFrame->setText("1");

    ui->comboBox->addItem(" 75");
    ui->comboBox->addItem("150");
    ui->comboBox->addItem("300");
    ui->comboBox->addItem("600");
    ui->comboBox->setCurrentIndex(1);
}


// --------------------------------------------------------------------------------------------


ExportToRasterFormat::~ExportToRasterFormat()
{
    delete ui;
}


// --------------------------------------------------------------------------------------------


bool ExportToRasterFormat::currentFrameOnly()  {
    return ui->radioButton->isChecked();
}


// --------------------------------------------------------------------------------------------


int ExportToRasterFormat::rangeStartFrame() {
   return ui->startFrame->text().toUInt();
}


// --------------------------------------------------------------------------------------------


int ExportToRasterFormat::rangeEndFrame()  {
    return ui->endFrame->text().toUInt();
}


// --------------------------------------------------------------------------------------------


int ExportToRasterFormat::getResolution()  {
    return ui->comboBox->currentText().toUInt();
}


// --------------------------------------------------------------------------------------------
