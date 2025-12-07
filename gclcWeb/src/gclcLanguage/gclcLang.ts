import { parser } from "./lang.js";
import {
  foldNodeProp,
  LRLanguage,
  LanguageSupport,
  HighlightStyle,
} from "@codemirror/language";
import { styleTags, Tag } from "@lezer/highlight";
import { completeFromList, ifNotIn } from "@codemirror/autocomplete";

const tagA: Tag = Tag.define();
const tagB: Tag = Tag.define();
const tagC: Tag = Tag.define();
const tagD: Tag = Tag.define();
const tagE: Tag = Tag.define();
const tagF: Tag = Tag.define();
const tagG: Tag = Tag.define();
const tagIdentifier: Tag = Tag.define();
const tagComment: Tag = Tag.define();
const tagNumber: Tag = Tag.define();

const parserWithMetadata = parser.configure({
  props: [
    styleTags({
      A: tagA,
      B: tagB,
      C: tagC,
      D: tagD,
      E: tagE,
      F: tagF,
      G: tagG,
      Identifier: tagIdentifier,
      LineComment: tagComment,
      Number: tagNumber,
    }),
    foldNodeProp.add({
      StatementBlock(tree) {
        return { from: tree.from + 1, to: tree.to - 1 };
      },
    }),
  ],
});

const highlightStyle = HighlightStyle.define([
  { tag: tagA, class: "tagA" },
  { tag: tagB, class: "tagB" },
  { tag: tagC, class: "tagC" },
  { tag: tagD, class: "tagD" },
  { tag: tagE, class: "tagE" },
  { tag: tagF, class: "tagF" },
  { tag: tagG, class: "tagG" },
  { tag: tagIdentifier, class: "tagIdentifier" },
  { tag: tagComment, class: "tagComment" },
  { tag: tagNumber, class: "tagNumber" },
]);

const gcLang = LRLanguage.define({
  parser: parserWithMetadata,
  languageData: {},
});

