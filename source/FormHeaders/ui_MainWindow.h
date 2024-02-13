/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionAbout;
    QAction *actionNew;
    QAction *actionBuild;
    QAction *actionSave_As;
    QAction *actionExit;
    QAction *actionChange_font;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionChange_output_font;
    QAction *actionClose;
    QAction *actionWatch_Window;
    QAction *actionManual;
    QAction *actionExport_to_TikZ;
    QAction *actionExport_to_LaTeX_pic;
    QAction *actionExport_to_EPS;
    QAction *actionExport_to_SVG;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionSelect_All;
    QAction *actionFind;
    QAction *actionFind_Next;
    QAction *actionFind_Previous;
    QAction *actionReplace;
    QAction *actionEnable_Theorem_Proving;
    QAction *actionArea_Method;
    QAction *actionWu_s_Method;
    QAction *actionGroebner_Bases_Method;
    QAction *actionProof_Export_to_LaTeX;
    QAction *actionProof_Export_to_XML;
    QAction *actionExport_to_LaTeX_PStrics;
    QAction *actionImport_from_JavaView;
    QAction *actionDeduction_Control;
    QAction *actionZoom_Normal;
    QAction *actionEnable_Highlighting;
    QAction *actionPick_Color;
    QAction *actionShowFreePoints;
    QAction *actionExport_to_Raster;
    QAction *actionExport_to_XML_Format_Specification;
    QAction *actionGCLC_Specification;
    QAction *actionGCLC_Image;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuExport;
    QMenu *menuImport;
    QMenu *menuPrint;
    QMenu *menuHelp;
    QMenu *menuBuild;
    QMenu *menuEdit;
    QMenu *menuTheorem_Proving;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(805, 545);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/WinGCLCDoc.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionOpen->setIconVisibleInMenu(false);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/document-save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon2);
        actionSave->setIconVisibleInMenu(true);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/document-new.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon3);
        actionNew->setIconVisibleInMenu(true);
        actionBuild = new QAction(MainWindow);
        actionBuild->setObjectName(QStringLiteral("actionBuild"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/build.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBuild->setIcon(icon4);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionChange_font = new QAction(MainWindow);
        actionChange_font->setObjectName(QStringLiteral("actionChange_font"));
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName(QStringLiteral("actionZoom_In"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/zoom-in.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_In->setIcon(icon5);
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QStringLiteral("actionZoom_Out"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/zoom-out.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Out->setIcon(icon6);
        actionChange_output_font = new QAction(MainWindow);
        actionChange_output_font->setObjectName(QStringLiteral("actionChange_output_font"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionWatch_Window = new QAction(MainWindow);
        actionWatch_Window->setObjectName(QStringLiteral("actionWatch_Window"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/watch.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWatch_Window->setIcon(icon7);
        actionManual = new QAction(MainWindow);
        actionManual->setObjectName(QStringLiteral("actionManual"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionManual->setIcon(icon8);
        actionExport_to_TikZ = new QAction(MainWindow);
        actionExport_to_TikZ->setObjectName(QStringLiteral("actionExport_to_TikZ"));
        actionExport_to_LaTeX_pic = new QAction(MainWindow);
        actionExport_to_LaTeX_pic->setObjectName(QStringLiteral("actionExport_to_LaTeX_pic"));
        actionExport_to_EPS = new QAction(MainWindow);
        actionExport_to_EPS->setObjectName(QStringLiteral("actionExport_to_EPS"));
        actionExport_to_SVG = new QAction(MainWindow);
        actionExport_to_SVG->setObjectName(QStringLiteral("actionExport_to_SVG"));
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QStringLiteral("actionCut"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QStringLiteral("actionPaste"));
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QStringLiteral("actionSelect_All"));
        actionFind = new QAction(MainWindow);
        actionFind->setObjectName(QStringLiteral("actionFind"));
        actionFind_Next = new QAction(MainWindow);
        actionFind_Next->setObjectName(QStringLiteral("actionFind_Next"));
        actionFind_Previous = new QAction(MainWindow);
        actionFind_Previous->setObjectName(QStringLiteral("actionFind_Previous"));
        actionReplace = new QAction(MainWindow);
        actionReplace->setObjectName(QStringLiteral("actionReplace"));
        actionEnable_Theorem_Proving = new QAction(MainWindow);
        actionEnable_Theorem_Proving->setObjectName(QStringLiteral("actionEnable_Theorem_Proving"));
        actionEnable_Theorem_Proving->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/proving.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEnable_Theorem_Proving->setIcon(icon9);
        actionArea_Method = new QAction(MainWindow);
        actionArea_Method->setObjectName(QStringLiteral("actionArea_Method"));
        actionArea_Method->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/proving-area.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionArea_Method->setIcon(icon10);
        actionWu_s_Method = new QAction(MainWindow);
        actionWu_s_Method->setObjectName(QStringLiteral("actionWu_s_Method"));
        actionWu_s_Method->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icons/proving-wu.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWu_s_Method->setIcon(icon11);
        actionGroebner_Bases_Method = new QAction(MainWindow);
        actionGroebner_Bases_Method->setObjectName(QStringLiteral("actionGroebner_Bases_Method"));
        actionGroebner_Bases_Method->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/icons/proving-groebner.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGroebner_Bases_Method->setIcon(icon12);
        actionProof_Export_to_LaTeX = new QAction(MainWindow);
        actionProof_Export_to_LaTeX->setObjectName(QStringLiteral("actionProof_Export_to_LaTeX"));
        actionProof_Export_to_LaTeX->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/icons/prooof-latex.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionProof_Export_to_LaTeX->setIcon(icon13);
        actionProof_Export_to_XML = new QAction(MainWindow);
        actionProof_Export_to_XML->setObjectName(QStringLiteral("actionProof_Export_to_XML"));
        actionProof_Export_to_XML->setCheckable(true);
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/icons/prooof-xml.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionProof_Export_to_XML->setIcon(icon14);
        actionExport_to_LaTeX_PStrics = new QAction(MainWindow);
        actionExport_to_LaTeX_PStrics->setObjectName(QStringLiteral("actionExport_to_LaTeX_PStrics"));
        actionImport_from_JavaView = new QAction(MainWindow);
        actionImport_from_JavaView->setObjectName(QStringLiteral("actionImport_from_JavaView"));
        actionDeduction_Control = new QAction(MainWindow);
        actionDeduction_Control->setObjectName(QStringLiteral("actionDeduction_Control"));
        actionDeduction_Control->setCheckable(true);
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/icons/deduction_control.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDeduction_Control->setIcon(icon15);
        actionZoom_Normal = new QAction(MainWindow);
        actionZoom_Normal->setObjectName(QStringLiteral("actionZoom_Normal"));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/icons/zoom-normal.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Normal->setIcon(icon16);
        actionEnable_Highlighting = new QAction(MainWindow);
        actionEnable_Highlighting->setObjectName(QStringLiteral("actionEnable_Highlighting"));
        actionEnable_Highlighting->setCheckable(true);
        actionPick_Color = new QAction(MainWindow);
        actionPick_Color->setObjectName(QStringLiteral("actionPick_Color"));
        actionShowFreePoints = new QAction(MainWindow);
        actionShowFreePoints->setObjectName(QStringLiteral("actionShowFreePoints"));
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/icons/free-points.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowFreePoints->setIcon(icon17);
        actionExport_to_Raster = new QAction(MainWindow);
        actionExport_to_Raster->setObjectName(QStringLiteral("actionExport_to_Raster"));
        actionExport_to_XML_Format_Specification = new QAction(MainWindow);
        actionExport_to_XML_Format_Specification->setObjectName(QStringLiteral("actionExport_to_XML_Format_Specification"));
        actionGCLC_Specification = new QAction(MainWindow);
        actionGCLC_Specification->setObjectName(QStringLiteral("actionGCLC_Specification"));
        actionGCLC_Image = new QAction(MainWindow);
        actionGCLC_Image->setObjectName(QStringLiteral("actionGCLC_Image"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));

        verticalLayout->addWidget(mdiArea);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 805, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuExport = new QMenu(menuFile);
        menuExport->setObjectName(QStringLiteral("menuExport"));
        menuImport = new QMenu(menuFile);
        menuImport->setObjectName(QStringLiteral("menuImport"));
        menuPrint = new QMenu(menuFile);
        menuPrint->setObjectName(QStringLiteral("menuPrint"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuBuild = new QMenu(menuBar);
        menuBuild->setObjectName(QStringLiteral("menuBuild"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuTheorem_Proving = new QMenu(menuBar);
        menuTheorem_Proving->setObjectName(QStringLiteral("menuTheorem_Proving"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuTheorem_Proving->menuAction());
        menuBar->addAction(menuBuild->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(menuImport->menuAction());
        menuFile->addAction(menuExport->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(menuPrint->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionClose);
        menuFile->addAction(actionExit);
        menuExport->addAction(actionExport_to_TikZ);
        menuExport->addAction(actionExport_to_LaTeX_PStrics);
        menuExport->addAction(actionExport_to_LaTeX_pic);
        menuExport->addAction(actionExport_to_EPS);
        menuExport->addAction(actionExport_to_SVG);
        menuExport->addAction(actionExport_to_Raster);
        menuExport->addSeparator();
        menuExport->addAction(actionExport_to_XML_Format_Specification);
        menuImport->addAction(actionImport_from_JavaView);
        menuPrint->addAction(actionGCLC_Specification);
        menuPrint->addAction(actionGCLC_Image);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionManual);
        menuBuild->addAction(actionBuild);
        menuBuild->addAction(actionZoom_In);
        menuBuild->addAction(actionZoom_Out);
        menuBuild->addAction(actionZoom_Normal);
        menuBuild->addSeparator();
        menuBuild->addSeparator();
        menuBuild->addAction(actionShowFreePoints);
        menuBuild->addAction(actionWatch_Window);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);
        menuEdit->addSeparator();
        menuEdit->addAction(actionFind);
        menuEdit->addAction(actionFind_Next);
        menuEdit->addAction(actionFind_Previous);
        menuEdit->addAction(actionReplace);
        menuEdit->addSeparator();
        menuEdit->addAction(actionChange_font);
        menuEdit->addAction(actionChange_output_font);
        menuEdit->addSeparator();
        menuEdit->addAction(actionEnable_Highlighting);
        menuEdit->addSeparator();
        menuEdit->addAction(actionPick_Color);
        menuTheorem_Proving->addAction(actionEnable_Theorem_Proving);
        menuTheorem_Proving->addSeparator();
        menuTheorem_Proving->addAction(actionArea_Method);
        menuTheorem_Proving->addAction(actionWu_s_Method);
        menuTheorem_Proving->addAction(actionGroebner_Bases_Method);
        menuTheorem_Proving->addSeparator();
        menuTheorem_Proving->addAction(actionProof_Export_to_LaTeX);
        menuTheorem_Proving->addAction(actionProof_Export_to_XML);
        menuTheorem_Proving->addSeparator();
        menuTheorem_Proving->addAction(actionDeduction_Control);
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionEnable_Theorem_Proving);
        mainToolBar->addAction(actionArea_Method);
        mainToolBar->addAction(actionWu_s_Method);
        mainToolBar->addAction(actionGroebner_Bases_Method);
        mainToolBar->addAction(actionProof_Export_to_LaTeX);
        mainToolBar->addAction(actionProof_Export_to_XML);
        mainToolBar->addAction(actionDeduction_Control);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionBuild);
        mainToolBar->addAction(actionZoom_In);
        mainToolBar->addAction(actionZoom_Out);
        mainToolBar->addAction(actionZoom_Normal);
        mainToolBar->addAction(actionShowFreePoints);
        mainToolBar->addAction(actionWatch_Window);
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);
        QObject::connect(actionAbout, SIGNAL(triggered()), MainWindow, SLOT(displayAbout()));
        QObject::connect(actionNew, SIGNAL(triggered()), MainWindow, SLOT(newDocument()));
        QObject::connect(actionOpen, SIGNAL(triggered()), MainWindow, SLOT(openDocument()));
        QObject::connect(actionBuild, SIGNAL(triggered()), MainWindow, SLOT(buildCurrentGCLCDocument()));
        QObject::connect(actionSave, SIGNAL(triggered()), MainWindow, SLOT(saveDocument()));
        QObject::connect(actionSave_As, SIGNAL(triggered()), MainWindow, SLOT(saveDocumentAs()));
        QObject::connect(actionExit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(actionChange_font, SIGNAL(triggered()), MainWindow, SLOT(changeFont()));
        QObject::connect(actionZoom_In, SIGNAL(triggered()), MainWindow, SLOT(zoomIn()));
        QObject::connect(actionZoom_Out, SIGNAL(triggered()), MainWindow, SLOT(zoomOut()));
        QObject::connect(actionChange_output_font, SIGNAL(triggered()), MainWindow, SLOT(changeOutputFont()));
        QObject::connect(actionClose, SIGNAL(triggered()), MainWindow, SLOT(closeDocument()));
        QObject::connect(actionWatch_Window, SIGNAL(triggered()), MainWindow, SLOT(watch()));
        QObject::connect(actionManual, SIGNAL(triggered()), MainWindow, SLOT(openManual()));
        QObject::connect(actionExport_to_TikZ, SIGNAL(triggered()), MainWindow, SLOT(ExportTikZ()));
        QObject::connect(actionExport_to_LaTeX_pic, SIGNAL(triggered()), MainWindow, SLOT(ExportPic()));
        QObject::connect(actionExport_to_EPS, SIGNAL(triggered()), MainWindow, SLOT(ExportEPS()));
        QObject::connect(actionExport_to_SVG, SIGNAL(triggered()), MainWindow, SLOT(ExportSVG()));
        QObject::connect(actionUndo, SIGNAL(triggered()), MainWindow, SLOT(onUndo()));
        QObject::connect(actionRedo, SIGNAL(triggered()), MainWindow, SLOT(onRedo()));
        QObject::connect(actionCut, SIGNAL(triggered()), MainWindow, SLOT(onCut()));
        QObject::connect(actionCopy, SIGNAL(triggered()), MainWindow, SLOT(onCopy()));
        QObject::connect(actionPaste, SIGNAL(triggered()), MainWindow, SLOT(onPaste()));
        QObject::connect(actionSelect_All, SIGNAL(triggered()), MainWindow, SLOT(onSelectAll()));
        QObject::connect(actionFind, SIGNAL(triggered()), MainWindow, SLOT(onFind()));
        QObject::connect(actionFind_Next, SIGNAL(triggered()), MainWindow, SLOT(onFindNext()));
        QObject::connect(actionFind_Previous, SIGNAL(triggered()), MainWindow, SLOT(onFindPrevious()));
        QObject::connect(actionReplace, SIGNAL(triggered()), MainWindow, SLOT(onReplace()));
        QObject::connect(actionEnable_Theorem_Proving, SIGNAL(triggered()), MainWindow, SLOT(onTheoremProving()));
        QObject::connect(actionArea_Method, SIGNAL(triggered()), MainWindow, SLOT(onArea()));
        QObject::connect(actionWu_s_Method, SIGNAL(triggered()), MainWindow, SLOT(onWu()));
        QObject::connect(actionGroebner_Bases_Method, SIGNAL(triggered()), MainWindow, SLOT(onGroebner()));
        QObject::connect(actionProof_Export_to_LaTeX, SIGNAL(triggered()), MainWindow, SLOT(onProofsToLaTeX()));
        QObject::connect(actionProof_Export_to_XML, SIGNAL(triggered()), MainWindow, SLOT(onProofsToXML()));
        QObject::connect(actionExport_to_LaTeX_PStrics, SIGNAL(triggered()), MainWindow, SLOT(ExportPSTrics()));
        QObject::connect(actionImport_from_JavaView, SIGNAL(triggered()), MainWindow, SLOT(ImportJavaView()));
        QObject::connect(actionDeduction_Control, SIGNAL(triggered(bool)), MainWindow, SLOT(onDeductionControl()));
        QObject::connect(actionZoom_Normal, SIGNAL(triggered()), MainWindow, SLOT(zoomNormal()));
        QObject::connect(actionEnable_Highlighting, SIGNAL(triggered()), MainWindow, SLOT(onEnableHighlighting()));
        QObject::connect(actionPick_Color, SIGNAL(triggered()), MainWindow, SLOT(onPickColor()));
        QObject::connect(actionShowFreePoints, SIGNAL(triggered()), MainWindow, SLOT(onShowFreePoints()));
        QObject::connect(actionExport_to_Raster, SIGNAL(triggered()), MainWindow, SLOT(ExportRaster()));
        QObject::connect(actionExport_to_XML_Format_Specification, SIGNAL(triggered()), MainWindow, SLOT(ExportXML()));
        QObject::connect(actionGCLC_Specification, SIGNAL(triggered()), MainWindow, SLOT(onPrintGCLCSpecification()));
        QObject::connect(actionGCLC_Image, SIGNAL(triggered()), MainWindow, SLOT(onPrintGCLCImage()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "GCLC", nullptr));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave->setText(QApplication::translate("MainWindow", "Save", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionAbout->setText(QApplication::translate("MainWindow", "About", nullptr));
        actionNew->setText(QApplication::translate("MainWindow", "New", nullptr));
#ifndef QT_NO_SHORTCUT
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_NO_SHORTCUT
        actionBuild->setText(QApplication::translate("MainWindow", "Build", nullptr));
#ifndef QT_NO_SHORTCUT
        actionBuild->setShortcut(QApplication::translate("MainWindow", "Ctrl+B", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSave_As->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionExit->setText(QApplication::translate("MainWindow", "Exit", nullptr));
        actionChange_font->setText(QApplication::translate("MainWindow", "Change font", nullptr));
        actionZoom_In->setText(QApplication::translate("MainWindow", "Zoom In", nullptr));
#ifndef QT_NO_SHORTCUT
        actionZoom_In->setShortcut(QApplication::translate("MainWindow", "Ctrl++", nullptr));
#endif // QT_NO_SHORTCUT
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", nullptr));
#ifndef QT_NO_SHORTCUT
        actionZoom_Out->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", nullptr));
#endif // QT_NO_SHORTCUT
        actionChange_output_font->setText(QApplication::translate("MainWindow", "Change output font", nullptr));
        actionClose->setText(QApplication::translate("MainWindow", "Close", nullptr));
        actionWatch_Window->setText(QApplication::translate("MainWindow", "Watch Window", nullptr));
        actionManual->setText(QApplication::translate("MainWindow", "Manual", nullptr));
#ifndef QT_NO_SHORTCUT
        actionManual->setShortcut(QApplication::translate("MainWindow", "F1", nullptr));
#endif // QT_NO_SHORTCUT
        actionExport_to_TikZ->setText(QApplication::translate("MainWindow", "LaTeX/TikZ", nullptr));
        actionExport_to_LaTeX_pic->setText(QApplication::translate("MainWindow", "LaTeX/pic", nullptr));
        actionExport_to_EPS->setText(QApplication::translate("MainWindow", "EPS", nullptr));
        actionExport_to_SVG->setText(QApplication::translate("MainWindow", "SVG", nullptr));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", nullptr));
#ifndef QT_NO_SHORTCUT
        actionUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", nullptr));
#ifndef QT_NO_SHORTCUT
        actionRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionCut->setText(QApplication::translate("MainWindow", "Cut", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_NO_SHORTCUT
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        actionPaste->setText(QApplication::translate("MainWindow", "Paste", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPaste->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_NO_SHORTCUT
        actionSelect_All->setText(QApplication::translate("MainWindow", "Select All", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSelect_All->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_NO_SHORTCUT
        actionFind->setText(QApplication::translate("MainWindow", "Find...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionFind->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", nullptr));
#endif // QT_NO_SHORTCUT
        actionFind_Next->setText(QApplication::translate("MainWindow", "Find Next", nullptr));
#ifndef QT_NO_SHORTCUT
        actionFind_Next->setShortcut(QApplication::translate("MainWindow", "Ctrl+G", nullptr));
#endif // QT_NO_SHORTCUT
        actionFind_Previous->setText(QApplication::translate("MainWindow", "Find Previous", nullptr));
#ifndef QT_NO_SHORTCUT
        actionFind_Previous->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+G", nullptr));
#endif // QT_NO_SHORTCUT
        actionReplace->setText(QApplication::translate("MainWindow", "Replace...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionReplace->setShortcut(QApplication::translate("MainWindow", "Ctrl+H", nullptr));
#endif // QT_NO_SHORTCUT
        actionEnable_Theorem_Proving->setText(QApplication::translate("MainWindow", "Enable Theorem Proving ", nullptr));
        actionArea_Method->setText(QApplication::translate("MainWindow", "Area Method", nullptr));
        actionWu_s_Method->setText(QApplication::translate("MainWindow", "Wu's Method", nullptr));
        actionGroebner_Bases_Method->setText(QApplication::translate("MainWindow", "Groebner Bases Method", nullptr));
        actionProof_Export_to_LaTeX->setText(QApplication::translate("MainWindow", "Proof Export to LaTeX", nullptr));
        actionProof_Export_to_XML->setText(QApplication::translate("MainWindow", "Proof Export to XML", nullptr));
        actionExport_to_LaTeX_PStrics->setText(QApplication::translate("MainWindow", "Export to LaTeX PStrics", nullptr));
        actionImport_from_JavaView->setText(QApplication::translate("MainWindow", "Import from JavaView", nullptr));
        actionDeduction_Control->setText(QApplication::translate("MainWindow", "Deduction Control", nullptr));
        actionZoom_Normal->setText(QApplication::translate("MainWindow", "Zoom Normal", nullptr));
        actionEnable_Highlighting->setText(QApplication::translate("MainWindow", "Enable Highlighting", nullptr));
        actionPick_Color->setText(QApplication::translate("MainWindow", "Pick Color", nullptr));
        actionShowFreePoints->setText(QApplication::translate("MainWindow", "Show Free Points", nullptr));
#ifndef QT_NO_TOOLTIP
        actionShowFreePoints->setToolTip(QApplication::translate("MainWindow", "Show Free Points", nullptr));
#endif // QT_NO_TOOLTIP
        actionExport_to_Raster->setText(QApplication::translate("MainWindow", "Raster Format (BMP, JPG, PNG)", nullptr));
        actionExport_to_XML_Format_Specification->setText(QApplication::translate("MainWindow", "XML Format Specification", nullptr));
        actionGCLC_Specification->setText(QApplication::translate("MainWindow", "GCLC Specification", nullptr));
        actionGCLC_Image->setText(QApplication::translate("MainWindow", "GCLC Image", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuExport->setTitle(QApplication::translate("MainWindow", "Export to...", nullptr));
        menuImport->setTitle(QApplication::translate("MainWindow", "Import...", nullptr));
        menuPrint->setTitle(QApplication::translate("MainWindow", "Print...", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
        menuBuild->setTitle(QApplication::translate("MainWindow", "Picture", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", nullptr));
        menuTheorem_Proving->setTitle(QApplication::translate("MainWindow", "Deduction", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
