// GCLC.h: interface for the CGCLC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(gclc_H)
#define gclc_H

#include "../GenericEngine/GCompiler.h"
#include <fstream>
#include <map>
#include <memory>
#include <string>

/* ********************************************************
The class CGCLC provides a full support for handling
GCLC language. It inherits the class CGCompler and
implements its virtual methods.
This class does not export figures, instead it sends
processed data (reduced to a very few primitive forms)
via CGCompler methods. When exporting figures, these
data will be processed by the class CListOfPrimitiveCommands.
******************************************************** */

#define MAX_NUMBER_OF_LOOP_EXECUTIONS 10000

enum eOBJECT_TYPES {
  GCLC_NUMBER,
  GCLC_POINT,
  GCLC_LINE,
  GCLC_CIRCLE,
  GCLC_CONIC
};

enum eGCLC_DIRECTIONS {
  GCLC_DIR_LT = 1,
  GCLC_DIR_L,
  GCLC_DIR_LB,
  GCLC_DIR_T,
  GCLC_DIR_B,
  GCLC_DIR_RT,
  GCLC_DIR_R,
  GCLC_DIR_RB,
  GCLC_DIR_C
};

enum GCLCError {
  rvGCLCOK,
  rvGCLCEndOfInput,
  rvGCLCNoError,
  rvGCLCInvalidInput,
  rvGCLCIdExpected,
  rvGCLCIdOrNumberExpected,
  rvGCLCNumberExpected,
  rvGCLCTwoDecimalPoints,
  rvGCLCUndefinedVariable,
  rvGCLCWrongType,
  rvGCLCOutOfMemory,
  rvGCLCBadLine,
  rvGCLCBadCircle,
  rvGCLCBadEllipse,
  rvGCLCBadIntersection,
  rvGCLCBadAngle,
  rvGCLCLBracketExpected,
  rvGCLCRBracketExpected,
  rvGCLCFixedPointExpected,
  rvGCLCPointExpected,
  rvGCLCUnknownCommand,
  rvCannotExport,
  rvGCLCChangingValue,
  rvGCLCChangingType,
  rvGCLCInvalidExpression,
  rvGCLCInvalidConjecture,
  rvGCLCSeparatorExpected,
  rvGCLCInvalidWhileBlock,
  rvGCLCInvalidIfBlock,
  rvGCLCToManyWhileBlockExecutions,
  rvGCLCUnknownprocedure,
  rvGCLCParametersNotMatched,
  rvGCLCNoProceduresInBlocks,
  rvGCLCInvalidProcedure,
  rvGCLCInvalidIncludeFile,
  rvGCLCLIllFormedTree,
  rvGCLCLInvalidGraphRepresentation
};

