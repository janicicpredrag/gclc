#-------------------------------------------------
#
# Project created by QtCreator 2015-07-27T16:41:18
#
#-------------------------------------------------

QT += core
QT += gui 
QT += printsupport
QT += widgets

TARGET = ../../../executable/gclc-gui
TEMPLATE = app


SOURCES += gGCLCmain.cpp\
    Export/TikZOutput.cpp \
    Export/SVGOutput.cpp \
    Export/PSTricksOutput.cpp \
    Export/LaTeXOutput.cpp \
    Export/GCLCOutput.cpp \
    Export/EPSOutput.cpp \
    ExpressionParser/parser.cpp \
    GCLCEngine/Transformations.cpp \
    GCLCEngine/TheoremProving.cpp \
    GCLCEngine/LowLevelCommands.cpp \
    GCLCEngine/Layers.cpp \
    GCLCEngine/LabelingCommands.cpp \
    GCLCEngine/GCLC.cpp \
    GCLCEngine/DrawingCommands.cpp \
    GCLCEngine/Cartesian3DCommands.cpp \
    GCLCEngine/Cartesian2DCommands.cpp \
    GCLCEngine/Calculations.cpp \
    GCLCEngine/BasicDefinitions.cpp \
    GCLCEngine/BasicConstructions.cpp \
    GCLCEngine/Animations.cpp \
    GenericEngine/GCompiler.cpp \
    GraphDrawing/settings.cpp \
    GraphDrawing/graph_util.cpp \
    GraphDrawing/graphnode.cpp \
    GraphDrawing/graph.cpp \
    GraphDrawing/drawing.cpp \
    GraphDrawing/barycenterdrawing.cpp \
    GraphDrawing/arclayereddrawing.cpp \
    Input/StringInput.cpp \
    Input/GCLCInput.cpp \
    Input/FileInput.cpp \
    Logging/GCLCLog.cpp \
    Logging/FileLog.cpp \
    Logging/DummyLog.cpp \
    Utils/Utils.cpp \
    Utils/Timer.cpp \
    TheoremProver/WuMethod.cpp \
    TheoremProver/TheoremProver.cpp \
    TheoremProver/ProverExpression.cpp \
    TheoremProver/GroebnerMethod.cpp \
    TheoremProver/AreaMethod.cpp \
    TheoremProver/AlgMethodReducible.cpp \
    TheoremProver/AlgMethod.cpp \
    AlgebraicMethods/XTerm.cpp \
    AlgebraicMethods/xpolynomial.cpp \
    AlgebraicMethods/Wu.cpp \
    AlgebraicMethods/UTerm.cpp \
    AlgebraicMethods/UPolynomialFraction.cpp \
    AlgebraicMethods/UPolynomial.cpp \
    AlgebraicMethods/TermStorageVector.cpp \
    AlgebraicMethods/TermStorageAvl.cpp \
    AlgebraicMethods/TermStorage.cpp \
    AlgebraicMethods/Term.cpp \
    AlgebraicMethods/StringBuilder.cpp \
    AlgebraicMethods/Reduce.cpp \
    AlgebraicMethods/Prover.cpp \
    AlgebraicMethods/Power.cpp \
    AlgebraicMethods/PolyReader.cpp \
    AlgebraicMethods/Polynomial.cpp \
    AlgebraicMethods/ObjectBank.cpp \
    AlgebraicMethods/Object.cpp \
    AlgebraicMethods/Log.cpp \
    AlgebraicMethods/ITimeOut.cpp \
    AlgebraicMethods/Groebner.cpp \
    Input/EditorInput.cpp \
    Export/QGraphicsViewOutput.cpp \
    Logging/QOutputLog.cpp \
    Import/JavaView.cpp \
    Import/ListOfFaces.cpp \
    GUI/FreePointItem.cpp \
    GUI/FindDialog.cpp \
    GUI/ReplaceDialog.cpp \
    GUI/ChildWindow.cpp \
    GUI/MainWindow.cpp \
    GUI/ExportToRasterFormat.cpp \
    GUI/WatchWindow.cpp \
    GUI/ChildForm.cpp \
    GUI/Highlighter.cpp \
    GUI/OutputHighlighter.cpp \
    GenericEngine/IntermediateRepresentation.cpp

