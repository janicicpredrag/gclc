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

const exampleLanguage = LRLanguage.define({
  parser: parserWithMetadata,
  languageData: {},
});

const exampleCompletion = exampleLanguage.data.of({
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
        label: "point",
        type: "type",
        info: "Define a moving point by its start and end coordinates",
        detail: "Name x₁ y₁ x₂ y₂",
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
        type: "type",
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
      { label: "call", type: "keyword" },
      { label: "include", type: "keyword" },
      { label: "set_equal", type: "keyword" },
      { label: "intersec", type: "function" },
      { label: "intersec2", type: "function" },
      { label: "midpoint", type: "function" },
      { label: "med", type: "function" },
      { label: "bis", type: "function" },
      { label: "perp", type: "function" },
      { label: "foot", type: "function" },
      { label: "parallel", type: "function" },
      { label: "getcenter", type: "function" },
      { label: "onsegment", type: "function" },
      { label: "online", type: "function" },
      { label: "oncircle", type: "function" },
      { label: "translate", type: "function" },
      { label: "rotate", type: "function" },
      { label: "rotateonellipse", type: "function" },
      { label: "sim", type: "function" },
      { label: "turtle", type: "function" },
      { label: "towards", type: "function" },
      { label: "getx", type: "function" },
      { label: "getx", type: "function" },
      { label: "distance", type: "function" },
      { label: "angle", type: "function" },
      { label: "angle_o", type: "function" },
      { label: "random", type: "function" },
      { label: "drawpoint", type: "function" },
      { label: "drawsegment", type: "function" },
      { label: "drawdashsegment", type: "function" },
      { label: "drawline", type: "function" },
      { label: "drawdashline", type: "function" },
      { label: "drawvector", type: "function" },
      { label: "drawarrow", type: "function" },
      { label: "drawcircle", type: "function" },
      { label: "drawdashcircle", type: "function" },
      { label: "drawarc", type: "function" },
      { label: "drawarc_p", type: "function" },
      { label: "drawdasharc", type: "function" },
      { label: "drawdasharc_p", type: "function" },
      { label: "drawellipse", type: "function" },
      { label: "drawdashellipse", type: "function" },
      { label: "drawellipsearc", type: "function" },
      { label: "drawdashellipsearc", type: "function" },
      { label: "drawellipsearc1", type: "function" },
      { label: "drawdashellipsearc1", type: "function" },
      { label: "drawellipsearc2", type: "function" },
      { label: "drawdashellipsearc2", type: "function" },
      { label: "drawbezier3", type: "function" },
      { label: "drawdashbezier3", type: "function" },
      { label: "drawbezier4", type: "function" },
      { label: "drawpolygon", type: "function" },
      { label: "drawtree", type: "function" },
      { label: "drawgraph_a", type: "function" },
      { label: "drawgraph_b", type: "function" },
      { label: "filltriangle", type: "function" },
      { label: "fillrectangle", type: "function" },
      { label: "fillcircle", type: "function" },
      { label: "fillellipse", type: "function" },
      { label: "fillarc", type: "function" },
      { label: "fillarc0", type: "function" },
      { label: "fillellipsearc", type: "function" },
      { label: "fillellipsearc0", type: "function" },
      { label: "cmark", type: "function" },
      { label: "cmark_lt", type: "function" },
      { label: "cmark_l", type: "function" },
      { label: "cmark_lb", type: "function" },
      { label: "cmark_t", type: "function" },
      { label: "cmark_rt", type: "function" },
      { label: "cmark_r", type: "function" },
      { label: "cmark_rb", type: "function" },
      { label: "mark", type: "function" },
      { label: "mark_lt", type: "function" },
      { label: "mark_l", type: "function" },
      { label: "mark_lb", type: "function" },
      { label: "mark_t", type: "function" },
      { label: "mark_b", type: "function" },
      { label: "mark_rt", type: "function" },
      { label: "mark_r", type: "function" },
      { label: "mark_rb", type: "function" },
    ])
  ),
});

const gclcLanguageSupport = () => {
  return new LanguageSupport(exampleLanguage, [exampleCompletion]);
};

export { gclcLanguageSupport, highlightStyle };
