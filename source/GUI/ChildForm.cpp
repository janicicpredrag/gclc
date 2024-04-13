#include "ChildForm.h"
#include "ui_ChildForm.h"

// --------------------------------------------------------------------------------------------

ChildForm::ChildForm(QWidget *parent) : QWidget(parent), ui(new Ui::ChildForm) {
  ui->setupUi(this);
}

// --------------------------------------------------------------------------------------------

ChildForm::~ChildForm() { delete ui; }

// --------------------------------------------------------------------------------------------

void ChildForm::on_editor_cursorPositionChanged() { emit(CursorMoved()); }

// --------------------------------------------------------------------------------------------

void ChildForm::on_editor_textChanged() { emit(FileChanged()); }

// --------------------------------------------------------------------------------------------