const gclCompletion = gcLang.data.of({
  autocomplete: ifNotIn(
    ["LineComment"],
    completeFromList([
      {
        label: "number",
        type: "type",
        info: "Define a number",
      },
      {
        label: "point",
        type: "type",
        info: "Define a point by its coordinates",
        detail: "Name x y",
      },
      {
        label: "line",
        type: "type",
        info: "Define a line by two points",
        detail: "Name Point₁ Point₂",
      },
      {
        label: "circle",
        type: "type",
        info: "Define a circle by center and arbitrary point on a circle",
        detail: "Name Point₁ Point₂",
      },
      {
        label: "set_equal",
        type: "function",
        info: "Set type type and the value of first object to the type and the value of the second object",
        detail: "Object₁ Object₂",
      },
      {
        label: "expression",
        type: "keyword",
        info: "Define a numeric value by calculating given expression",
        detail: "Name Expression",
      },
      { label: "while", type: "keyword" },
      {
        label: "if_then_else",
        type: "keyword",
        info: "Conditionaly execute blocks of commands",
        detail: "Expression ThenBlock ElseBlock",
      },
      {
        label: "procedure",
        type: "keyword",
        info: "Define a procedure",
        detail: "Name ListOfArguments BlockOfCommands",
      },
      {
        label: "call",
        type: "keyword",
      },
      {
        label: "include",
        type: "keyword",
        info: "Include the content of another .gcl file",
      },
      {
        label: "intersec",
        type: "function",
        info: "Define point by intersection of two lines",
        detail: "Name Line₁ Line₂",
      },
      {
        label: "intersec2",
        type: "function",
        info: "Define points by intersection of two circles",
        detail: "Name₁ Name₂ Circle₁ Circle₂",
      },
      {
        label: "midpoint",
        type: "function",
        info: "Define point as midpoint of the segment determined by given points",
        detail: "Name Point₁ Point₂",
      },
      {
        label: "med",
        type: "function",
        info: "Define a line that bisects and is perpendicular to the segment determined by the two given points",
        detail: "Name Point₁ Point₂",
      },
      {
        label: "bis",
        type: "function",
        info: "Define a line that bisects and is perpendicular to the segment determined by the two given points",
        detail: "Name Point₁ Point₂",
      },
      {
        label: "perp",
        type: "function",
        info: "Define a line that is perpendicular to the given line and contains given point",
        detail: "Name Point Line",
      },
      {
        label: "foot",
        type: "function",
        info: "Define a point as intersection of given line and line perpendicular to it that contains given point",
        detail: "Name Point Line",
      },
      {
        label: "parallel",
        type: "function",
        info: "Define a line that is parallel to the given line and contains the given point",
        detail: "Name Point Line",
      },
      {
        label: "getcenter",
        type: "function",
        info: "Define center of given circle",
        detail: "Name Circle",
      },
      {
        label: "onsegment",
        type: "function",
        info: "Define random point on segment defined by two given points",
        detail: "Name Point₁ Point₂",
      },
      {
        label: "online",
        type: "function",
        info: "Define random point on line defined by two given points",
        detail: "Name Point₁ Point₂",
      },
      {
        label: "oncircle",
        type: "function",
        info: "Define random point on circle which contains the second given point and has the first given point as center",
        detail: "Name Point₁ Point₂",
      },
      {
        label: "translate",
        type: "function",
        detail: "Name Point₁ Point₂ Point₃",
      },
      {
        label: "rotate",
        type: "function",
        detail: "Name Point₁ angle Point₂",
      },
      {
        label: "rotateonellipse",
        type: "function",
        detail: "Name Point₁ Point₂ Point₃ angle Point₄",
      },
      {
        label: "sim",
        type: "function",
        detail: "Name Object Point",
      },
      {
        label: "turtle",
        type: "function",
        detail: "Name Point₁ Point₂ angle length",
      },
      {
        label: "towards",
        type: "function",
        detail: "Name Point₁ Point₂ coefficient",
      },
      { label: "getx", type: "function", detail: "Name Point" },
      { label: "getx", type: "function", detail: "Name Point" },
      { label: "distance", type: "function", detail: "Name P₁ P₂" },
      { label: "angle", type: "function", detail: "Name P₁ P₂ Point₃" },
      { label: "angle_o", type: "function", detail: "Name P₁ P₂ Point₃" },
      { label: "random", type: "function", detail: "Name" },
      { label: "drawpoint", type: "function", detail: "Point" },
      { label: "drawsegment", type: "function", detail: "Point₁ Point₂" },
      { label: "drawdashsegment", type: "function", detail: "Point₁ Point₂" },
      { label: "drawline", type: "function", detail: "Line" },
      { label: "drawline", type: "function", detail: "Point₁ Point₂" },
      { label: "drawdashline", type: "function", detail: "Line" },
      { label: "drawdashline", type: "function", detail: "Point₁ Point₂" },
      { label: "drawvector", type: "function", detail: "Point₁ Point₂" },
      { label: "drawarrow", type: "function", detail: "Point₁ Point₂ n" },
      { label: "drawcircle", type: "function", detail: "Circle" },
      { label: "drawcircle", type: "function", detail: "Point₁ Point₂" },
      { label: "drawdashcircle", type: "function", detail: "Point₁ Point₂" },
      { label: "drawarc", type: "function", detail: "Point₁ Point₂ angle" },
      { label: "drawarc_p", type: "function", detail: "Point₁ Point₂ angle" },
      { label: "drawdasharc", type: "function", detail: "Point₁ Point₂ angle" },
      {
        label: "drawdasharc_p",
        type: "function",
        detail: "Point₁ Point₂ angle",
      },
      {
        label: "drawellipse",
        type: "function",
        detail: "Point₁ Point₂ Point₃",
      },
      {
        label: "drawdashellipse",
        type: "function",
        detail: "Point₁ Point₂ Point₃",
      },
      {
        label: "drawellipsearc",
        type: "function",
        detail: "Point₁ Point₂ Point₃ angle",
      },
      {
        label: "drawdashellipsearc",
        type: "function",
        detail: "Point₁ Point₂ Point₃ angle",
      },
      {
        label: "drawellipsearc1",
        type: "function",
        detail: "Point₁ Point₂ Point₃ Point₄ angle",
      },
      {
        label: "drawdashellipsearc1",
        type: "function",
        detail: "Point₁ Point₂ Point₃ Point₄ angle",
      },
      {
        label: "drawellipsearc2",
        type: "function",
        detail: "Point₁ Point₂ Point₃ angle₁ angle₂",
      },
      {
        label: "drawdashellipsearc2",
        type: "function",
        detail: "Point₁ Point₂ Point₃ angle₁ angle₂",
      },
      {
        label: "drawbezier3",
        type: "function",
        detail: "Point₁ Point₂ Point₃",
      },
      {
        label: "drawdashbezier3",
        type: "function",
        detail: "Point₁ Point₂ Point₃",
      },
      {
        label: "drawbezier4",
        type: "function",
        detail: "Point₁ Point₂ Point₃ Point₄",
      },
      {
        label: "drawdashbezier4",
        type: "function",
        detail: "Point₁ Point₂ Point₃ Point₄",
      },
      {
        label: "drawpolygon",
        type: "function",
        detail: "Point₁ Point₂ Point₃ Point₄",
      },
      {
        label: "drawtree",
        type: "function",
        detail: "Point n₁ n₂ n₃ n₄",
      },
      {
        label: "drawgraph_a",
        type: "function",
        detail: "Point n₁ n₂ ListOfNodes ListOfEdges",
      },
      {
        label: "drawgraph_b Label ListOfNodes ListOfEdges",
        type: "function",
        detail: "Point₁ Point₂ Point₃ Point₄",
      },
      {
        label: "filltriangle",
        type: "function",
        detail: "Point₁ Point₂ Point₃",
      },
      {
        label: "fillrectangle",
        type: "function",
        detail: "Point₁ Point₂",
      },
      { label: "fillcircle", type: "function", detail: "Point₁ Point₂" },
      { label: "fillcircle", type: "function", detail: "Circle" },
      { label: "fillellipse", type: "function", detail: "Point r₁ r₂" },
      { label: "fillarc", type: "function", detail: "Point₁ Point₂ angle" },
      { label: "fillarc0", type: "function", detail: "Point₁ Point₂ angle" },
      { label: "fillellipsearc", type: "function", detail: "Point r₁ r₂ startAngle angle" },
      { label: "fillellipsearc0", type: "function", detail: "Point r₁ r₂ startAngle angle" },
      {
        label: "cmark",
        type: "function",
        info: "Denote the given point by name and small empty circle",
        detail: "Point",
      },
      {
        label: "cmark_lt",
        type: "function",
        info: "Denote the given point by name and small empty circle",
        detail: "Point",
      },
      {
        label: "cmark_l",
        type: "function",
        info: "Denote the given point by name and small empty circle",
        detail: "Point",
      },
      {
        label: "cmark_lb",
        type: "function",
        info: "Denote the given point by name and small empty circle",
        detail: "Point",
      },
      {
        label: "cmark_t",
        type: "function",
        info: "Denote the given point by name and small empty circle",
        detail: "Point",
      },
      {
        label: "cmark_rt",
        type: "function",
        info: "Denote the given point by name and small empty circle",
        detail: "Point",
      },
      {
        label: "cmark_r",
        type: "function",
        info: "Denote the given point by name and small empty circle",
        detail: "Point",
      },
      {
        label: "cmark_rb",
        type: "function",
        info: "Denote the given point by name and small empty circle",
        detail: "Point",
      },
      {
        label: "mark",
        type: "function",
        info: "Denote the given point by name",
        detail: "Point",
      },
      {
        label: "mark_lt",
        type: "function",
        info: "Denote the given point by name",
        detail: "Point",
      },
      {
        label: "mark_l",
        type: "function",
        info: "Denote the given point by name",
        detail: "Point",
      },
      {
        label: "mark_lb",
        type: "function",
        info: "Denote the given point by name",
        detail: "Point",
      },
      {
        label: "mark_t",
        type: "function",
        info: "Denote the given point by name",
        detail: "Point",
      },
      {
        label: "mark_b",
        type: "function",
        info: "Denote the given point by name",
        detail: "Point",
      },
      {
        label: "mark_rt",
        type: "function",
        info: "Denote the given point by name",
        detail: "Point",
      },
      {
        label: "mark_r",
        type: "function",
        info: "Denote the given point by name",
        detail: "Point",
      },
      {
        label: "mark_rb",
        type: "function",
        info: "Denote the given point by name",
        detail: "Point",
      },
      {
        label: "printat",
        type: "function",
        info: "Print the given text at coordinates of the given point",
        detail: "Point Text",
      },
      {
        label: "printat_lt",
        type: "function",
        info: "Print the given text at coordinates of the given point",
        detail: "Point Text",
      },
      {
        label: "printat_l",
        type: "function",
        info: "Print the given text at coordinates of the given point",
        detail: "Point Text",
      },
      {
        label: "printat_lb",
        type: "function",
        info: "Print the given text at coordinates of the given point",
        detail: "Point Text",
      },
      {
        label: "printat_t",
        type: "function",
        info: "Print the given text at coordinates of the given point",
        detail: "Point Text",
      },
      {
        label: "printat_b",
        type: "function",
        info: "Print the given text at coordinates of the given point",
        detail: "Point Text",
      },
      {
        label: "printat_rt",
        type: "function",
        info: "Print the given text at coordinates of the given point",
        detail: "Point Text",
      },
      {
        label: "printat_r",
        type: "function",
        info: "Print the given text at coordinates of the given point",
        detail: "Point Text",
      },
      {
        label: "printat_rb",
        type: "function",
        info: "Print the given text at coordinates of the given point",
        detail: "Point Text",
      },
      {
        label: "printvalueat",
        type: "function",
        info: "Print value of the given object at coordinates of the given point",
        detail: "Point Object",
      },
      {
        label: "printvalueat_lt",
        type: "function",
        info: "Print value of the given object at coordinates of the given point",
        detail: "Point Object",
      },
      {
        label: "printvalueat_l",
        type: "function",
        info: "Print value of the given object at coordinates of the given point",
        detail: "Point Object",
      },
      {
        label: "printvalueat_lb",
        type: "function",
        info: "Print value of the given object at coordinates of the given point",
        detail: "Point Object",
      },
      {
        label: "printvalueat_t",
        type: "function",
        info: "Print value of the given object at coordinates of the given point",
        detail: "Point Object",
      },
      {
        label: "printvalueat_b",
        type: "function",
        info: "Print value of the given object at coordinates of the given point",
        detail: "Point Object",
      },
      {
        label: "printvalueat_rt",
        type: "function",
        info: "Print value of the given object at coordinates of the given point",
        detail: "Point Object",
      },
      {
        label: "printvalueat_r",
        type: "function",
        info: "Print value of the given object at coordinates of the given point",
        detail: "Point Object",
      },
      {
        label: "printvalueat_rb",
        type: "function",
        info: "Print value of the given object at coordinates of the given point",
        detail: "Point Object",
      },
      {
        label: "dim",
        type: "function",
        info: "Define picture dimension",
        detail: "w h",
      },
      {
        label: "area",
        type: "function",
        info: "Define the visible area of picture",
        detail: "x₁ y₁ x₂ y₂",
      },
      {
        label: "color",
        type: "function",
        info: "Set the current color",
        detail: "r g b",
      },
      {
        label: "background",
        type: "function",
        info: "Set the background color",
        detail: "r g b",
      },
      {
        label: "fontsize",
        type: "function",
        info: "Set the background color",
        detail: "s",
      },
      {
        label: "arrowstyle",
        type: "function",
        info: "Define the shape of arrows",
        detail: "n₁ n₂ n₃",
      },
      {
        label: "circleprecision",
        type: "function",
        info: "Define the circle drawing precision for LaTex export",
        detail: "n",
      },
      {
        label: "bezierprecision",
        type: "function",
        info: "Define the bezier drawing precision",
        detail: "n",
      },
      {
        label: "linethickness",
        type: "function",
        info: "Set line thickness in millimeters",
        detail: "n",
      },
      {
        label: "double",
        type: "function",
        info: "Makes all subsequent lines to be drawn with double thickness",
      },
      {
        label: "normal",
        type: "function",
        info: "Makes all subsequent lines to be drawn with normal thickness",
      },
      {
        label: "dash",
        type: "function",
        info: "Set length of dashes in millimeters",
        detail: "n",
      },
      {
        label: "dashstyle",
        type: "function",
        info: "Set style of dashes",
        detail: "n₁ n₂ n₃ n₄",
      },
      {
        label: "layer",
        type: "function",
        info: "Declare a layer",
        detail: "n",
      },
      {
        label: "hide_layer",
        type: "function",
        info: "Hides contents of the layer n",
        detail: "n",
      },
      {
        label: "hide_layers_from",
        type: "function",
        info: "Hides contents of all layers higher than n, including the layer n",
        detail: "n",
      },
      {
        label: "hide_layers_to",
        type: "function",
        info: "Hides contents of all layers lower than n, including the layer n",
        detail: "n",
      },
      {
        label: "animation_frames",
        type: "function",
        info: "Hides contents of all layers lower than n, including the layer n",
        detail: "n₁ n₂",
      },
      {
        label: "point",
        type: "type",
        info: "Define a moving point by its start and end coordinates",
        detail: "Name x₁ y₁ x₂ y₂",
      },
      {
        label: "trace",
        type: "type",
        info: "Trace a moving point defined by its start and end coordinates",
        detail: "Name x₁ y₁ x₂ y₂",
      },
      {
        label: "prove",
        type: "keyword",
        info: "Call theorem prover on given statement",
        detail: "{ Statement }",
      },
      {
        label: "prooflevel",
        type: "function",
        info: "Set the output level of the prove",
        detail: "n",
      },
      {
        label: "prooflimit",
        type: "function",
        info: "Set the the maximal number of proof step",
        detail: "n",
      },
      {
        label: "prover_timeout",
        type: "function",
        info: "Set the time (in seconds) available to the prover",
        detail: "n",
      },
      {
        label: "theorem_name",
        type: "function",
        info: "Set the name of the theorem",
        detail: "text",
      },
    ])
  ),
});

const gclcLanguageSupport = () => {
  return new LanguageSupport(gcLang, [gclCompletion]);
};

export { gclcLanguageSupport, highlightStyle };
