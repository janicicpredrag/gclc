#include "WatchWindow.h"
#include "FormHeaders/ui_WatchWindow.h"

// --------------------------------------------------------------------------------------------

WatchWindow::WatchWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::WatchWindow) {
  ui->setupUi(this);
  this->setWindowTitle("Watch window");
}

// --------------------------------------------------------------------------------------------

WatchWindow::~WatchWindow() { delete ui; }

// --------------------------------------------------------------------------------------------

void WatchWindow::showValues() {
  show();
  this->ui->tableWidget->setRowCount(10);
  this->ui->tableWidget->setColumnCount(2);
  this->ui->tableWidget->setColumnWidth(1, 230);

  QStringList ql;
  ql << "Object name"
     << "Object value";
  this->ui->tableWidget->setHorizontalHeaderLabels(ql);
}

// --------------------------------------------------------------------------------------------

void WatchWindow::on_tableWidget_cellEntered(int row, int column) {
  emit(changedcell(row, column));
}

// --------------------------------------------------------------------------------------------

QString WatchWindow::getObjectName(int row, int column) {
  QTableWidgetItem *i;
  i = this->ui->tableWidget->item(row, column);
  if (i == 0)
    return "";
  return i->text();
}

// --------------------------------------------------------------------------------------------

void WatchWindow::setObjectValue(int row, int column, QString s) {
  QTableWidgetItem *i = new QTableWidgetItem;
  //    i = this->ui->tableWidget->takeItem( row, column );
  i->setText(s);
  i->setFlags(i->flags() ^ Qt::ItemIsEditable);

  this->ui->tableWidget->setItem(row, column, i);
  this->ui->tableWidget->show();
}

// --------------------------------------------------------------------------------------------

void WatchWindow::on_tableWidget_cellChanged(int row, int column) {
  emit(changedcell(row, column));
}

// --------------------------------------------------------------------------------------------
