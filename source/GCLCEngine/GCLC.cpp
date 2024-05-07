// GCLC.cpp: implementation of the CGCLC class.
//
//////////////////////////////////////////////////////////////////////

#include "GCLC.h"
#include "../Input/StringInput.h"
#include "../Logging/GCLCLog.h"
#include "../Utils/Version.h"
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

/*
GCLC_area CGCLC::m_Basic_Area = {0, 0, 0, 0};
int CGCLC::CIRCLE_PRECISION = 72;
int CGCLC::MARK_CIRCLE_PRECISION = 9;
double CGCLC::MARK_RADIUS = 0.40;
double CGCLC::DM = 1.00;

double CGCLC::ARROW_ANGLE = 15.0;
double CGCLC::ARROW_LENGTH = 3.0;
double CGCLC::ARROW_INNER_TOWARDS = 2.0 / 3.0;

int CGCLC::BEZIER_PRECISION = 36;

GCLC_area CGCLC::m_2D_Area = {0, 0, 0, 0};
double CGCLC::X_ORIGIN = 0.00;
double CGCLC::Y_ORIGIN = 0.00;
double CGCLC::UNIT = 10.00;
double CGCLC::ANG_SCALE = 1.00;
int CGCLC::ANG_LOG = 1;
int CGCLC::CONIC_PRECISION = 144;

GCLC_area CGCLC::m_3D_Area = {0, 0, 0, 0};
bool CGCLC::ANG3D_AXES_LIMITED = false;
double CGCLC::X3D_ORIGIN = 0.00;
double CGCLC::Y3D_ORIGIN = 0.00;
double CGCLC::SIN_ALPHA3D = 0.00;
double CGCLC::SIN_BETA3D = 0.00;
double CGCLC::COS_ALPHA3D = 1.00;
double CGCLC::COS_BETA3D = 1.00;
double CGCLC::UNIT3D = 10.00;
double CGCLC::ANG3D_SCALE_Y = 1.00;
double CGCLC::ANG3D_SCALE_Z = 1.00;
int CGCLC::ANG3D_LOG = 1;
double CGCLC::X3D_MIN = 0.00;
double CGCLC::X3D_MAX = 0.00;
double CGCLC::Y3D_MIN = 0.00;
double CGCLC::Y3D_MAX = 0.00;
double CGCLC::Z3D_MIN = 0.00;
double CGCLC::Z3D_MAX = 0.00;

int CGCLC::m_nAnimationFrames = 0;
int CGCLC::m_nAnimationSpeed = 0;
*/

static std::map<std::string, enum GCLCcommands> KeyWords = {
    {"point", gclc_point},
    {"line", gclc_line},
    {"circle", gclc_circle},
    {"number", gclc_number},
    {"array", gclc_array},
    {"set_equal", gclc_set_equal},
    {"expression", gclc_expression},
    {"while", gclc_while},
    {"if_then_else", gclc_if_then_else},
    {"random", gclc_random},
    {"angle", gclc_angle},
    {"angle_o", gclc_angle_o},
    {"intersec", gclc_intersec},
    {"intersection", gclc_intersec},
    {"intersec2", gclc_intersec2},
    {"intersection2", gclc_intersec2},
    {"drawpoint", gclc_drawpoint},
    {"drawsegment", gclc_drawsegment},
    {"drawdashsegment", gclc_drawdashsegment},
    {"drawline", gclc_drawline},
    {"drawdashline", gclc_drawdashline},
    {"drawvector", gclc_drawvector},
    {"drawcircle", gclc_drawcircle},
    {"drawdashcircle", gclc_drawdashcircle},
    {"drawarc", gclc_drawarc},
    {"drawdasharc", gclc_drawdasharc},
    {"drawarc_p", gclc_drawarc_p},
    {"drawdasharc_p", gclc_drawdasharc_p},
    {"drawellipse", gclc_drawellipse},
    {"drawdashellipse", gclc_drawdashellipse},
    {"drawellipsearc", gclc_drawellipsearc},
    {"drawdashellipsearc", gclc_drawdashellipsearc},
    {"drawellipsearc1", gclc_drawellipsearc1},
    {"drawdashellipsearc1", gclc_drawdashellipsearc1},
    {"drawellipsearc2", gclc_drawellipsearc2},
    {"drawdashellipsearc2", gclc_drawdashellipsearc2},
    {"drawpolygon", gclc_drawpolygon},
    {"layer", gclc_getlayer},
    {"hide_layer", gclc_hidelayer},
    {"hide_layers_from", gclc_hidelayersfrom},
    {"hide_layers_to", gclc_hidelayersto},
    {"filltriangle", gclc_filltriangle},
    {"fillrectangle", gclc_fillrectangle},
    {"fillcircle", gclc_fillcircle},
    {"fillellipse", gclc_fillellipse},
    {"fillarc", gclc_fillarc},
    {"fillarc0", gclc_fillarc0},
    {"fillellipsearc", gclc_fillellipsearc},
    {"fillellipsearc0", gclc_fillellipsearc0},
    {"drawarrow", gclc_drawarrow},
    {"drawbezier3", gclc_drawbezier3},
    {"drawdashbezier3", gclc_drawdashbezier3},
    {"drawbezier4", gclc_drawbezier4},
    {"drawdashbezier4", gclc_drawdashbezier4},
    {"midpoint", gclc_midpoint},
    {"perp", gclc_perp},
    {"perpendicular", gclc_perp},
    {"parallel", gclc_parallel},
    {"getcenter", gclc_getcenter},
    {"online", gclc_online},
    {"onsegment", gclc_onsegment},
    {"oncircle", gclc_oncircle},
    {"bis", gclc_bis},
    {"bisector", gclc_bis},
    {"med", gclc_med},
    {"mediatrice", gclc_med},
    {"%", gclc_comment},
    {"distance", gclc_distance},
    {"sim", gclc_sim},
    {"symmetrical", gclc_sim},
    {"translate", gclc_translate},
    {"rotate", gclc_rotate},
    {"rotateonellipse", gclc_rotateonellipse},
    {"turtle", gclc_turtle},
    {"towards", gclc_towards},
    {"circleprecision", gclc_circleprecision},
    {"bezierprecision", gclc_bezierprecision},
    {"linethickness", gclc_linethickness},
    {"dash", gclc_dash},
    {"dashstyle", gclc_dashstyle},
    {"arrowstyle", gclc_arrow_style},
    {"dmc", gclc_dmc},
    {"mcr", gclc_mcr},
    {"mcp", gclc_mcp},
    {"normal", gclc_normal},
    {"double", gclc_double},
    {"export_to_latex", gclc_export_to_generic_latex},
    {"export_to_simple_latex", gclc_export_to_simple_latex},
    {"export_to_pstricks", gclc_export_to_pstricks},
    {"export_to_tikz", gclc_export_to_tikz},
    {"export_to_eps", gclc_export_to_eps},
    {"export_to_svg", gclc_export_to_svg},
    {"cmark", gclc_cmark},
    {"cmark_lt", gclc_cmark_lt},
    {"cmark_l", gclc_cmark_l},
    {"cmark_lb", gclc_cmark_lb},
    {"cmark_t", gclc_cmark_t},
    {"cmark_b", gclc_cmark_b},
    {"cmark_rt", gclc_cmark_rt},
    {"cmark_r", gclc_cmark_r},
    {"cmark_rb", gclc_cmark_rb},
    {"mark", gclc_mark},
    {"mark_lt", gclc_mark_lt},
    {"mark_l", gclc_mark_l},
    {"mark_lb", gclc_mark_lb},
    {"mark_t", gclc_mark_t},
    {"mark_b", gclc_mark_b},
    {"mark_rt", gclc_mark_rt},
    {"mark_r", gclc_mark_r},
    {"mark_rb", gclc_mark_rb},
    {"printat", gclc_printat},
    {"printat_lt", gclc_printat_lt},
    {"printat_l", gclc_printat_l},
    {"printat_lb", gclc_printat_lb},
    {"printat_t", gclc_printat_t},
    {"printat_b", gclc_printat_b},
    {"printat_rt", gclc_printat_rt},
    {"printat_r", gclc_printat_r},
    {"printat_rb", gclc_printat_rb},
    {"printvalueat", gclc_printvalueat},
    {"printvalueat_lt", gclc_printvalueat_lt},
    {"printvalueat_l", gclc_printvalueat_l},
    {"printvalueat_lb", gclc_printvalueat_lb},
    {"printvalueat_t", gclc_printvalueat_t},
    {"printvalueat_b", gclc_printvalueat_b},
    {"printvalueat_rt", gclc_printvalueat_rt},
    {"printvalueat_r", gclc_printvalueat_r},
    {"printvalueat_rb", gclc_printvalueat_rb},

    {"ang_picture", gclc_ang_picture},
    {"ang_origin", gclc_ang_origin},
    {"ang_unit", gclc_ang_unit},
    {"ang_scale", gclc_ang_scale},
    {"ang_point", gclc_ang_point},
    {"ang_line", gclc_ang_line},
    {"ang_conic", gclc_ang_conic},
    {"ang_conicprecision", gclc_ang_conicprecision},
    {"ang_intersec2", gclc_ang_intersec2},
    {"ang_intersection2", gclc_ang_intersec2},
    {"ang_drawline", gclc_ang_drawline},
    {"ang_drawline_p", gclc_ang_drawline_p},
    {"ang_drawsystem_p", gclc_ang_drawsystem_p},
    {"ang_drawsystem", gclc_ang_drawsystem},
    {"ang_drawsystem0", gclc_ang_drawsystem0},
    {"ang_drawsystem1", gclc_ang_drawsystem1},
    {"ang_drawsystem_a", gclc_ang_drawsystem_a},
    {"ang_drawsystem0_a", gclc_ang_drawsystem0_a},
    {"ang_drawsystem1_a", gclc_ang_drawsystem1_a},
    {"ang_drawconic", gclc_ang_drawconic},
    {"ang_drawdashconic", gclc_ang_drawdashconic},
    {"ang_draw_parametric_curve", gclc_ang_draw_parametric_curve},
    {"ang_tangent", gclc_ang_tangent},
    {"ang_plot_data", gclc_ang_plotdata},
    {"ang_getx", gclc_ang_getx},
    {"ang_gety", gclc_ang_gety},

    {"ang3d_picture", gclc_ang3d_picture},
    {"ang3d_origin", gclc_ang3d_origin},
    {"ang3d_scale", gclc_ang3d_scale},
    {"ang3d_unit", gclc_ang3d_unit},
    {"ang3d_point", gclc_ang3d_point},
    {"ang3d_axes_drawing_range", gclc_ang3d_axes_drawing_range},
    {"ang3d_drawsystem_p", gclc_ang3d_drawsystem_p},
    {"ang3d_draw_parametric_surface", gclc_ang3d_draw_parametric_surface},
    {"ang3d_draw_parametric_curve", gclc_ang3d_draw_parametric_curve},
    {"ang3d_drawline_p", gclc_ang3d_drawline_p},
    {"ang3d_getx", gclc_ang3d_getx},
    {"ang3d_gety", gclc_ang3d_gety},
    {"ang3d_getz", gclc_ang3d_getz},

    {"dim", gclc_dim},
    {"color", gclc_color},
    {"background", gclc_background},
    {"area", gclc_area},
    {"getx", gclc_getx},
    {"gety", gclc_gety},
    {"fontsize", gclc_fontsize},
    {"foot", gclc_foot},

    {"animation_frames", gclc_animation_frames},
    {"trace", gclc_animation_trace_point},

    {"prove", gclc_conjecture},
    {"prooflevel", gclc_proof_level},
    {"prooflimit", gclc_proof_limit},
    {"prover_timeout", gclc_prover_timeout},
    {"theorem_name", gclc_theorem_name},
    {"procedure", gclc_procedure},
    {"call", gclc_procedure_call},

    {"include", gclc_include},

    {"drawtree", gclc_draw_tree},
    {"drawgraph_a", gclc_draw_graph_a},
    {"drawgraph_b", gclc_draw_graph_b}

};