enum GCLCcommands {
  // Basic definitions
  gclc_number,
  gclc_point,
  gclc_line,
  gclc_circle,
  gclc_set_equal,
  // Basic constructions
  gclc_intersec,
  gclc_intersec2,
  gclc_midpoint,
  gclc_med,
  gclc_bis,
  gclc_perp,
  gclc_foot,
  gclc_parallel,
  gclc_getcenter,
  gclc_onsegment,
  gclc_online,
  gclc_oncircle,
  // Transformations
  gclc_translate,
  gclc_rotate,
  gclc_rotateonellipse,
  gclc_sim,
  gclc_turtle,
  gclc_towards,
  // Calculations, expressions, arrays, and control structures
  gclc_getx,
  gclc_gety,
  gclc_distance,
  gclc_angle,
  gclc_angle_o,
  gclc_random,
  gclc_expression,
  gclc_array,
  gclc_while,
  gclc_if_then_else,
  gclc_procedure,
  gclc_procedure_call,
  gclc_include,
  // Drawing and Filling commands
  gclc_drawpoint,
  gclc_drawsegment,
  gclc_drawdashsegment,
  gclc_drawline,
  gclc_drawdashline,
  gclc_drawvector,
  gclc_drawarrow,
  gclc_drawcircle,
  gclc_drawdashcircle,
  gclc_drawarc,
  gclc_drawdasharc,
  gclc_drawarc_p,
  gclc_drawdasharc_p,
  gclc_drawellipse,
  gclc_drawdashellipse,
  gclc_drawellipsearc,
  gclc_drawdashellipsearc,
  gclc_drawellipsearc1,
  gclc_drawdashellipsearc1,
  gclc_drawellipsearc2,
  gclc_drawdashellipsearc2,
  gclc_drawbezier3,
  gclc_drawdashbezier3,
  gclc_drawbezier4,
  gclc_drawdashbezier4,
  gclc_drawpolygon,
  gclc_draw_tree,
  gclc_draw_graph_a,
  gclc_draw_graph_b,
  gclc_filltriangle,
  gclc_fillrectangle,
  gclc_fillcircle,
  gclc_fillellipse,
  gclc_fillarc,
  gclc_fillarc0,
  gclc_fillellipsearc,
  gclc_fillellipsearc0,
  // Labeling and Printing commands
  gclc_cmark,
  gclc_cmark_lt,
  gclc_cmark_l,
  gclc_cmark_lb,
  gclc_cmark_t,
  gclc_cmark_b,
  gclc_cmark_rt,
  gclc_cmark_r,
  gclc_cmark_rb,
  gclc_mark,
  gclc_mark_lt,
  gclc_mark_l,
  gclc_mark_lb,
  gclc_mark_t,
  gclc_mark_b,
  gclc_mark_rt,
  gclc_mark_r,
  gclc_mark_rb,
  gclc_printat,
  gclc_printat_lt,
  gclc_printat_l,
  gclc_printat_lb,
  gclc_printat_t,
  gclc_printat_b,
  gclc_printat_rt,
  gclc_printat_r,
  gclc_printat_rb,
  gclc_printvalueat,
  gclc_printvalueat_lt,
  gclc_printvalueat_l,
  gclc_printvalueat_lb,
  gclc_printvalueat_t,
  gclc_printvalueat_b,
  gclc_printvalueat_rt,
  gclc_printvalueat_r,
  gclc_printvalueat_rb,
  // Low level commands
  gclc_comment,
  gclc_dim,
  gclc_area,
  gclc_color,
  gclc_background,
  gclc_fontsize,
  gclc_arrow_style,
  gclc_circleprecision,
  gclc_bezierprecision,
  gclc_linethickness,
  gclc_double,
  gclc_normal,
  gclc_dash,
  gclc_dashstyle,
  gclc_dmc,
  gclc_mcr,
  gclc_mcp,
  gclc_export_to_generic_latex,
  gclc_export_to_simple_latex,
  gclc_export_to_pstricks,
  gclc_export_to_tikz,
  gclc_export_to_eps,
  gclc_export_to_svg,
  // 2D Cartesian commands
  gclc_ang_picture,
  gclc_ang_origin,
  gclc_ang_unit,
  gclc_ang_scale,
  gclc_ang_drawsystem_p,
  gclc_ang_drawsystem,
  gclc_ang_drawsystem0,
  gclc_ang_drawsystem1,
  gclc_ang_drawsystem_a,
  gclc_ang_drawsystem0_a,
  gclc_ang_drawsystem1_a,
  gclc_ang_point,
  gclc_ang_getx,
  gclc_ang_gety,
  gclc_ang_line,
  gclc_ang_conic,
  gclc_ang_intersec2,
  gclc_ang_tangent,
  gclc_ang_drawline,
  gclc_ang_drawline_p,
  gclc_ang_drawconic,
  gclc_ang_drawdashconic,
  gclc_ang_draw_parametric_curve,
  gclc_ang_conicprecision,
  gclc_ang_plotdata,
  // 3D Cartesian commands
  gclc_ang3d_picture,
  gclc_ang3d_origin,
  gclc_ang3d_unit,
  gclc_ang3d_scale,
  gclc_ang3d_axes_drawing_range,
  gclc_ang3d_drawsystem_p,
  gclc_ang3d_point,
  gclc_ang3d_getx,
  gclc_ang3d_gety,
  gclc_ang3d_getz,
  gclc_ang3d_drawline_p,
  gclc_ang3d_draw_parametric_surface,
  gclc_ang3d_draw_parametric_curve,
  // Layers
  gclc_getlayer,
  gclc_hidelayer,
  gclc_hidelayersfrom,
  gclc_hidelayersto,
  // Animations
  gclc_animation_frames,
  gclc_animation_trace_point,
  // Theorem proving
  gclc_conjecture,
  gclc_proof_level,
  gclc_proof_limit,
  gclc_prover_timeout,
  gclc_theorem_name,

  gclc_unknowncommand
};

enum XML_group {
  eXMLdefine,
  eXMLconstruct,
  eXMLdraw,
  eXMLdrawdashed,
  eXMLlabel,
  eXMLconjecture,
  eXMLnone
};

typedef struct procedure_tag {
  std::string sName;
  std::string sParameters;
  std::string sBlock;
} GCLCprocedure;