HEADERS  += \
    Export/TikZOutput.h \
    Export/SVGOutput.h \
    Export/PSTricksOutput.h \
    Export/LaTeXOutput.h \
    Export/GCLCOutput.h \
    Export/EPSOutput.h \
    ExpressionParser/parser.h \
    GCLCEngine/GCLC.h \
    GenericEngine/GCompiler.h \
    GraphDrawing/settings.h \
    GraphDrawing/graph_util.h \
    GraphDrawing/graphnode.h \
    GraphDrawing/graph.h \
    GraphDrawing/drawing.h \
    GraphDrawing/barycenterdrawing.h \
    GraphDrawing/arclayereddrawing.h \
    Input/StringInput.h \
    Input/GCLCInput.h \
    Input/FileInput.h \
    Logging/GCLCLog.h \
    Logging/FileLog.h \
    Logging/DummyLog.h \
    Utils/Utils.h \
    Utils/Timer.h \
    Utils/Common.h \
    TheoremProver/WuMethod.h \
    TheoremProver/TheoremProver.h \
    TheoremProver/ProverExpression.h \
    TheoremProver/GroebnerMethod.h \
    TheoremProver/AreaMethod.h \
    TheoremProver/AlgMethod.h \
    AlgebraicMethods/XTerm.h \
    AlgebraicMethods/XPolynomial.h \
    AlgebraicMethods/Wu.h \
    AlgebraicMethods/UTerm.h \
    AlgebraicMethods/UPolynomialFraction.h \
    AlgebraicMethods/UPolynomial.h \
    AlgebraicMethods/TermStorageVector.h \
    AlgebraicMethods/TermStorageAvl.h \
    AlgebraicMethods/TermStorage.h \
    AlgebraicMethods/Term.h \
    AlgebraicMethods/StringBuilder.h \
    AlgebraicMethods/stdinc.h \
    AlgebraicMethods/Reduce.h \
    AlgebraicMethods/Prover.h \
    AlgebraicMethods/Power.h \
    AlgebraicMethods/PolyReader.h \
    AlgebraicMethods/Polynomial.h \
    AlgebraicMethods/ObjectBank.h \
    AlgebraicMethods/Object.h \
    AlgebraicMethods/Log.h \
    AlgebraicMethods/ITimeout.h \
    AlgebraicMethods/Groebner.h \
    Input/EditorInput.h \
    Export/QGraphicsViewOutput.h \
    Logging/QOutputLog.h \
    Import/JavaView.h \
    Import/ListOfFaces.h \
    GUI/FreePointItem.h \
    GUI/FindDialog.h \
    GUI/ReplaceDialog.h \
    GUI/ChildWindow.h \
    GUI/MainWindow.h \
    GUI/ExportToRasterFormat.h \
    GUI/WatchWindow.h \
    GUI/ChildForm.h \
    GUI/Highlighter.h \
    GUI/OutputHighlighter.h \
    GenericEngine/IntermediateRepresentation.h

FORMS    += \
    GUI/FindDialog.ui \
    GUI/ReplaceDialog.ui \
    GUI/MainWindow.ui \
    GUI/WatchWindow.ui \
    GUI/ExportToRasterFormat.ui \
    GUI/ChildForm.ui

UI_DIR = ../source/FormHeaders

RESOURCES += \
    resource.qrc

CONFIG(debug, debug|release) {
    DESTDIR = ../build/gui/debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../build/gui/release
}

OBJECTS_DIR = $$DESTDIR
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc
UI_DIR = $$DESTDIR/u

QMAKE_CXXFLAGS += "-fno-sized-deallocation"
