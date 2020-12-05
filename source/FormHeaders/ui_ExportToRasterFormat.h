/********************************************************************************
** Form generated from reading UI file 'ExportToRasterFormat.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTTORASTERFORMAT_H
#define UI_EXPORTTORASTERFORMAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ExportToRasterFormat
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QLineEdit *startFrame;
    QLineEdit *endFrame;
    QGroupBox *groupBox_2;
    QDialogButtonBox *buttonBox;
    QComboBox *comboBox;

    void setupUi(QDialog *ExportToRasterFormat)
    {
        if (ExportToRasterFormat->objectName().isEmpty())
            ExportToRasterFormat->setObjectName(QStringLiteral("ExportToRasterFormat"));
        ExportToRasterFormat->resize(484, 227);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/WinGCLC.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ExportToRasterFormat->setWindowIcon(icon);
        gridLayout = new QGridLayout(ExportToRasterFormat);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox = new QGroupBox(ExportToRasterFormat);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(10, 30, 181, 22));
        radioButton->setChecked(true);
        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(10, 60, 181, 21));
        startFrame = new QLineEdit(groupBox);
        startFrame->setObjectName(QStringLiteral("startFrame"));
        startFrame->setGeometry(QRect(190, 60, 113, 27));
        endFrame = new QLineEdit(groupBox);
        endFrame->setObjectName(QStringLiteral("endFrame"));
        endFrame->setGeometry(QRect(310, 60, 113, 27));

        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(ExportToRasterFormat);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        buttonBox = new QDialogButtonBox(groupBox_2);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(0, 60, 466, 27));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        comboBox = new QComboBox(groupBox_2);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(50, 30, 78, 27));

        gridLayout->addWidget(groupBox_2, 1, 0, 1, 1);


        retranslateUi(ExportToRasterFormat);
        QObject::connect(buttonBox, SIGNAL(accepted()), ExportToRasterFormat, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ExportToRasterFormat, SLOT(reject()));

        QMetaObject::connectSlotsByName(ExportToRasterFormat);
    } // setupUi

    void retranslateUi(QDialog *ExportToRasterFormat)
    {
        ExportToRasterFormat->setWindowTitle(QApplication::translate("ExportToRasterFormat", "Export to Raster images", nullptr));
        groupBox->setTitle(QApplication::translate("ExportToRasterFormat", "Range", nullptr));
        radioButton->setText(QApplication::translate("ExportToRasterFormat", "Current Frame", nullptr));
        radioButton_2->setText(QApplication::translate("ExportToRasterFormat", "Frames Range", nullptr));
        groupBox_2->setTitle(QApplication::translate("ExportToRasterFormat", "Resolution", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExportToRasterFormat: public Ui_ExportToRasterFormat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTTORASTERFORMAT_H