typedef struct {
  int CIRCLE_PRECISION;
  int MARK_CIRCLE_PRECISION;
  double MARK_RADIUS;
  double DM; /* distance point-mark */

  double ARROW_ANGLE;
  double ARROW_LENGTH;
  double ARROW_INNER_TOWARDS;
  int BEZIER_PRECISION;

  GCLC_area m_Basic_Area;
  int m_FontSize;

  GCLC_area m_2D_Area;
  double X_ORIGIN;
  double Y_ORIGIN;
  double UNIT;
  int CONIC_PRECISION;
  double ANG_SCALE;
  int ANG_LOG;

  GCLC_area m_3D_Area;
  int ANG3D_LOG;
  double X3D_ORIGIN;
  double Y3D_ORIGIN;
  double SIN_ALPHA3D;
  double SIN_BETA3D;
  double COS_ALPHA3D;
  double COS_BETA3D;
  double UNIT3D;
  double ANG3D_SCALE_Y;
  double ANG3D_SCALE_Z;

  double X3D_MIN;
  double X3D_MAX;
  double Y3D_MIN;
  double Y3D_MAX;
  double Z3D_MIN;
  double Z3D_MAX;
  bool ANG3D_AXES_LIMITED;

  int m_nAnimationFrames;
  int m_nAnimationSpeed;
} GCLC_env;

class CGCLC : public CGCompiler {
  friend class Parser;

public:
  CGCLC(CGCLCInput &input, CGCLCLog &Log, prover_config &ProverConfig,
        bool bXMLoutput, std::ofstream &hXML);
  CGCLC(CGCLCInput &input, CGCLCLog &Log,
        std::shared_ptr<CIntermediateRepresentation> pL,
        std::shared_ptr<std::map<std::string, GCLCprocedure>> procedures,
        std::map<std::string, GCLC_object> *pTable, bool bXMLOutput,
        std::ofstream &hXMLOutput);
  virtual ~CGCLC();
  void Init();
  void CleanUp();
  void SetEnv(GCLC_env &e);
  void GetEnv(GCLC_env &e) const;

  GReturnValue Import(eGCLC_conjecture_status &prover_status,
                      double &prover_time);
  GReturnValue Import();

  GReturnValue GetError(int &iErrorCode, std::string &sErrMessage, int &line,
                        int &pos);
  GReturnValue GetValue(const std::string &sVarName, std::string &sValue);

  bool GetAnimationFrames(int &iFrames, int &iSpeed);

  void InitXML();

  int view();

private:
  GCLCError Execute();

  GCLCError take_text(std::string &v);
  GCLCcommands choose_command(const std::string &word);

  GCLCError ReadToken(CGCLCInput &Input, std::string &sToken);
  GCLCError ReadToken(std::string &sToken);
  GCLCError ReadToken();
  const std::string &LastToken();
  void RecordPositionForWarning() {
    m_iWarnLine = m_iLineBeforeLastToken;
    m_iWarnPos = m_iPositionBeforeLastToken;
  }

  GCLCError ReadNumber(double &dNumber);
  GCLCError ReadObject(int type, GCLC_object &o);
  GCLCError ReadNextObject(GCLC_object &o);

  GCLCError Let(const std::string &sVarName, int type, double p1, double p2,
                double p3, double p4, double p5, double p6);
  GCLCError Value(const std::string &sVarName, GCLC_object &o);
  double distance2d(GCLC_object &o1, GCLC_object &o2);
  int warning(int w);

  bool EndOfInputData();
  CGCLCInput *m_Input;

  std::string m_sToken;
  int m_iLineBeforeLastToken;
  int m_iPositionBeforeLastToken;
  int m_iLastReadLine;
  int m_iLastReadPosition;
  int m_iWarnLine;
  int m_iWarnPos;
  enum GCLCError m_iErrorCode;
  bool m_bWhileBlockOrProcedureCall;
  bool m_bProcedureCall;
  bool m_bSupressWarnings;

  CGCLCLog &m_Log;
  void ResetLog();
  void AddToLog(const std::string &pText);

  std::map<std::string, GCLC_object> *m_pSymbolTable;
  std::shared_ptr<std::map<std::string, GCLCprocedure>> m_pProcedures;

  bool m_bXMLOutput;
  std::ofstream &m_hXMLOutput;
  XML_group m_CurrentXMLgroup;
  std::string m_sXMLCurrentColor;
  std::string m_sXMLNewColor;
  int m_iXMLCurrentFontSize, m_iXMLNewFontSize;
  void ChangeCurrentXMLGroup(XML_group eXMLgroup);

  // ----------------------------------------------------------

  // Basic definitions
  GCLCError get_number();
  GCLCError get_point(int &iArgs);
  GCLCError get_line();
  GCLCError get_circle();
  GCLCError set_equal();

