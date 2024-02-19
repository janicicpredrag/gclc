#include "Highlighter.h"
#include <QRegularExpression>


// --------------------------------------------------------------------------------------------


Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    rule.keywords << "\\bpoint\\b";
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(QColor(0, 128, 255));
    highlightingRules.append(rule);
    rule.keywords.clear();

    rule.keywords
        << "\\bline\\b"
        << "\\bcircle\\b"
        << "\\bnumber\\b"
        << "\\bangle\\b"
        << "\\bangle_o\\b"
        << "\\bset_equal\\b"
        << "\\bexpression\\b"
        << "\\barray\\b"
        << "\\bwhile\\b"
        << "\\bif_then_else\\b"
        << "\\brandom\\b"
        << "\\bintersec\\b"
        << "\\bintersection\\b"
        << "\\bintersec2\\b"
        << "\\bintersection2\\b"
        << "\\bdrawpoint\\b"
        << "\\bdrawsegment\\b"
        << "\\bdrawdashsegment\\b"
        << "\\bdrawline\\b"
        << "\\bdrawdashline\\b"
        << "\\bdrawvector\\b"
        << "\\bdrawcircle\\b"
        << "\\bdrawdashcircle\\b"
        << "\\bdrawarc\\b"
        << "\\bdrawdasharc\\b"
        << "\\bdrawarc_p\\b"
        << "\\bdrawdasharc_p\\b"
        << "\\bdrawellipse\\b"
        << "\\bdrawdashellipse\\b"
        << "\\bdrawellipsearc\\b"
        << "\\bdrawdashellipsearc\\b"
        << "\\bdrawellipsearc1\\b"
        << "\\bdrawdashellipsearc1\\b"
        << "\\bdrawellipsearc2\\b"
        << "\\bdrawdashellipsearc2\\b"
        << "\\bdrawpolygon\\b"
        << "\\bdrawarrow\\b"
        << "\\bdrawbezier3\\b"
        << "\\bdrawbezier4\\b"
        << "\\bdrawdashbezier3\\b"
        << "\\bdrawdashbezier4\\b"
        << "\\blayer\\b"
        << "\\bhide_layer\\b"
        << "\\bhide_layers_from\\b"
        << "\\bhide_layers_to\\b"
        << "\\bfilltriangle\\b"
        << "\\bfillrectangle\\b"
        << "\\bfillcircle\\b"
        << "\\bfillellipse\\b"
        << "\\bfillarc\\b"
        << "\\bfillarc0\\b"
        << "\\bfillellipsearc\\b"
        << "\\bfillellipsearc0\\b"
        << "\\bmidpoint\\b"
        << "\\bperp\\b"
        << "\\bperpendicular\\b"
        << "\\bparallel\\b"
        << "\\bgetcenter\\b"
        << "\\bonline\\b"
        << "\\bonsegment\\b"
        << "\\boncircle\\b"
        << "\\bbis\\b"
        << "\\bbisector\\b"
        << "\\bmed\\b"
        << "\\bmediatrice\\b"
        << "\\bdistance\\b"
        << "\\bsim\\b"
        << "\\bsymmetrical\\b"
        << "\\btranslate\\b"
        << "\\brotate\\b"
        << "\\brotateonellipse\\b"
        << "\\bturtle\\b"
        << "\\btowards\\b"
        << "\\bcmark\\b"
        << "\\bcmark_lt\\b"
        << "\\bcmark_l\\b"
        << "\\bcmark_lb\\b"
        << "\\bcmark_t\\b"
        << "\\bcmark_b\\b"
        << "\\bcmark_rt\\b"
        << "\\bcmark_r\\b"
        << "\\bcmark_rb\\b"
        << "\\bmark\\b"
        << "\\bmark_lt\\b"
        << "\\bmark_l\\b"
        << "\\bmark_lb\\b"
        << "\\bmark_t\\b"
        << "\\bmark_b\\b"
        << "\\bmark_rt\\b"
        << "\\bmark_r\\b"
        << "\\bmark_rb\\b"
        << "\\bprintat\\b"
        << "\\bprintat_lt\\b"
        << "\\bprintat_l\\b"
        << "\\bprintat_lb\\b"
        << "\\bprintat_t\\b"
        << "\\bprintat_b\\b"
        << "\\bprintat_rt\\b"
        << "\\bprintat_r\\b"
        << "\\bprintat_rb\\b"
        << "\\bprintvalueat\\b"
        << "\\bprintvalueat_lt\\b"
        << "\\bprintvalueat_l\\b"
        << "\\bprintvalueat_lb\\b"
        << "\\bprintvalueat_t\\b"
        << "\\bprintvalueat_b\\b"
        << "\\bprintvalueat_rt\\b"
        << "\\bprintvalueat_r\\b"
        << "\\bprintvalueat_rb\\b"
        << "\\bgetx\\b"
        << "\\bgety\\b"
        << "\\bfoot\\b"
        << "\\bprocedure\\b"
        << "\\bcall\\b"
        << "\\binclude\\b"
        << "\\bdrawtree\\b"
        << "\\bdrawgraph_a\\b"
        << "\\bdrawgraph_b\\b";
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(QColor(0, 0, 255));
    highlightingRules.append(rule);
    rule.keywords.clear();

    rule.keywords
        << "\\bang_picture\\b"
        << "\\bang_origin\\b"
        << "\\bang_unit\\b"
        << "\\bang_scale\\b"
        << "\\bang_point\\b"
        << "\\bang_line\\b"
        << "\\bang_conic\\b"
        << "\\bang_conicprecision\\b"
        << "\\bang_intersec2\\b"
        << "\\bang_intersection2\\b"
        << "\\bang_drawline\\b"
        << "\\bang_drawline_p\\b"
        << "\\bang_drawsystem\\b"
        << "\\bang_drawsystem_p\\b"
        << "\\bang_drawsystem0\\b"
        << "\\bang_drawsystem1\\b"
        << "\\bang_drawsystem_a\\b"
        << "\\bang_drawsystem0_a\\b"
        << "\\bang_drawsystem1_a\\b"
        << "\\bang_drawconic\\b"
        << "\\bang_drawdashconic\\b"
        << "\\bang_draw_parametric_curve\\b"
        << "\\bang_tangent\\b" << "\\bang_plot_data\\b"
        << "\\bang_getx\\b" << "\\bang_gety\\b";
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(QColor(128, 0, 255));
    highlightingRules.append(rule);
    rule.keywords.clear();

    rule.keywords
        << "\\bang3d_picture\\b"
        << "\\bang3d_origin\\b"
        << "\\bang3d_unit\\b"
        << "\\bang3d_scale\\b"
        << "\\bang3d_point\\b"
        << "\\bang3d_drawline_p\\b"
        << "\\bang3d_axes_drawing_range\\b"
        << "\\bang3d_drawsystem_p\\b"
        << "\\bang3d_draw_parametric_surface\\b"
        << "\\bang3d_draw_parametric_curve\\b"
        << "\\bang3d_getx\\b"
        << "\\bang3d_gety\\b"
        << "\\bang3d_getz\\b";
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(QColor(64, 0, 255));
    highlightingRules.append(rule);
    rule.keywords.clear();

    rule.keywords
        << "\\bdim\\b"
        << "\\bcolor\\b"
        << "\\bbackground\\b"
        << "\\barea\\b"
        << "\\bcircleprecision\\b"
        << "\\bbezierprecision\\b"
        << "\\blinethickness\\b"
        << "\\barrowstyle\\b"
        << "\\bdash\\b"
        << "\\bdashstyle\\b"
        << "\\bdmc\\b"
        << "\\bmcr\\b"
        << "\\bmcp\\b"
        << "\\bnormal\\b"
        << "\\bdouble\\b"
        << "\\bfontsize\\b"
        << "\\bexport_to_latex\\b"
        << "\\bexport_to_simple_latex\\b"
        << "\\bexport_to_pstricks\\b"
        << "\\bexport_to_tikz\\b"
        << "\\bexport_to_eps\\b"
        << "\\bexport_to_svg\\b";
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(QColor(128, 128, 255));
    highlightingRules.append(rule);
    rule.keywords.clear();

    rule.keywords
        << "\\banimation_frames\\b"
        << "\\btrace\\b";
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(QColor(192, 0, 192));
    highlightingRules.append(rule);
    rule.keywords.clear();

    rule.keywords
        << "\\bprove\\b"
        << "\\bprooflevel\\b"
        << "\\bprooflimit\\b"
        << "\\bprover_timeout\\b"
        << "\\btheorem_name\\b";
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(QColor(0, 192, 128));
    highlightingRules.append(rule);
    rule.keywords.clear();

    rule.keywords << "%[^\n]*";
    rule.format.setFontWeight(QFont::Light);
    rule.format.setForeground(Qt::darkGray);
    highlightingRules.append(rule);
    rule.keywords.clear();
}


// --------------------------------------------------------------------------------------------


void Highlighter::highlightBlock(const QString &text)
{
    HighlightingRule rule;
    QString keyword;
    foreach (rule, highlightingRules) {
       foreach (keyword, rule.keywords) {
          QRegularExpression expression(keyword);
          // int index=text.indexOf(expression);
          int index=0;
          QRegularExpressionMatchIterator i = expression.globalMatch(text);
          // while (index >= 0) {
          while (i.hasNext()) {
             QRegularExpressionMatch match = i.next();
             int length = match.capturedLength();
             // int length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index += length;
             index = text.indexOf(expression,index);
          }
       }
    }
}
