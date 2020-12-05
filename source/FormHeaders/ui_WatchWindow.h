/********************************************************************************
** Form generated from reading UI file 'WatchWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WATCHWINDOW_H
#define UI_WATCHWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_WatchWindow
{
public:
    QTableWidget *tableWidget;

    void setupUi(QDialog *WatchWindow)
    {
        if (WatchWindow->objectName().isEmpty())
            WatchWindow->setObjectName(QStringLiteral("WatchWindow"));
        WatchWindow->resize(400, 300);
        tableWidget = new QTableWidget(WatchWindow);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(20, 20, 351, 261));

        retranslateUi(WatchWindow);

        QMetaObject::connectSlotsByName(WatchWindow);
    } // setupUi

    void retranslateUi(QDialog *WatchWindow)
    {
        WatchWindow->setWindowTitle(QApplication::translate("WatchWindow", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WatchWindow: public Ui_WatchWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WATCHWINDOW_H
