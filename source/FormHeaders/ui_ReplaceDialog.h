/********************************************************************************
** Form generated from reading UI file 'ReplaceDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPLACEDIALOG_H
#define UI_REPLACEDIALOG_H

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

class Ui_ReplaceDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *findEdit;
    QLabel *label;
    QCheckBox *caseSensitive;
    QCheckBox *matchWholeWord;
    QLineEdit *replaceEdit;
    QLabel *label_2;

    void setupUi(QDialog *ReplaceDialog)
    {
        if (ReplaceDialog->objectName().isEmpty())
            ReplaceDialog->setObjectName(QStringLiteral("ReplaceDialog"));
        ReplaceDialog->resize(369, 203);
        buttonBox = new QDialogButtonBox(ReplaceDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(180, 160, 176, 27));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        findEdit = new QLineEdit(ReplaceDialog);
        findEdit->setObjectName(QStringLiteral("findEdit"));
        findEdit->setGeometry(QRect(130, 30, 221, 27));
        label = new QLabel(ReplaceDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 40, 66, 17));
        caseSensitive = new QCheckBox(ReplaceDialog);
        caseSensitive->setObjectName(QStringLiteral("caseSensitive"));
        caseSensitive->setGeometry(QRect(130, 100, 141, 22));
        matchWholeWord = new QCheckBox(ReplaceDialog);
        matchWholeWord->setObjectName(QStringLiteral("matchWholeWord"));
        matchWholeWord->setGeometry(QRect(130, 120, 161, 22));
        replaceEdit = new QLineEdit(ReplaceDialog);
        replaceEdit->setObjectName(QStringLiteral("replaceEdit"));
        replaceEdit->setGeometry(QRect(130, 60, 221, 27));
        label_2 = new QLabel(ReplaceDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 70, 111, 17));
        QWidget::setTabOrder(findEdit, replaceEdit);
        QWidget::setTabOrder(replaceEdit, caseSensitive);
        QWidget::setTabOrder(caseSensitive, matchWholeWord);
        QWidget::setTabOrder(matchWholeWord, buttonBox);

        retranslateUi(ReplaceDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ReplaceDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ReplaceDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ReplaceDialog);
    } // setupUi

    void retranslateUi(QDialog *ReplaceDialog)
    {
        ReplaceDialog->setWindowTitle(QApplication::translate("ReplaceDialog", "Replace Text", nullptr));
        label->setText(QApplication::translate("ReplaceDialog", "Find text:", nullptr));
        caseSensitive->setText(QApplication::translate("ReplaceDialog", "Case sensitive", nullptr));
        matchWholeWord->setText(QApplication::translate("ReplaceDialog", "Match whole word", nullptr));
        label_2->setText(QApplication::translate("ReplaceDialog", "Replace with:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReplaceDialog: public Ui_ReplaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPLACEDIALOG_H
