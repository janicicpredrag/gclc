import { EditorState } from "@codemirror/state";
import {
  autocompletion,
  closeBrackets,
  closeBracketsKeymap,
  completionKeymap,
} from "@codemirror/autocomplete";
import { defaultKeymap, history, historyKeymap } from "@codemirror/commands";
import {
  bracketMatching,
  foldGutter,
  syntaxHighlighting,
} from "@codemirror/language";
import {
  drawSelection,
  EditorView,
  highlightActiveLine,
  highlightActiveLineGutter,
  highlightSpecialChars,
  keymap,
  lineNumbers,
} from "@codemirror/view";
import { gclcLanguageSupport, highlightStyle } from "./gclcLanguage/gclcLang";
import { getCodeFromUrl } from "./share";
import { setOutputSize } from "./outputView";

const initialCodeExample = `point A 50 65
point B 45 35
point C 90 35

cmark_lt A
cmark_lb B
cmark_rb C

drawsegment A B
drawsegment B C
drawsegment C A

med a C B
med b A C
intersec O a b
drawcircle O A
`;

let editorView: EditorView;

const getCode = (): string => {
  return editorView.state.doc.toString();
};

const setCode = (code: string) => {
  editorView.dispatch({
    changes: {
      from: 0,
      to: editorView.state.doc.length,
      insert: code,
    },
  });
};

const resizeEditor = () => {
  editorView.requestMeasure();
  setOutputSize();
};

const repaintEditor = () => {
  editorView.requestMeasure();
};

const setEditorUI = () => {
  const inputDiv = document.getElementById("input")!;
  editorView = new EditorView({
    parent: inputDiv,
    state: EditorState.create({
      doc: "",
      extensions: [
        lineNumbers(),
        highlightActiveLineGutter(),
        highlightSpecialChars(),
        history(),
        foldGutter(),
        drawSelection(),
        EditorState.allowMultipleSelections.of(true),
        syntaxHighlighting(highlightStyle, { fallback: true }),
        bracketMatching(),
        closeBrackets(),
        autocompletion(),
        highlightActiveLine(),
        keymap.of([
          ...closeBracketsKeymap,
          ...defaultKeymap,
          ...historyKeymap,
          ...completionKeymap,
        ]),
        gclcLanguageSupport(),
      ],
    }),
  });

  const codeFromUrl = getCodeFromUrl();
  editorView.dispatch({
    changes: {
      from: 0,
      to: editorView.state.doc.length,
      insert: codeFromUrl || initialCodeExample,
    },
  });

  const ro = new ResizeObserver(resizeEditor);
  ro.observe(inputDiv);
};

export { getCode, setEditorUI, setCode, repaintEditor };