// ----------------------------------------------------------------------------////

CGCLC::CGCLC(CGCLCInput &input, CGCLCLog &Log, prover_config &ProverConfig,
             bool bXMLoutput, std::ofstream &hXML)
    : m_Input(&input), m_Log(Log), m_bXMLOutput(bXMLoutput), m_hXMLOutput(hXML),
      m_ProverConfig(ProverConfig) {
  m_bWhileBlockOrProcedureCall = false;
  m_bProcedureCall = false;
  m_pSymbolTable = new std::map<std::string, GCLC_object>;
  m_pProcedures = new std::map<std::string, GCLCprocedure>;
  m_bSupressWarnings = false;
  Init();
  InitTheoremProver(ProverConfig.TheoremProvingMethod);
  if (m_bXMLOutput)
    InitXML();
}

//----------------------------------------------------------------------------

CGCLC::CGCLC(CGCLCInput &input, CGCLCLog &Log, CIntermediateRepresentation *pL,
             std::map<std::string, GCLCprocedure> *procedures,
             std::map<std::string, GCLC_object> *pTable, bool bXMLoutput,
             std::ofstream &hXMLOutput)
    : CGCompiler(pL), m_Input(&input), m_Log(Log), m_pProcedures(procedures),
      m_bXMLOutput(bXMLoutput), m_hXMLOutput(hXMLOutput) {
  m_bWhileBlockOrProcedureCall = true;
  m_bSupressWarnings = true;
  m_bProcedureCall = (pTable == NULL);
  m_pSymbolTable = (m_bProcedureCall ? new std::map<std::string, GCLC_object> : pTable);
  Init();
  InitXML();
}

// ----------------------------------------------------------------------------////

CGCLC::~CGCLC() { CleanUp(); }

// ----------------------------------------------------------------------------////

void CGCLC::Init() {
  m_Basic_Area = {0, 0, 0, 0};
  CIRCLE_PRECISION = 72;
  MARK_CIRCLE_PRECISION = 9;
  MARK_RADIUS = 0.40;
  DM = 1.00; /* distance point-mark */

  ARROW_ANGLE = 15.0;
  ARROW_LENGTH = 3.0;
  ARROW_INNER_TOWARDS = 2.0 / 3.0;

  BEZIER_PRECISION = 36;

  m_2D_Area = {0, 0, 0, 0};
  X_ORIGIN = 0.00;
  Y_ORIGIN = 0.00;
  UNIT = 10.00;
  ANG_SCALE = 1.00;
  ANG_LOG = 1;
  CONIC_PRECISION = 144;

  m_3D_Area = {0, 0, 0, 0};
  ANG3D_AXES_LIMITED = false;
  X3D_ORIGIN = 0.00;
  Y3D_ORIGIN = 0.00;
  SIN_ALPHA3D = 0.00;
  SIN_BETA3D = 0.00;
  COS_ALPHA3D = 1.00;
  COS_BETA3D = 1.00;
  UNIT3D = 10.00;
  ANG3D_SCALE_Y = 1.00;
  ANG3D_SCALE_Z = 1.00;
  ANG3D_LOG = 1;
  X3D_MIN = 0.00;
  X3D_MAX = 0.00;
  Y3D_MIN = 0.00;
  Y3D_MAX = 0.00;
  Z3D_MIN = 0.00;
  Z3D_MAX = 0.00;

  m_nAnimationFrames = 0;
  m_nAnimationSpeed = 0;
}

// ----------------------------------------------------------------------------////