  // ----------------------------------------------------------

  // Basic constructions
  GCLCError get_intersec();
  GCLCError get_intersec2();
  GCLCError get_midpoint();
  GCLCError get_med();
  GCLCError get_bis();
  GCLCError get_perp();
  GCLCError get_foot();
  GCLCError get_parallel();
  GCLCError get_getcenter();
  GCLCError get_onsegment();
  GCLCError get_online();
  GCLCError get_oncircle();

  // ----------------------------------------------------------

  // Transformations
  GCLCError get_translate();
  GCLCError get_rotate();
  GCLCError get_rotateonellipse();
  GCLCError get_sim();
  GCLCError get_turtle();
  GCLCError get_towards();

  // ----------------------------------------------------------

  // Calculations, expressions, arrays, and control structures
  GCLCError get_x();
  GCLCError get_y();
  GCLCError get_distance();
  GCLCError get_angle(bool orientation);
  GCLCError get_random();
  GCLCError get_expression();
  GCLCError get_array();
  GCLCError EvaluateIndex(std::string &sObjectName);
  GCLCError generate_array_elements(int Dim[], int current_index, int max_index,
                                    const std::string &sName);
  GCLCError get_while();
  GCLCError get_if_then_else();
  GCLCError get_procedure();
  GCLCError get_call_procedure();
  GCLCError AddNewProcedure(std::string name, std::string params, std::string block);
  GCLCError get_include();

  GCLCError calc_expression(const std::string &text, double &dNumber);

  // ----------------------------------------------------------

  // Drawing and Filling commands
  GCLCError draw_point();
  GCLCError draw_segment(bool dashed);
  GCLCError draw_line(bool dashed);
  GCLCError draw_vector();
  GCLCError draw_arrow();
  GCLCError draw_circle(bool dashed);
  GCLCError draw_arc(bool dashed, bool positive);
  GCLCError draw_ellipse(bool dashed);
  GCLCError draw_ellipsearc(bool dashed, bool bFourGivenPoints, bool bByAngles);
  GCLCError draw_bezier3(bool dashed);
  GCLCError draw_bezier4(bool dashed);
  GCLCError draw_polygon();
  GCLCError draw_tree();
  int check_tree(const std::string &sTree, int iLevel, int iMaxNodesAtLevel[],
                 int &iMaxHeight);
  int draw_all_nodes(double dFigureWidth, double dLevelHeight,
                     const std::string &sTree, int iMaxNodesAtLevel[], int iLevel,
                     int iNumberOfSiblings, int iCurrentNode, double dParentX,
                     double dParentY, bool bParentNoName, double dRootX,
                     double dRootY, int iStyle, double dRotationAngle,
                     const std::string &sRefPointName);
  GCLCError draw_graph_a();
  GCLCError draw_graph_b();
  GCLCError fill_triangle();
  GCLCError fill_rectangle();
  GCLCError fill_circle();
  GCLCError fill_ellipse();
  GCLCError fill_arc(bool bFillTriangle);
  GCLCError fill_ellipsearc(bool bFillTriangle);

  // ----------------------------------------------------------

  // Labeling and Printing commands
  GCLCError cmark(int dir);
  GCLCError mark(int dir);
  GCLCError printat(int dir);
  GCLCError printvalueat(int dir);

  // ----------------------------------------------------------

  // Low level commands
  GCLCError get_comment();
  GCLCError dim();
  GCLCError get_area();
  GCLCError get_color();
  GCLCError get_background();
  GCLCError get_fontsize();
  GCLCError get_arrow_style();
  GCLCError get_circleprecision();
  GCLCError get_bezierprecision();
  GCLCError get_linethickness();
  GCLCError set_double();
  GCLCError set_normal();
  GCLCError get_dash();
  GCLCError get_dashstyle();
  GCLCError get_dmc();
  GCLCError get_mcr();
  GCLCError get_mcp();
  GCLCError get_export(export_type format);

  int CIRCLE_PRECISION;
  int MARK_CIRCLE_PRECISION;
  double MARK_RADIUS;
  double DM; /* distance point-mark */

  double ARROW_ANGLE;
  double ARROW_LENGTH;
  double ARROW_INNER_TOWARDS;
  int BEZIER_PRECISION;

  GCLC_area m_Basic_Area;
  int m_FontSize;

  // ----------------------------------------------------------

