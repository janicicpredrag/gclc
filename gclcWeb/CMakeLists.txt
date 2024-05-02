cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(GCLCWEB LANGUAGES CXX)

# default installation location to “../executable/”
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../executable CACHE PATH
    "installation prefix" FORCE)
endif()

add_executable(gclc
  ../source/wGCLCmain.cpp
  ../source/Export/TikZOutput.cpp
  ../source/Export/SVGOutput.cpp
  ../source/Export/PSTricksOutput.cpp
  ../source/Export/LaTeXOutput.cpp
  ../source/Export/GCLCOutput.cpp
  ../source/Export/EPSOutput.cpp
  ../source/ExpressionParser/parser.cpp
  ../source/GCLCEngine/Transformations.cpp
  ../source/GCLCEngine/TheoremProving.cpp
  ../source/GCLCEngine/LowLevelCommands.cpp
  ../source/GCLCEngine/Layers.cpp
  ../source/GCLCEngine/LabelingCommands.cpp
  ../source/GCLCEngine/GCLC.cpp
  ../source/GCLCEngine/DrawingCommands.cpp
  ../source/GCLCEngine/Cartesian3DCommands.cpp
  ../source/GCLCEngine/Cartesian2DCommands.cpp
  ../source/GCLCEngine/Calculations.cpp
  ../source/GCLCEngine/BasicDefinitions.cpp
  ../source/GCLCEngine/BasicConstructions.cpp
  ../source/GCLCEngine/Animations.cpp
  ../source/GenericEngine/IntermediateRepresentation.cpp
  ../source/GenericEngine/GCompiler.cpp
  ../source/GraphDrawing/settings.cpp
  ../source/GraphDrawing/graph_util.cpp
  ../source/GraphDrawing/graphnode.cpp
  ../source/GraphDrawing/graph.cpp
  ../source/GraphDrawing/drawing.cpp
  ../source/GraphDrawing/barycenterdrawing.cpp
  ../source/GraphDrawing/arclayereddrawing.cpp
  ../source/Input/StringInput.cpp
  ../source/Input/GCLCInput.cpp
  ../source/Input/FileInput.cpp
  ../source/Logging/GCLCLog.cpp
  ../source/Logging/FileLog.cpp
  ../source/Logging/DummyLog.cpp
  ../source/Utils/Utils.cpp
  ../source/Utils/Timer.cpp
  ../source/TheoremProver/WuMethod.cpp
  ../source/TheoremProver/TheoremProver.cpp
  ../source/TheoremProver/ProverExpression.cpp
  ../source/TheoremProver/GroebnerMethod.cpp
  ../source/TheoremProver/AreaMethod.cpp
  ../source/TheoremProver/AlgMethodReducible.cpp
  ../source/TheoremProver/AlgMethod.cpp
  ../source/AlgebraicMethods/XTerm.cpp
  ../source/AlgebraicMethods/xpolynomial.cpp
  ../source/AlgebraicMethods/Wu.cpp
  ../source/AlgebraicMethods/UTerm.cpp
  ../source/AlgebraicMethods/UPolynomialFraction.cpp
  ../source/AlgebraicMethods/UPolynomial.cpp
  ../source/AlgebraicMethods/TermStorageVector.cpp
  ../source/AlgebraicMethods/TermStorageAvl.cpp
  ../source/AlgebraicMethods/TermStorage.cpp
  ../source/AlgebraicMethods/Term.cpp
  ../source/AlgebraicMethods/Reduce.cpp
  ../source/AlgebraicMethods/Prover.cpp
  ../source/AlgebraicMethods/Power.cpp
  ../source/AlgebraicMethods/PolyReader.cpp
  ../source/AlgebraicMethods/Polynomial.cpp
  ../source/AlgebraicMethods/Object.cpp
  ../source/AlgebraicMethods/Log.cpp
  ../source/AlgebraicMethods/ITimeOut.cpp
  ../source/AlgebraicMethods/Groebner.cpp
  ../source/Import/ListOfFaces.cpp
)

# require C++17
target_compile_features(gclc PRIVATE cxx_std_17)

target_link_options(gclc PRIVATE -sWASM=1)
target_link_options(gclc PRIVATE -sMODULARIZE=1)
target_link_options(gclc PRIVATE -sENVIRONMENT=web)
target_link_options(gclc PRIVATE "-sEXPORTED_FUNCTIONS=[_render, _malloc, _free, _calloc]")
target_link_options(gclc PRIVATE "-sEXPORTED_RUNTIME_METHODS=[cwrap, AsciiToString, stringToUTF8, lengthBytesUTF8, UTF8ToString, stringToUTF8, getValue, FS]")
target_link_options(gclc PRIVATE -sEXPORT_NAME=createModule)
target_link_options(gclc PRIVATE -s EXPORT_ES6=1)
target_link_options(gclc PRIVATE -O2)
target_link_options(gclc PRIVATE -o gclc.js)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/gclc.js
        ${CMAKE_CURRENT_BINARY_DIR}/gclc.wasm
  DESTINATION ${CMAKE_INSTALL_PREFIX}
)