void CGCLC::SetEnv(GCLC_env &e) {
  m_Basic_Area = e.m_Basic_Area;
  CIRCLE_PRECISION = e.CIRCLE_PRECISION;
  MARK_CIRCLE_PRECISION = e.MARK_CIRCLE_PRECISION;
  MARK_RADIUS = e.MARK_RADIUS;
  DM = e.DM;

  ARROW_ANGLE = e.ARROW_ANGLE;
  ARROW_LENGTH = e.ARROW_LENGTH;
  ARROW_INNER_TOWARDS = e.ARROW_INNER_TOWARDS;

  BEZIER_PRECISION = e.BEZIER_PRECISION;

  m_2D_Area = e.m_2D_Area;
  X_ORIGIN = e.X_ORIGIN;
  Y_ORIGIN = e.Y_ORIGIN;
  UNIT = e.UNIT;
  ANG_SCALE = e.ANG_SCALE;
  ANG_LOG = e.ANG_LOG;
  CONIC_PRECISION = e.CONIC_PRECISION;

  m_3D_Area = e.m_3D_Area;
  ANG3D_AXES_LIMITED = e.ANG3D_AXES_LIMITED;
  X3D_ORIGIN = e.X3D_ORIGIN;
  Y3D_ORIGIN = e.Y3D_ORIGIN;
  SIN_ALPHA3D = e.SIN_ALPHA3D;
  SIN_BETA3D = e.SIN_BETA3D;
  COS_ALPHA3D = e.COS_ALPHA3D;
  COS_BETA3D = e.COS_BETA3D;
  UNIT3D = e.UNIT3D;
  ANG3D_SCALE_Y = e.ANG3D_SCALE_Y;
  ANG3D_SCALE_Z = e.ANG3D_SCALE_Z;
  ANG3D_LOG = e.ANG3D_LOG;
  X3D_MIN = e.X3D_MIN;
  X3D_MAX = e.X3D_MAX;
  Y3D_MIN = e.Y3D_MIN;
  Y3D_MAX = e.Y3D_MAX;
  Z3D_MIN = e.Z3D_MIN;
  Z3D_MAX = e.Z3D_MAX;
}

// ----------------------------------------------------------------------------////

void CGCLC::GetEnv(GCLC_env &e) const {
  e.m_Basic_Area = m_Basic_Area;
  e.CIRCLE_PRECISION = CIRCLE_PRECISION;
  e.MARK_CIRCLE_PRECISION = MARK_CIRCLE_PRECISION;
  e.MARK_RADIUS = MARK_RADIUS;
  e.DM = DM;

  e.ARROW_ANGLE = ARROW_ANGLE;
  e.ARROW_LENGTH = ARROW_LENGTH;
  e.ARROW_INNER_TOWARDS = ARROW_INNER_TOWARDS;

  e.BEZIER_PRECISION = BEZIER_PRECISION;

  e.m_2D_Area = m_2D_Area;
  e.X_ORIGIN = X_ORIGIN;
  e.Y_ORIGIN = Y_ORIGIN;
  e.UNIT = UNIT;
  e.ANG_SCALE = ANG_SCALE;
  e.ANG_LOG = ANG_LOG;
  e.CONIC_PRECISION = CONIC_PRECISION;

  e.m_3D_Area = m_3D_Area;
  e.ANG3D_AXES_LIMITED = ANG3D_AXES_LIMITED;
  e.X3D_ORIGIN = X3D_ORIGIN;
  e.Y3D_ORIGIN = Y3D_ORIGIN;
  e.SIN_ALPHA3D = SIN_ALPHA3D;
  e.SIN_BETA3D = SIN_BETA3D;
  e.COS_ALPHA3D = COS_ALPHA3D;
  e.COS_BETA3D = COS_BETA3D;
  e.UNIT3D = UNIT3D;
  e.ANG3D_SCALE_Y = ANG3D_SCALE_Y;
  e.ANG3D_SCALE_Z = ANG3D_SCALE_Z;
  e.ANG3D_LOG = ANG3D_LOG;
  e.X3D_MIN = X3D_MIN;
  e.X3D_MAX = X3D_MAX;
  e.Y3D_MIN = Y3D_MIN;
  e.Y3D_MAX = Y3D_MAX;
  e.Z3D_MIN = Z3D_MIN;
  e.Z3D_MAX = Z3D_MAX;
}

// ----------------------------------------------------------------------------////

void CGCLC::CleanUp() {
  CGCompiler::CleanUp();
  if (!m_bWhileBlockOrProcedureCall)
    delete m_pProcedures;
  if (!m_bWhileBlockOrProcedureCall || m_bProcedureCall)
    delete m_pSymbolTable;
}

// ----------------------------------------------------------------------------////

