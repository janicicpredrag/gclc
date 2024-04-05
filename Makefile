CXX ?= g++

SRC_DIR ?= source
BUILD_DIR ?= build/command-line
BIN_DIR ?= executable
TARGET_EXEC ?= gclc

SRCS       = cGCLCmain.cpp \
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
		GenericEngine/IntermediateRepresentation.cpp \
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
		Import/ListOfFaces.cpp 

OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CPPFLAGS ?= 
CXXFLAGS = -m64 -std=c++11 -O2 -Wall -W -pipe -fPIC

LDFLAGS = 

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo Compiling $< ...
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/$(TARGET_EXEC): $(OBJS)
	@echo Creating $@ 
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(BIN_DIR)/$(TARGET_EXEC)

-include $(DEPS)

MKDIR_P ?= mkdir -p


