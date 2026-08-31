import { EditorState, StateEffect, StateField } from "@codemirror/state";
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
  Decoration,
  type DecorationSet,
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
import { refreshOutputAtCurrentTime } from "./pane/time";

const initialCodeExample = `point A 50 65
point B 45 35
point C 90 35

point D 99 39 49 39

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

const setErrorLinesEffect = StateEffect.define<readonly number[]>();

const errorLineDecoration = Decoration.line({ class: "cm-errorLine" });

const errorLinesField = StateField.define<DecorationSet>({
  create() {
    return Decoration.none;
  },
  update(decorations, transaction) {
    for (const effect of transaction.effects) {
      if (effect.is(setErrorLinesEffect)) {
        const ranges = [...new Set(effect.value)]
          .filter((line) => line >= 1 && line <= transaction.state.doc.lines)
          .sort((a, b) => a - b)
          .map((line) =>
            errorLineDecoration.range(transaction.state.doc.line(line).from)
          );
        return Decoration.set(ranges);
      }
    }

    if (transaction.docChanged) {
      return Decoration.none;
    }

    return decorations;
  },
  provide: (field) => EditorView.decorations.from(field),
});

let editorView: EditorView;

const setErrorLines = (lines: number[]) => {
  if (editorView == undefined) {
    return;
  }

  const validLines = [...new Set(lines)].filter(
    (line) => line >= 1 && line <= editorView.state.doc.lines
  );

  const effects: StateEffect<unknown>[] = [setErrorLinesEffect.of(validLines)];
  if (validLines.length > 0) {
    const firstLine = editorView.state.doc.line(validLines[0]);
    effects.push(EditorView.scrollIntoView(firstLine.from, { y: "nearest" }));
  }

  editorView.dispatch({ effects });
};

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

const repositionPoint = (name: string, x: number, y: number) => {
  const code = getCode();

  var regEx = new RegExp('point\\s+'+name+'\\s+(-?\\d+(\\.\\d+)?)\\s+(-?\\d+(\\.\\d+)?)', 'gi');
  const newCode = code.replace(regEx, `point ${name} ${x.toLocaleString('en-US')} ${y.toLocaleString('en-US')}`)

  setCode(newCode)
  refreshOutputAtCurrentTime()
}

const repositionPoint2 = (name: string, x: number, y: number, z: number, w: number) => {
  const code = getCode();

  var regEx = new RegExp('point\\s+'+name+'\\s+(-?\\d+(\\.\\d+)?)\\s+(-?\\d+(\\.\\d+)?)\\s+(-?\\d+(\\.\\d+)?)\\s+(-?\\d+(\\.\\d+)?)', 'gi');
  const newCode = code.replace(regEx, `point ${name} ${x.toLocaleString('en-US')} ${y.toLocaleString('en-US')} ${z.toLocaleString('en-US')} ${w.toLocaleString('en-US')}`)

  setCode(newCode)
  refreshOutputAtCurrentTime()
}

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
        errorLinesField,
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

export {
  getCode,
  setEditorUI,
  setCode,
  setErrorLines,
  repaintEditor,
  repositionPoint,
  repositionPoint2,
};