GReturnValue CGCLC::Import(eGCLC_conjecture_status &prover_output,
                           double &prover_time) {
  assert(!m_bWhileBlockOrProcedureCall);
  m_iErrorCode = rvGCLCNoError;
  ResetLog();
  AddToLog("GCLC 2024 " GCLC_VERSION "\n");
  AddToLog("Copyright (c) 1995-2024 by Predrag Janicic, University of "
           "Belgrade.\n");
  AddToLog("Licensed under the Creative Commons licence CC BY-ND.\n");
  // AddToLog("\n");
  // AddToLog("\n");

  if (m_bXMLOutput) {
    Print(m_hXMLOutput, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    Print(m_hXMLOutput, "<!DOCTYPE figure SYSTEM \"GeoCons.dtd\">\n");
    Print(m_hXMLOutput,
          "<?xml-stylesheet href=\"GeoConsHTML.xsl\" type=\"text/xsl\"?>\n\n");
    Print(m_hXMLOutput, "<figure>\n");
  }

  enum GCLCError iErrorCode = Execute();
  if (!m_bWhileBlockOrProcedureCall)
    view();

  std::string sErrorMessage;
  int line, pos, i;
  if ((iErrorCode != rvGCLCOK) &&
      GetError(i, sErrorMessage, line, pos) == rvG_OK) {

    AddToLog("\nError " + i2s(i) + ": " + sErrorMessage);
    AddToLog(" (Line: " + i2s(line) + ", position: " + i2s(pos) + ")");
    AddToLog("\nFile not processed.\n");

    if (ProvingTheorem()) {
      if (m_ProverConfig.bDeductionControl) {
        if (ValidConjecture()) {
          AddToLog("\nDeduction check invoked: the property that led to the "
                   "error will be tested for validity.\n");
          std::string sProofLaTeXOutput, sProofXMLOutput;
          if (m_ProverConfig.bLaTeX)
            sProofLaTeXOutput = "error-proof.tex";
          if (m_ProverConfig.bXML)
            sProofXMLOutput = "error-proof.xml";
          double Time;
          /* GReturnValue r = */ Prove(sProofLaTeXOutput, sProofXMLOutput, Time,
                                       "Deduction check conjecture",
                                       prover_output);
          AddToLog("\n\nTime spent by the prover: " + d2s(Time/(double)1000, 3) +
                   " seconds");
          if (prover_output == e_proved)
            AddToLog("\nThe conjecture successfully proved - the critical "
                     "property always holds.");
          else if (prover_output == e_unknown)
            AddToLog("\nThe conjecture not proved - the critical property "
                     "does not always hold.");
          else if (prover_output == e_unknown_toomanysteps)
            AddToLog("\nThe conjecture not proved - maximal number of proof "
                     "steps reached.");
          else if (prover_output == e_unknown_timeout)
            AddToLog("\nThe conjecture not proved - timeout.");
          else
            AddToLog("\nThe conjecture out of scope of the prover.");

          if (sProofLaTeXOutput != "")
            AddToLog("\nThe prover output is written in the file "
                     "error-proof.tex.\n");
          if (sProofXMLOutput != "")
            AddToLog("\nThe prover output is written in the file "
                     "error-proof.xml.\n");
        }
      }
      CleanUpProver();
    } else {
      prover_output = e_idle;
      prover_time = 0;
    }
    return rvG_ErrorInInput;
  }

  AddToLog("\nFile successfully processed.\n");

  // support for the automated theorem prover
  if (m_ProverConfig.TheoremProvingMethod != tpNone) {
    if (ValidConjecture()) {
      std::string sLaTeXFile, sXMLFile;
      if (m_ProverConfig.bLaTeX)
        sLaTeXFile = m_ProverConfig.sTheoremFileName + "_proof.tex";
      if (m_ProverConfig.bXML)
        sXMLFile = m_ProverConfig.sTheoremFileName + "_proof.xml";

      if (m_ProverConfig.m_sTheoremName.size() == 0)
        m_ProverConfig.m_sTheoremName = m_ProverConfig.sTheoremFileName;

      GReturnValue r = Prove(sLaTeXFile, sXMLFile, prover_time,
                             m_ProverConfig.m_sTheoremName, prover_output);
      if (r == rvG_CannotOpenOutputFile) {
        AddToLog("File error. Cannot open output file (" + sLaTeXFile + " or " +
                 sXMLFile + ") for the proof.\n");
        CleanUpProver();
        return rvG_CannotOpenOutputFile;
      }
      AddToLog("\n\nTime spent by the prover: " + d2s(prover_time/(double)1000, 3) +
               " seconds.");
      if (prover_output != e_conjecture_out_of_scope && prover_output != e_construction_out_of_scope) {
        AddToLog(GetMethodSpecificOutput());
      }

      switch (prover_output) {
      case e_proved:
        AddToLog("\nThe conjecture successfully proved.");
        break;
      case e_disproved:
        AddToLog("\nThe conjecture disproved.");
        break;
      case e_conjecture_out_of_scope:
        AddToLog("\nThe conjecture out of scope of the prover.");
        break;
      case e_construction_out_of_scope:
        AddToLog("\nThe construction out of scope of the prover.");
        break;
      case e_unknown:
        AddToLog("\nThe conjecture not proved.");
        break;
      case e_unknown_toomanysteps:
        AddToLog("\nThe conjecture not proved - maximal number of proof steps "
                 "reached.");
        break;
      case e_unknown_timeout:
        AddToLog("\nThe conjecture not proved - timeout.");
        break;
      default:
        break;
      }

      if (prover_output != e_conjecture_out_of_scope) {
        if (sLaTeXFile != "")
          AddToLog("\nThe prover output is written in the file " + sLaTeXFile +
                   ".\n");
        if (sXMLFile != "")
          AddToLog("\nThe prover output is written in the file " + sXMLFile +
                   ".\n");
      }
    }
  }

  if (m_bXMLOutput) {
    ChangeCurrentXMLGroup(eXMLnone);
    Print(m_hXMLOutput, "</figure>");
  }

  CleanUpProver();
  // end of support for the automated theorem prover

  return rvG_OK;
}

// ----------------------------------------------------------------------------////

GReturnValue CGCLC::Import() {
  assert(m_bWhileBlockOrProcedureCall);
  m_iErrorCode = rvGCLCNoError;
  if (m_bXMLOutput && !m_bWhileBlockOrProcedureCall) {
    Print(m_hXMLOutput, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    Print(m_hXMLOutput, "<!DOCTYPE figure SYSTEM \"GeoCons.dtd\">\n");
    Print(m_hXMLOutput,
          "<?xml-stylesheet href=\"GeoConsHTML.xsl\" type=\"text/xsl\"?>\n\n");
    Print(m_hXMLOutput, "<figure>\n");
  }
  if (Execute() != rvGCLCOK) {
    std::string sErrorMessage;
    int line, pos, i;
    if (GetError(i, sErrorMessage, line, pos) == rvG_OK)
      return rvG_ErrorInInput;
  }
  if (m_bXMLOutput) {
    ChangeCurrentXMLGroup(eXMLnone);
    if (!m_bWhileBlockOrProcedureCall)
      Print(m_hXMLOutput, "</figure>");
  }
  return rvG_OK;
}

// ----------------------------------------------------------------------------////

GReturnValue CGCLC::GetError(int &iErrorCode, std::string &sErrMessage, int &line,
                             int &pos) {
  if ((m_iErrorCode == rvGCLCNoError) || (m_iErrorCode == rvGCLCOK)) {
    pos = m_iWarnPos;
    line = m_iWarnLine;
    return rvG_Error;
  }
  pos = m_iPositionBeforeLastToken;
  line = m_iLineBeforeLastToken;

  iErrorCode = (int)m_iErrorCode;
  switch (m_iErrorCode) {
  case rvGCLCNumberExpected:
    sErrMessage = "Syntax error: Number expected.";
    break;
  case rvGCLCTwoDecimalPoints:
    sErrMessage = "Syntax error: Two decimal points in number.";
    break;
  case rvGCLCIdExpected:
    sErrMessage = "Syntax error: Identifier expected.";
    break;
  case rvGCLCIdOrNumberExpected:
    sErrMessage = "Syntax error: Identifier or number expected.";
    break;
  case rvGCLCUndefinedVariable:
    sErrMessage = "Syntax error: Undefined variable.";
    break;
  case rvGCLCWrongType:
    sErrMessage = "Syntax error: Wrong variable type.";
    break;
  case rvGCLCLBracketExpected:
    sErrMessage = "Syntax error: Symbol '{' expected.";
    break;
  case rvGCLCRBracketExpected:
    sErrMessage = "Syntax error: Symbol '}' expected.";
    break;
  case rvGCLCUnknownCommand:
    sErrMessage = "Syntax error: Unrecognized definition or command.";
    break;
  case rvGCLCFixedPointExpected:
    sErrMessage = "Syntax error: Identifier of a fixed point expected.";
    break;
  case rvGCLCPointExpected:
    sErrMessage = "Syntax error: Identifier of a point expected.";
    break;
  case rvGCLCBadLine:
    sErrMessage = "Run-time error: Bad definition. Cannot determine line.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCBadCircle:
    sErrMessage = "Run-time error: Bad definition. Circle radius too small.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCBadEllipse:
    sErrMessage = "Run-time error: Bad definition. Cannot determine ellipse.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCBadIntersection:
    sErrMessage =
        "Run-time error: Bad definition. Can not determine intersection.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCBadAngle:
    sErrMessage = "Run-time error: Bad definition. Can not determine angle.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCUnknownprocedure:
    sErrMessage = "Syntax error: Unknown procedure.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCParametersNotMatched:
    sErrMessage = "Syntax error: Parameters lists not matched.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCNoProceduresInBlocks:
    sErrMessage = "Syntax erros: Procedures cannot be defined within "
                  "while-blocks or procedures.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCInvalidProcedure:
    sErrMessage = "Invalid procedure definition.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCInvalidIncludeFile:
    sErrMessage = "Invalid include file.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCLIllFormedTree:
    sErrMessage = "Syntax error: Invalid tree description.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCLInvalidGraphRepresentation:
    sErrMessage = "Syntax error: Invalid graph description.";
    pos = m_iWarnPos;
    line = m_iWarnLine;
    break;
  case rvGCLCInvalidInput:
    sErrMessage = "Input error: Invalid input.";
    break;
  case rvCannotExport:
    sErrMessage = "Run-time error: Cannot export data.";
    break;
  case rvGCLCOutOfMemory:
    sErrMessage = "Memory error: Cannot allocate enough memory.";
    break;
  case rvGCLCInvalidExpression:
    sErrMessage = "Invalid or non-defined expression.";
    break;
  case rvGCLCSeparatorExpected:
    sErrMessage = "Symbol ';' expected.";
    break;
  case rvGCLCInvalidWhileBlock:
    sErrMessage = "Invalid while block.";
    break;
  case rvGCLCInvalidIfBlock:
    sErrMessage = "Invalid if-then-else block.";
    break;
  case rvGCLCToManyWhileBlockExecutions:
    sErrMessage = "Too many while-block executions (more than 10000). "
                  "Possible infinite loop.";
    break;
  case rvGCLCInvalidConjecture:
    sErrMessage = "The conjecture given to prove is ill-formed or includes a "
                  "point that is not constructed by commands supported by the "
                  "prover.";
    break;
  default:
    sErrMessage = "Unknown error.";
    break;
  }
  return rvG_OK;
}

// ----------------------------------------------------------------------------////

int CGCLC::warning(int w) {
  if (m_bSupressWarnings)
    return rvGCLCOK;

  if (w == rvGCLCChangingValue) {
    AddToLog("Warning: Changing variable value (Line: " + i2s(m_iWarnLine) +
             ", position: " + i2s(m_iWarnPos) + ").\n");
  } else // rvGCLCChangingType
  {
    AddToLog("Warning: Changing variable type (Line: " + i2s(m_iWarnLine) +
             ", position: " + i2s(m_iWarnPos) + ").\n");
  }
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------////

GCLCError CGCLC::take_text(std::string &v) {
  unsigned char c;
  GReturnValue iRv;
  int brackets;
  v = "";

  do {
    iRv = m_Input->ReadChar(c);
    if (iRv != rvG_OK)
      return rvGCLCLBracketExpected;
    if (iRv == rvG_InvalidInput)
      return rvGCLCInvalidInput;
  } while ((c == ' ') || (c == '\n') || (c == 9) || (c == '\t') || (c == '\r'));

  m_iLineBeforeLastToken = m_Input->GetCurrentLine();
  m_iPositionBeforeLastToken = m_Input->GetCurrentPosition();
  // RecordPositionForWarning();

  if (c != '{')
    return rvGCLCLBracketExpected;
  iRv = m_Input->ReadChar(c);
  if (iRv == rvG_EndOfData)
    return rvGCLCRBracketExpected;
  if (iRv == rvG_InvalidInput)
    return rvGCLCInvalidInput;
  brackets = 1;
  for (;;) {
    if (iRv == rvG_EndOfData)
      return rvGCLCRBracketExpected;
    if (iRv == rvG_InvalidInput)
      return rvGCLCInvalidInput;
    if (c == '{') {
      v += '{';
      brackets++;
    } else {
      if (c == '}' && --brackets == 0) {
        // RecordPositionForWarning();
        return rvGCLCOK;
      } else
        v += c;
    }
    iRv = m_Input->ReadChar(c);
  }
}

// ----------------------------------------------------------------------------////

enum GCLCError CGCLC::Execute() {
  enum GCLCError err = rvGCLCOK;
  m_iErrorCode = rvGCLCOK;

  GCLCcommands com;
  int iArgs;
  bool bReadMore = false;

  for (;;) {
    if (EndOfInputData())
      return rvGCLCOK;
    if (!bReadMore)
      err = ReadToken();
    else
      err = rvGCLCOK;
    bReadMore = false;

    if (LastToken() == "")
      return rvGCLCOK;

    if ((err == rvGCLCOK) || (err == rvGCLCEndOfInput)) {
      com = choose_command(LastToken());
      switch (com) {
      // Basic definitions
      case gclc_number:
        err = get_number();
        break;
      case gclc_point:
        err = get_point(iArgs);
        bReadMore = (iArgs == 2);
        break;
      case gclc_line:
        err = get_line();
        break;
      case gclc_circle:
        err = get_circle();
        break;
      case gclc_set_equal:
        err = set_equal();
        break;

      // Basic constructions
      case gclc_intersec:
        err = get_intersec();
        break;
      case gclc_intersec2:
        err = get_intersec2();
        break;
      case gclc_midpoint:
        err = get_midpoint();
        break;
      case gclc_med:
        err = get_med();
        break;
      case gclc_bis:
        err = get_bis();
        break;
      case gclc_perp:
        err = get_perp();
        break;
      case gclc_foot:
        err = get_foot();
        break;
      case gclc_parallel:
        err = get_parallel();
        break;
      case gclc_getcenter:
        err = get_getcenter();
        break;
      case gclc_onsegment:
        err = get_onsegment();
        break;
      case gclc_online:
        err = get_online();
        break;
      case gclc_oncircle:
        err = get_oncircle();
        break;

      // Transformations
      case gclc_translate:
        err = get_translate();
        break;
      case gclc_rotate:
        err = get_rotate();
        break;
      case gclc_rotateonellipse:
        err = get_rotateonellipse();
        break;
      case gclc_sim:
        err = get_sim();
        break;
      case gclc_turtle:
        err = get_turtle();
        break;
      case gclc_towards:
        err = get_towards();
        break;

      // Calculations, expressions, arrays, and control structures
      case gclc_getx:
        err = get_x();
        break;
      case gclc_gety:
        err = get_y();
        break;
      case gclc_distance:
        err = get_distance();
        break;
      case gclc_angle:
        err = get_angle(0);
        break;
      case gclc_angle_o:
        err = get_angle(1);
        break;
      case gclc_random:
        err = get_random();
        break;
      case gclc_expression:
        err = get_expression();
        break;
      case gclc_array:
        err = get_array();
        break;
      case gclc_while:
        err = get_while();
        break;
      case gclc_if_then_else:
        err = get_if_then_else();
        break;
      case gclc_procedure:
        err = get_procedure();
        break;
      case gclc_procedure_call:
        err = get_call_procedure();
        break;
      case gclc_include:
        err = get_include();
        break;

      // Drawing and Filling commands
      case gclc_drawpoint:
        err = draw_point();
        break;
      case gclc_drawsegment:
        err = draw_segment(false);
        break;
      case gclc_drawdashsegment:
        err = draw_segment(true);
        break;
      case gclc_drawline:
        err = draw_line(false);
        break;
      case gclc_drawdashline:
        err = draw_line(true);
        break;
      case gclc_drawvector:
        err = draw_vector();
        break;
      case gclc_drawarrow:
        err = draw_arrow();
        break;
      case gclc_drawcircle:
        err = draw_circle(false);
        break;
      case gclc_drawdashcircle:
        err = draw_circle(true);
        break;
      case gclc_drawarc:
        err = draw_arc(false, false);
        break;
      case gclc_drawdasharc:
        err = draw_arc(true, false);
        break;
      case gclc_drawarc_p:
        err = draw_arc(false, true);
        break;
      case gclc_drawdasharc_p:
        err = draw_arc(true, true);
        break;
      case gclc_drawellipse:
        err = draw_ellipse(false);
        break;
      case gclc_drawdashellipse:
        err = draw_ellipse(true);
        break;
      case gclc_drawellipsearc:
        err = draw_ellipsearc(false, false, false);
        break;
      case gclc_drawdashellipsearc:
        err = draw_ellipsearc(true, false, false);
        break;
      case gclc_drawellipsearc1:
        err = draw_ellipsearc(false, true, false);
        break;
      case gclc_drawdashellipsearc1:
        err = draw_ellipsearc(true, true, false);
        break;
      case gclc_drawellipsearc2:
        err = draw_ellipsearc(false, false, true);
        break;
      case gclc_drawdashellipsearc2:
        err = draw_ellipsearc(true, false, true);
        break;
      case gclc_drawbezier3:
        err = draw_bezier3(false);
        break;
      case gclc_drawdashbezier3:
        err = draw_bezier3(true);
        break;
      case gclc_drawbezier4:
        err = draw_bezier4(false);
        break;
      case gclc_drawdashbezier4:
        err = draw_bezier4(true);
        break;
      case gclc_drawpolygon:
        err = draw_polygon();
        break;
      case gclc_draw_tree:
        err = draw_tree();
        break;
      case gclc_draw_graph_a:
        err = draw_graph_a();
        break;
      case gclc_draw_graph_b:
        err = draw_graph_b();
        break;
      case gclc_filltriangle:
        err = fill_triangle();
        break;
      case gclc_fillrectangle:
        err = fill_rectangle();
        break;
      case gclc_fillcircle:
        err = fill_circle();
        break;
      case gclc_fillellipse:
        err = fill_ellipse();
        break;
      case gclc_fillarc:
        err = fill_arc(true);
        break;
      case gclc_fillellipsearc:
        err = fill_ellipsearc(true);
        break;
      case gclc_fillarc0:
        err = fill_arc(false);
        break;
      case gclc_fillellipsearc0:
        err = fill_ellipsearc(false);
        break;

      // Labeling and Printing commands
      case gclc_cmark:
        err = cmark(0);
        break;
      case gclc_cmark_lt:
        err = cmark(GCLC_DIR_LT);
        break;
      case gclc_cmark_l:
        err = cmark(GCLC_DIR_L);
        break;
      case gclc_cmark_lb:
        err = cmark(GCLC_DIR_LB);
        break;
      case gclc_cmark_t:
        err = cmark(GCLC_DIR_T);
        break;
      case gclc_cmark_b:
        err = cmark(GCLC_DIR_B);
        break;
      case gclc_cmark_rt:
        err = cmark(GCLC_DIR_RT);
        break;
      case gclc_cmark_r:
        err = cmark(GCLC_DIR_R);
        break;
      case gclc_cmark_rb:
        err = cmark(GCLC_DIR_RB);
        break;
      case gclc_mark:
        err = mark(0);
        break;
      case gclc_mark_lt:
        err = mark(GCLC_DIR_LT);
        break;
      case gclc_mark_l:
        err = mark(GCLC_DIR_L);
        break;
      case gclc_mark_lb:
        err = mark(GCLC_DIR_LB);
        break;
      case gclc_mark_t:
        err = mark(GCLC_DIR_T);
        break;
      case gclc_mark_b:
        err = mark(GCLC_DIR_B);
        break;
      case gclc_mark_rt:
        err = mark(GCLC_DIR_RT);
        break;
      case gclc_mark_r:
        err = mark(GCLC_DIR_R);
        break;
      case gclc_mark_rb:
        err = mark(GCLC_DIR_RB);
        break;
      case gclc_printat:
        err = printat(0);
        break;
      case gclc_printat_lt:
        err = printat(GCLC_DIR_LT);
        break;
      case gclc_printat_l:
        err = printat(GCLC_DIR_L);
        break;
      case gclc_printat_lb:
        err = printat(GCLC_DIR_LB);
        break;
      case gclc_printat_t:
        err = printat(GCLC_DIR_T);
        break;
      case gclc_printat_b:
        err = printat(GCLC_DIR_B);
        break;
      case gclc_printat_rt:
        err = printat(GCLC_DIR_RT);
        break;
      case gclc_printat_r:
        err = printat(GCLC_DIR_R);
        break;
      case gclc_printat_rb:
        err = printat(GCLC_DIR_RB);
        break;
      case gclc_printvalueat:
        err = printvalueat(0);
        break;
      case gclc_printvalueat_lt:
        err = printvalueat(GCLC_DIR_LT);
        break;
      case gclc_printvalueat_l:
        err = printvalueat(GCLC_DIR_L);
        break;
      case gclc_printvalueat_lb:
        err = printvalueat(GCLC_DIR_LB);
        break;
      case gclc_printvalueat_t:
        err = printvalueat(GCLC_DIR_T);
        break;
      case gclc_printvalueat_b:
        err = printvalueat(GCLC_DIR_B);
        break;
      case gclc_printvalueat_rt:
        err = printvalueat(GCLC_DIR_RT);
        break;
      case gclc_printvalueat_r:
        err = printvalueat(GCLC_DIR_R);
        break;
      case gclc_printvalueat_rb:
        err = printvalueat(GCLC_DIR_RB);
        break;

      // Low level commands
      case gclc_comment:
        err = get_comment();
        break;
      case gclc_dim:
        err = dim();
        break;
      case gclc_area:
        err = get_area();
        break;
      case gclc_color:
        err = get_color();
        break;
      case gclc_background:
        err = get_background();
        break;
      case gclc_fontsize:
        err = get_fontsize();
        break;
      case gclc_arrow_style:
        err = get_arrow_style();
        break;
      case gclc_circleprecision:
        err = get_circleprecision();
        break;
      case gclc_bezierprecision:
        err = get_bezierprecision();
        break;
      case gclc_linethickness:
        err = get_linethickness();
        break;
      case gclc_double:
        err = set_double();
        break;
      case gclc_normal:
        err = set_normal();
        break;
      case gclc_dash:
        err = get_dash();
        break;
      case gclc_dashstyle:
        err = get_dashstyle();
        break;
      case gclc_dmc:
        err = get_dmc();
        break;
      case gclc_mcr:
        err = get_mcr();
        break;
      case gclc_mcp:
        err = get_mcp();
        break;
      case gclc_export_to_generic_latex:
        err = get_export(eGenericLaTeXoutput);
        break;
      case gclc_export_to_simple_latex:
        err = get_export(eLaTeXoutput);
        break;
      case gclc_export_to_pstricks:
        err = get_export(ePSTricksoutput);
        break;
      case gclc_export_to_tikz:
        err = get_export(eTikZoutput);
        break;
      case gclc_export_to_eps:
        err = get_export(eEPSoutput);
        break;
      case gclc_export_to_svg:
        err = get_export(eSVGoutput);
        break;

      // 2D Cartesian commands
      case gclc_ang_picture:
        err = ang_picture();
        break;
      case gclc_ang_origin:
        err = ang_origin();
        break;
      case gclc_ang_unit:
        err = ang_unit();
        break;
      case gclc_ang_scale:
        err = ang_scale();
        break;
      case gclc_ang_drawsystem_p:
        err = ang_draw_system_p();
        break;
      case gclc_ang_drawsystem:
        err = ang_draw_system(1);
        break;
      case gclc_ang_drawsystem0:
        err = ang_draw_system(2);
        break;
      case gclc_ang_drawsystem1:
        err = ang_draw_system(3);
        break;
      case gclc_ang_drawsystem_a:
        err = ang_draw_system(101);
        break;
      case gclc_ang_drawsystem0_a:
        err = ang_draw_system(102);
        break;
      case gclc_ang_drawsystem1_a:
        err = ang_draw_system(103);
        break;
      case gclc_ang_point:
        err = ang_point();
        break;
      case gclc_ang_getx:
        err = ang_getx();
        break;
      case gclc_ang_gety:
        err = ang_gety();
        break;
      case gclc_ang_line:
        err = ang_line();
        break;
      case gclc_ang_conic:
        err = ang_conic();
        break;
      case gclc_ang_intersec2:
        err = ang_intersec2();
        break;
      case gclc_ang_tangent:
        err = ang_tangent();
        break;
      case gclc_ang_drawline:
        err = ang_draw_line();
        break;
      case gclc_ang_drawline_p:
        err = ang_draw_line_p();
        break;
      case gclc_ang_drawconic:
        err = ang_draw_conic(0);
        break;
      case gclc_ang_drawdashconic:
        err = ang_draw_conic(1);
        break;
      case gclc_ang_draw_parametric_curve:
        err = ang_draw_parametric_curve();
        break;
      case gclc_ang_conicprecision:
        err = get_conicprecision();
        break;
      case gclc_ang_plotdata:
        err = ang_plot_data();
        break;

      // 3D Cartesian commands
      case gclc_ang3d_picture:
        err = ang3d_picture();
        break;
      case gclc_ang3d_origin:
        err = ang3d_origin();
        break;
      case gclc_ang3d_scale:
        err = ang3d_scale();
        break;
      case gclc_ang3d_axes_drawing_range:
        err = ang3d_axes_drawing_range();
        break;
      case gclc_ang3d_unit:
        err = ang3d_unit();
        break;
      case gclc_ang3d_drawsystem_p:
        err = ang3d_draw_system_p();
        break;
      case gclc_ang3d_point:
        err = ang3d_point();
        break;
      case gclc_ang3d_getx:
        err = ang3d_getx();
        break;
      case gclc_ang3d_gety:
        err = ang3d_gety();
        break;
      case gclc_ang3d_getz:
        err = ang3d_getz();
        break;
      case gclc_ang3d_drawline_p:
        err = ang3d_draw_line_p();
        break;
      case gclc_ang3d_draw_parametric_surface:
        err = ang3d_draw_parametric_surface();
        break;
      case gclc_ang3d_draw_parametric_curve:
        err = ang3d_draw_parametric_curve();
        break;

      // Layers
      case gclc_getlayer:
        err = get_layer();
        break;
      case gclc_hidelayer:
        err = hide_layer();
        break;
      case gclc_hidelayersfrom:
        err = hide_layersfrom();
        break;
      case gclc_hidelayersto:
        err = hide_layersto();
        break;

      // Animations
      case gclc_animation_frames:
        err = get_animation_frames();
        break;
      case gclc_animation_trace_point:
        err = get_animation_trace_point();
        break;

      // Theorem proving
      case gclc_conjecture:
        err = get_conjecture();
        break;
      case gclc_proof_level:
        err = get_proof_level();
        break;
      case gclc_proof_limit:
        err = get_proof_limit();
        break;
      case gclc_prover_timeout:
        err = get_prover_timeout();
        break;
      case gclc_theorem_name:
        err = get_prover_theorem_name();
        break;

      default:
        m_iErrorCode = rvGCLCUnknownCommand;
        return rvGCLCUnknownCommand;
      }

      if (err != rvGCLCOK) {
        m_iErrorCode = err;
        return err;
      }
    }
  }
}

// ///////////////////////////////////////////////////////////////

enum GCLCcommands CGCLC::choose_command(const std::string &word) {
  auto c = KeyWords.find(word);
  if (c == KeyWords.end())
    return gclc_unknowncommand;
  else
    return c->second;
}

// ----------------------------------------------------------------------------////

int CGCLC::view() {
  std::string sLogText;

  AddToLog("\nObjects:");
  for (std::map<std::string, GCLC_object>::iterator it = m_pSymbolTable->begin();
       it != m_pSymbolTable->end(); it++) {
    AddToLog("\n" + it->first + ":");

    switch (it->second.type) {
    case GCLC_NUMBER:
      sLogText = " NUMBER : (" + d2s(it->second.p[0], 2) + ")";
      break;

    case GCLC_POINT:
      sLogText = " GCLC_POINT : (" + d2s(it->second.p[0], 2) + "," +
                 d2s(it->second.p[1], 2) + ")";
      break;

    case GCLC_LINE:
      sLogText = " GCLC_LINE : (" + d2s(it->second.p[0], 2) + "," +
                 d2s(it->second.p[1], 2) + "," + d2s(it->second.p[2], 2) + ")";
      break;

    case GCLC_CIRCLE:
      sLogText = " GCLC_CIRCLE : (" + d2s(it->second.p[0], 2) + "," +
                 d2s(it->second.p[1], 2) + "," + d2s(it->second.p[2], 2) + ")";
      break;

    case GCLC_CONIC:
      sLogText = " GCLC_CONIC : (" + d2s(it->second.p[0], 2) + "," +
                 d2s(it->second.p[1], 2) + "," + d2s(it->second.p[2], 2) + "," +
                 d2s(it->second.p[3], 2) + "," + d2s(it->second.p[4], 2) + "," +
                 d2s(it->second.p[5], 2) + ")";
      break;

    default:
      break;
    }

    AddToLog(sLogText);
  }
  AddToLog("\n");
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------////

GCLCError CGCLC::Let(const std::string &sVarName, int type, double p1, double p2,
                     double p3, double p4, double p5, double p6)
/* returns rvGCLCOK if variable already exist, warning otherwise */
{
  GCLC_object s;
  std::map<std::string, GCLC_object>::iterator it = m_pSymbolTable->find(sVarName);
  if (it == m_pSymbolTable->end()) // wasn't there already
  {
    s.name = sVarName;
    s.type = type;
    s.p[0] = p1;
    s.p[1] = p2;
    s.p[2] = p3;
    s.p[3] = p4;
    s.p[4] = p5;
    s.p[5] = p6;
    m_pSymbolTable->insert(std::pair<std::string, GCLC_object>(s.name, s));
    return rvGCLCOK;
  }
  if (it->second.type == type)
    warning(rvGCLCChangingValue);
  else
    warning(rvGCLCChangingType);

  it->second.type = type;
  it->second.p[0] = p1;
  it->second.p[1] = p2;
  it->second.p[2] = p3;
  it->second.p[3] = p4;
  it->second.p[4] = p5;
  it->second.p[5] = p6;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------////

/* returns rvGCLCOK if variable exist;
   rvGCLCUndefinedVariable otherwise
   (variable name is not copied to o.name) */
GCLCError CGCLC::Value(const std::string &sVarName, GCLC_object &o) {
  std::map<std::string, GCLC_object>::iterator it = m_pSymbolTable->find(sVarName);
  if (it != m_pSymbolTable->end()) {
    o = (it->second);
    return rvGCLCOK;
  }
  return rvGCLCUndefinedVariable;
}

// ----------------------------------------------------------------------------////

GReturnValue CGCLC::GetValue(const std::string &sVarName, std::string &sValue) {
  std::map<std::string, GCLC_object>::iterator it = m_pSymbolTable->find(sVarName);
  if (it == m_pSymbolTable->end())
    return rvG_InvalidInput;

  switch (it->second.type) {
  case GCLC_NUMBER:
    sValue = " Number : (" + d2s(it->second.p[0], 6) + ")";
    return rvG_OK;
  case GCLC_POINT:
    sValue = " Point : (" + d2s(it->second.p[0], 6) + ", " +
             d2s(it->second.p[1], 6) + ")";
    return rvG_OK;
  case GCLC_LINE:
    sValue = " Line : (" + d2s(it->second.p[0], 6) + ", " +
             d2s(it->second.p[1], 6) + ", " + d2s(it->second.p[2], 6) + ")";
    return rvG_OK;
  case GCLC_CIRCLE:
    sValue = " Circle : (" + d2s(it->second.p[0], 6) + ", " +
             d2s(it->second.p[1], 6) + ", " + d2s(it->second.p[2], 6) + ")";
    return rvG_OK;
  case GCLC_CONIC:
    sValue = " Conic : (" + d2s(it->second.p[0], 6) + ", " +
             d2s(it->second.p[1], 6) + ", " + d2s(it->second.p[2], 6) + ", " +
             d2s(it->second.p[3], 6) + ", " + d2s(it->second.p[4], 6) + ", " +
             d2s(it->second.p[5], 6) + ")";
    return rvG_OK;
  default:
    return rvG_EndOfData;
  }
}

// ----------------------------------------------------------------------------////

GReturnValue CGCLC::GetPointValue(const std::string &sVarName, double &x,
                                  double &y) {
  std::map<std::string, GCLC_object>::iterator it = m_pSymbolTable->find(sVarName);
  if (it != m_pSymbolTable->end() && it->second.type == GCLC_POINT) {
    x = it->second.p[0];
    y = it->second.p[1];
    return rvG_OK;
  }
  return rvG_InvalidInput;
}

// ----------------------------------------------------------------------------////

bool CGCLC::EndOfInputData() { return m_Input->EndOfInputData(); }

// ----------------------------------------------------------------------------////

GCLCError CGCLC::ReadToken(CGCLCInput &Input, std::string &sToken) {
  GReturnValue iRv;
  unsigned char c;

  sToken = "";
  do {
    if (Input.ReadChar(c) != rvG_OK)
      return rvGCLCEndOfInput;
  } while ((c == ' ') || (c == '\n') || (c == '\t') || (c == '\r'));

  if (c == '%') {
    sToken = "%";
    m_iLastReadLine = Input.GetCurrentLine();
    m_iLastReadPosition = Input.GetCurrentPosition();
    return rvGCLCOK;
  }

  iRv = rvG_OK;
  int anglebrackets = 0;
  for (;;) {
    if (EndOfInputData() || (iRv == rvG_EndOfData)) {
      if (!((c == ' ') || (c == '\n') || (c == '\t') ||
            (c == '\r'))) // fixed 17.09.2007.
        sToken += c;
      // changed in order to enable array elements - 24.07.2007
      // for instance  EvaluateIndex : A[6+5]->A[11]
      EvaluateIndex(sToken);
      if (!sToken.empty())
        return rvGCLCOK;
      else
        return rvGCLCEndOfInput;
    }
    if ((c == '\n') || (c == '\r') || (c == '\t')) {
      EvaluateIndex(sToken);
      if (!sToken.empty())
        return rvGCLCOK;
      else
        return rvGCLCEndOfInput;
    }
    if (c == ' ' && anglebrackets <= 0) {
      EvaluateIndex(sToken);
      if (!sToken.empty())
        return rvGCLCOK;
      else
        return rvGCLCEndOfInput;
    }
    if (c == '[')
      anglebrackets++;
    if (c == ']')
      anglebrackets--;

    sToken += c;

    iRv = Input.ReadChar(c);
    if (iRv == rvG_EndOfData) // 22.10.2007.
      c = ' ';
  }
}

// ----------------------------------------------------------------------------////

GCLCError CGCLC::ReadToken(std::string &sToken) {
  GReturnValue iRv;
  unsigned char c;
  sToken = "";

  m_iLineBeforeLastToken = m_Input->GetCurrentLine();
  m_iPositionBeforeLastToken = m_Input->GetCurrentPosition();

  do {
    if (m_Input->ReadChar(c) != rvG_OK)
      return rvGCLCEndOfInput;
  } while ((c == ' ') || (c == '\n') || (c == '\t') || (c == '\r'));

  if (c == '%') {
    sToken = "%";
    m_iLastReadLine = m_Input->GetCurrentLine();
    m_iLastReadPosition = m_Input->GetCurrentPosition();
    RecordPositionForWarning();
    return rvGCLCOK;
  }

  m_iLineBeforeLastToken = m_Input->GetCurrentLine();
  m_iPositionBeforeLastToken = m_Input->GetCurrentPosition();

  iRv = rvG_OK;
  int anglebrackets = 0;
  for (;;) {
    if (EndOfInputData() || (iRv == rvG_EndOfData)) {
      if (!((c == ' ') || (c == '\n') || (c == '\t') || (c == '\r')))
        sToken += c;

      // changed in order to enable array elements - 24.07.2007
      // for instance  EvaluateIndex : A[6+5]->A[11]
      EvaluateIndex(sToken);
      if (!sToken.empty()) {
        RecordPositionForWarning();
        return rvGCLCOK;
      } else
        return rvGCLCEndOfInput;
    }
    if ((c == '\n') || (c == '\r') || (c == '\t')) {
      EvaluateIndex(sToken);
      if (!sToken.empty()) {
        RecordPositionForWarning();
        return rvGCLCOK;
      } else
        return rvGCLCEndOfInput;
    }
    if (c == ' ' && anglebrackets <= 0) {
      EvaluateIndex(sToken);
      if (!sToken.empty()) {
        RecordPositionForWarning();
        return rvGCLCOK;
      } else
        return rvGCLCEndOfInput;
    }
    if (c == '[')
      anglebrackets++;
    if (c == ']')
      anglebrackets--;

    sToken += c;
    m_iLastReadLine = m_Input->GetCurrentLine();
    m_iLastReadPosition = m_Input->GetCurrentPosition();

    iRv = m_Input->ReadChar(c);
    if (iRv == rvG_EndOfData)
      c = ' ';
  }
}

// ----------------------------------------------------------------------------////

GCLCError CGCLC::ReadToken() { return ReadToken(m_sToken); }

// ----------------------------------------------------------------------------////

const std::string &CGCLC::LastToken() { return m_sToken; }

// ----------------------------------------------------------------------------////

GCLCError CGCLC::ReadNumber(double &dNumber) {
  GCLC_object o;
  GCLCError iRv;

  iRv = ReadToken();
  if (iRv != rvGCLCOK)
    return rvGCLCIdOrNumberExpected;

  if (!convert(LastToken(), dNumber)) {
    iRv = Value(LastToken(), o);
    if (iRv != rvGCLCOK)
      return rvGCLCNumberExpected;
    if (o.type != GCLC_NUMBER)
      return rvGCLCWrongType;
    dNumber = o.p[0];
  }
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------////

GCLCError CGCLC::ReadObject(int type, GCLC_object &o) {
  GCLCError iRv;
  if (ReadToken() != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = Value(LastToken(), o)) != rvGCLCOK)
    return iRv;
  if (o.type != type)
    return rvGCLCWrongType;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------////

GCLCError CGCLC::ReadNextObject(GCLC_object &o) {
  if (ReadToken() == rvGCLCOK)
    return Value(LastToken(), o);
  return rvGCLCIdExpected;
}

// ----------------------------------------------------------------------------////

void CGCLC::InitXML() {
  m_CurrentXMLgroup = eXMLnone;
  m_sXMLCurrentColor = "000000";
  m_sXMLNewColor = "000000";

  m_iXMLCurrentFontSize = 10;
  m_iXMLNewFontSize = 10;
}

// ----------------------------------------------------------------------------////

void CGCLC::ChangeCurrentXMLGroup(XML_group eXMLgroup) {
  if (m_CurrentXMLgroup == eXMLgroup) {
    if (m_CurrentXMLgroup == eXMLdraw) {
      if (m_sXMLCurrentColor == m_sXMLNewColor)
        return;
    } else if (m_CurrentXMLgroup == eXMLlabel) {
      if ((m_iXMLNewFontSize == m_iXMLCurrentFontSize) &&
          (m_sXMLCurrentColor == m_sXMLNewColor))
        return;
    } else
      return;
  }

  switch (m_CurrentXMLgroup) {
  case eXMLdefine:
    Print(m_hXMLOutput, "\t</define>\n\n");
    break;

  case eXMLconstruct:
    Print(m_hXMLOutput, "\t</construct>\n\n");
    break;

  case eXMLdraw:
    Print(m_hXMLOutput, "\t</draw>\n\n");
    break;

  case eXMLdrawdashed:
    Print(m_hXMLOutput, "\t</draw>\n\n");
    break;

  case eXMLlabel:
    Print(m_hXMLOutput, "\t</labels>\n\n");
    break;

  case eXMLconjecture:
    Print(m_hXMLOutput, "\t</conjecture>\n\n");
    break;

  default:
    break;
  }

  switch (eXMLgroup) {
  case eXMLdefine:
    Print(m_hXMLOutput, "\t<define>\n");
    break;

  case eXMLconstruct:
    Print(m_hXMLOutput, "\t<construct>\n");
    break;

  case eXMLdraw:
    if (m_sXMLNewColor == "000000")
      Print(m_hXMLOutput, "\t<draw>\n");
    else
      Print(m_hXMLOutput, "\t<draw color=\"" + m_sXMLNewColor + "\">\n");
    m_sXMLCurrentColor = m_sXMLNewColor;
    break;

  case eXMLdrawdashed:
    if (m_sXMLNewColor == "000000")
      Print(m_hXMLOutput, "\t<draw dashed=\"yes\">\n");
    else
      Print(m_hXMLOutput,
            "\t<draw dashed=\"yes\" color=\"" + m_sXMLNewColor + "\">\n");
    m_sXMLCurrentColor = m_sXMLNewColor;
    break;

  case eXMLlabel:
    if (m_sXMLCurrentColor == "000000") {
      if (m_iXMLCurrentFontSize == 10)
        Print(m_hXMLOutput, "\t<labels>\n");
      else
        Print(m_hXMLOutput,
              "\t<labels fontsize=\"" + i2s(m_iXMLCurrentFontSize) + "\">\n");
    } else {
      if (m_iXMLNewFontSize == 10)
        Print(m_hXMLOutput,
              "\t<labels color=\"" + m_sXMLCurrentColor + "\">\n");
      else
        Print(m_hXMLOutput, "\t<labels fontsize=\"" + i2s(m_iXMLNewFontSize) +
                                "\" color=\"" + m_sXMLCurrentColor + "\">\n");
    }
    m_sXMLCurrentColor = m_sXMLNewColor;
    m_iXMLCurrentFontSize = m_iXMLNewFontSize;
    break;

  case eXMLconjecture:
    Print(m_hXMLOutput, "\t<conjecture>\n");
    break;

  default:
    break;
  }
  m_CurrentXMLgroup = eXMLgroup;
}

// ----------------------------------------------------------------------------

void CGCLC::AddToLog(const std::string &pText) { m_Log.AddText(pText); }

// ----------------------------------------------------------------------------

void CGCLC::ResetLog() { m_Log.Reset(); }

// ----------------------------------------------------------------------------
