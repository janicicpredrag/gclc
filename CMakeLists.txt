cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(GCLC LANGUAGES CXX)

set(gui AUTO CACHE STRING "build the GCLC GUI")
set_property(CACHE gui PROPERTY STRINGS AUTO ON OFF)

# default installation location to “executable/”
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/executable CACHE PATH
    "installation prefix" FORCE)
endif()

# enable -W and -Wall if possible
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-W HAS_W)
if(HAS_W)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W")
endif()
CHECK_CXX_COMPILER_FLAG(-Wall HAS_WALL)
if(HAS_WALL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if(NOT gui STREQUAL "OFF")
  find_package(Qt6 COMPONENTS Core Gui PrintSupport Widgets)
  if(gui STREQUAL "AUTO")
    if(Qt6_FOUND)
      message(STATUS "setting -Dgui=ON")
      set(gui ON)
    else()
      message(STATUS "setting -Dgui=OFF")
      set(gui OFF)
    endif()
  elseif(NOT Qt6_FOUND)
    message(FATAL_ERROR "-Dgui=ON and Qt6 not found")
  endif()
endif()

add_executable(gclc
  source/cGCLCmain.cpp
  source/Export/TikZOutput.cpp
  source/Export/SVGOutput.cpp
  source/Export/PSTricksOutput.cpp
  source/Export/LaTeXOutput.cpp
  source/Export/GCLCOutput.cpp
  source/Export/EPSOutput.cpp
  source/ExpressionParser/parser.cpp
  source/GCLCEngine/Transformations.cpp
  source/GCLCEngine/TheoremProving.cpp
  source/GCLCEngine/LowLevelCommands.cpp
  source/GCLCEngine/Layers.cpp
  source/GCLCEngine/LabelingCommands.cpp
  source/GCLCEngine/GCLC.cpp
  source/GCLCEngine/DrawingCommands.cpp
  source/GCLCEngine/Cartesian3DCommands.cpp
  source/GCLCEngine/Cartesian2DCommands.cpp
  source/GCLCEngine/Calculations.cpp
  source/GCLCEngine/BasicDefinitions.cpp
  source/GCLCEngine/BasicConstructions.cpp
  source/GCLCEngine/Animations.cpp
  source/GenericEngine/IntermediateRepresentation.cpp
  source/GenericEngine/GCompiler.cpp
  source/GraphDrawing/settings.cpp
  source/GraphDrawing/graph_util.cpp
  source/GraphDrawing/graphnode.cpp
  source/GraphDrawing/graph.cpp
  source/GraphDrawing/drawing.cpp
  source/GraphDrawing/barycenterdrawing.cpp
  source/GraphDrawing/arclayereddrawing.cpp
  source/Input/StringInput.cpp
  source/Input/GCLCInput.cpp
  source/Input/FileInput.cpp
  source/Logging/GCLCLog.cpp
  source/Logging/FileLog.cpp
  source/Logging/DummyLog.cpp
  source/Utils/Utils.cpp
  source/Utils/Timer.cpp
  source/TheoremProver/WuMethod.cpp
  source/TheoremProver/TheoremProver.cpp
  source/TheoremProver/ProverExpression.cpp
  source/TheoremProver/GroebnerMethod.cpp
  source/TheoremProver/AreaMethod.cpp
  source/TheoremProver/AlgMethodReducible.cpp
  source/TheoremProver/AlgMethod.cpp
  source/AlgebraicMethods/XTerm.cpp
  source/AlgebraicMethods/xpolynomial.cpp
  source/AlgebraicMethods/Wu.cpp
  source/AlgebraicMethods/UTerm.cpp
  source/AlgebraicMethods/UPolynomialFraction.cpp
  source/AlgebraicMethods/UPolynomial.cpp
  source/AlgebraicMethods/TermStorageVector.cpp
  source/AlgebraicMethods/TermStorageAvl.cpp
  source/AlgebraicMethods/TermStorage.cpp
  source/AlgebraicMethods/Term.cpp
  source/AlgebraicMethods/Reduce.cpp
  source/AlgebraicMethods/Prover.cpp
  source/AlgebraicMethods/Power.cpp
  source/AlgebraicMethods/PolyReader.cpp
  source/AlgebraicMethods/Polynomial.cpp
  source/AlgebraicMethods/Object.cpp
  source/AlgebraicMethods/Log.cpp
  source/AlgebraicMethods/ITimeOut.cpp
  source/AlgebraicMethods/Groebner.cpp
  source/Import/ListOfFaces.cpp 
)

# require ISO C++11
target_compile_features(gclc PRIVATE cxx_std_11)

if(MINGW)
  target_link_options(gclc
    PRIVATE "LINKER:-s LINKER:-subsystem,console LINKER:-mthreads"
  )
endif()

install(TARGETS gclc
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
)

add_subdirectory(source)
