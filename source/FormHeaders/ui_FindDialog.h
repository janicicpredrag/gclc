/********************************************************************************
** Form generated from reading UI file 'FindDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDDIALOG_H
#define UI_FINDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_FindDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEdit;
    QLabel *label;
    QCheckBox *caseSensitive;
    QCheckBox *matchWholeWord;

    void setupUi(QDialog *FindDialog)
    {
        if (FindDialog->objectName().isEmpty())
            FindDialog->setObjectName(QStringLiteral("FindDialog"));
        FindDialog->resize(368, 175);
        buttonBox = new QDialogButtonBox(FindDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(170, 140, 176, 27));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lineEdit = new QLineEdit(FindDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(100, 30, 251, 27));
        label = new QLabel(FindDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 40, 66, 17));
        caseSensitive = new QCheckBox(FindDialog);
        caseSensitive->setObjectName(QStringLiteral("caseSensitive"));
        caseSensitive->setGeometry(QRect(100, 70, 141, 22));
        matchWholeWord = new QCheckBox(FindDialog);
        matchWholeWord->setObjectName(QStringLiteral("matchWholeWord"));
        matchWholeWord->setGeometry(QRect(100, 90, 161, 22));
        QWidget::setTabOrder(lineEdit, caseSensitive);
        QWidget::setTabOrder(caseSensitive, matchWholeWord);
        QWidget::setTabOrder(matchWholeWord, buttonBox);

        retranslateUi(FindDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), FindDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), FindDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(FindDialog);
    } // setupUi

    void retranslateUi(QDialog *FindDialog)
    {
        FindDialog->setWindowTitle(QApplication::translate("FindDialog", "Find Text", nullptr));
        label->setText(QApplication::translate("FindDialog", "Find text:", nullptr));
        caseSensitive->setText(QApplication::translate("FindDialog", "Case sensitive", nullptr));
        matchWholeWord->setText(QApplication::translate("FindDialog", "Match whole word", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindDialog: public Ui_FindDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDDIALOG_H