  // 2D Cartesian commands
  GCLCError ang_picture();
  GCLCError ang_origin();
  GCLCError ang_unit();
  GCLCError ang_scale();
  GCLCError ang_draw_system_p();
  GCLCError ang_draw_system(int t);
  GCLCError ang_draw_system(int mark_integers, int mark_axes, int arrows,
                            int x_step, int y_step);
  GCLCError ang_point();
  GCLCError ang_getx();
  GCLCError ang_gety();
  GCLCError ang_line();
  GCLCError ang_conic();
  GCLCError ang_intersec2();
  GCLCError ang_tangent();
  GCLCError ang_draw_line();
  GCLCError ang_draw_line_p();
  GCLCError ang_draw_conic(int dashed);
  GCLCError ang_draw_parametric_curve();
  GCLCError get_conicprecision();
  GCLCError ang_plot_data();

  GCLCError ang_mark(double x0, double y0, int dir, const std::string &name);
  GCLCError ang_cmark(double x0, double y0, int dir, const std::string &name);
  GCLCError ang_arrow(double x1, double y1, double x2, double y2);
  double ang_y(double y);
  double ang_y_inv(double y);
  GCLCError ang_fline(double x1, double y1, double x2, double y2);
  GCLCError ang_fsegment(double x1, double y1, double x2, double y2);

  GCLCError ang_2D_3D_mark(double x0, double y0, int dir, const std::string &name,
                           GCLC_area &area);
  GCLCError ang_2D_3D_cmark(double x0, double y0, int dir, const std::string &name,
                            GCLC_area &area);
  GCLCError ang_2D_3D_arrow(double x1, double y1, double x2, double y2,
                            GCLC_area &area);

  GCLC_area m_2D_Area;
  double X_ORIGIN;
  double Y_ORIGIN;
  double UNIT;
  int CONIC_PRECISION;
  double ANG_SCALE;
  int ANG_LOG;

  // ----------------------------------------------------------

  // 3D Cartesian commands
  GCLCError ang3d_picture();
  GCLCError ang3d_origin();
  GCLCError ang3d_unit();
  GCLCError ang3d_scale();
  GCLCError ang3d_axes_drawing_range();
  GCLCError ang3d_draw_system_p();
  GCLCError ang3d_draw_system(int mark_integers, int x_step, int y_step,
                              int z_step, int mark_axes, int arrows);
  GCLCError ang3d_point();
  GCLCError ang3d_getx();
  GCLCError ang3d_gety();
  GCLCError ang3d_getz();
  GCLCError ang3d_draw_line_p();
  GCLCError ang3d_draw_parametric_surface();
  GCLCError ang3d_draw_parametric_curve();

  GCLCError ang3d_mark(double x0, double y0, int dir, const std::string &name);
  GCLCError ang3d_cmark(double x0, double y0, int dir, const std::string &name);
  GCLCError ang3d_arrow(double x1, double y1, double x2, double y2);
  GCLCError ang3d_fsegment(double x1, double y1, double x2, double y2);
  GCLCError ang3d_fline(double x1, double y1, double x2, double y2);
  double ang3d_x(double x, double y, double z);
  double ang3d_y(double x, double y, double z);
  bool ang3d_clip_x_axis(double &x1, double &y1, double &x2, double &y2);
  bool ang3d_clip_y_axis(double &x1, double &y1, double &x2, double &y2);
  bool ang3d_clip_z_axis(double &x1, double &y1, double &x2, double &y2);

  GCLC_area m_3D_Area;
  int ANG3D_LOG;
  double X3D_ORIGIN;
  double Y3D_ORIGIN;
  double SIN_ALPHA3D;
  double SIN_BETA3D;
  double COS_ALPHA3D;
  double COS_BETA3D;
  double UNIT3D;
  double ANG3D_SCALE_Y;
  double ANG3D_SCALE_Z;

  double X3D_MIN;
  double X3D_MAX;
  double Y3D_MIN;
  double Y3D_MAX;
  double Z3D_MIN;
  double Z3D_MAX;
  bool ANG3D_AXES_LIMITED;

  // ----------------------------------------------------------

  // Layers
  GCLCError get_layer();
  GCLCError hide_layer();
  GCLCError hide_layersfrom();
  GCLCError hide_layersto();

  // ----------------------------------------------------------

  // Animations
  GCLCError get_animation_frames();
  GCLCError get_animation_trace_point();

  int m_nAnimationFrames;
  int m_nAnimationSpeed;

  // ----------------------------------------------------------

  // Theorem proving
  GCLCError get_conjecture();
  GCLCError get_proof_level();
  GCLCError get_proof_limit();
  GCLCError get_prover_timeout();
  GCLCError get_prover_theorem_name();

  prover_config m_ProverConfig;

  // ----------------------------------------------------------
};

#endif // !defined(gclc_H)
