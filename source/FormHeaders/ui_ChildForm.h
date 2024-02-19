/********************************************************************************
** Form generated from reading UI file 'ChildForm.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHILDFORM_H
#define UI_CHILDFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChildForm
{
public:
    QGridLayout *gridLayout;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QTextEdit *editor;
    QGraphicsView *graphicsOutput;
    QPlainTextEdit *output;

    void setupUi(QWidget *ChildForm)
    {
        if (ChildForm->objectName().isEmpty())
            ChildForm->setObjectName(QStringLiteral("ChildForm"));
        ChildForm->resize(748, 542);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/WinGCLC.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ChildForm->setWindowIcon(icon);
        gridLayout = new QGridLayout(ChildForm);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        splitter_2 = new QSplitter(ChildForm);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        editor = new QTextEdit(splitter);
        editor->setObjectName(QStringLiteral("editor"));
        splitter->addWidget(editor);
        graphicsOutput = new QGraphicsView(splitter);
        graphicsOutput->setObjectName(QStringLiteral("graphicsOutput"));
        graphicsOutput->setMinimumSize(QSize(0, 0));
        graphicsOutput->setBaseSize(QSize(0, 200));
        splitter->addWidget(graphicsOutput);
        splitter_2->addWidget(splitter);
        output = new QPlainTextEdit(splitter_2);
        output->setObjectName(QStringLiteral("output"));
        output->setMinimumSize(QSize(0, 0));
        output->setReadOnly(true);
        splitter_2->addWidget(output);

        gridLayout->addWidget(splitter_2, 0, 0, 1, 1);


        retranslateUi(ChildForm);

        QMetaObject::connectSlotsByName(ChildForm);
    } // setupUi

    void retranslateUi(QWidget *ChildForm)
    {
        ChildForm->setWindowTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class ChildForm: public Ui_ChildForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHILDFORM_H
