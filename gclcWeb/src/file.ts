import { compile } from "./compiler";
import { getCode, setCode } from "./editor";
import { openLink } from "./info";
import { resetOutputView } from "./outputView";
import { printLog, resetTerminal } from "./pane/terminal";
import { resetTimer } from "./pane/time";
import { closePopups, showPopup } from "./popup";

const defaultFileName = "out";

let FileName: string;

const numberOfExamples = 6;

const getFileName = (): string => {
  return FileName || defaultFileName;
};

const saveCode = () => {
  const file = new Blob([getCode()], {
    type: "text/plain",
  });

  window.URL = window.URL || window.webkitURL;

  const fileName = getFileName() + ".gcl";

  const downloadLink = document.createElement("a");
  downloadLink.setAttribute("href", window.URL.createObjectURL(file));
  downloadLink.setAttribute("download", fileName);
  downloadLink.style.display = "none";

  downloadLink.click();
};

const loadCode = () => {
  const input = document.createElement("input");
  input.type = "file";

  input.onchange = (e) => {
    if (e.target == null) {
      return;
    }

    const target = e.target as any;

    const file = target.files[0];
    const reader = new FileReader();

    reader.onload = (readerEvent) => {
      if (readerEvent.target == null) {
        return;
      }

      const content = readerEvent.target.result as string;
      if (input.files != null && input?.files?.length != 0) {
        let filename = input?.files[0].name.split(/(\\|\/)/g).pop();

        if (filename != undefined) {
          FileName = filename.split(".")[0];
        } else {
          FileName = defaultFileName;
        }

        document.title = getFileName() + " • GCLC";
        resetOutputView();
      }

      setCode(content);
      resetTimer();
      resetTerminal();
    };

    reader.readAsText(file, "UTF-8");
  };

  input.click();
};

const loadExample = (order: number) => () => {
  if (order < 1 || order > numberOfExamples) {
    return;
  }

  const exampleNames = [
    "triangle",
    "parametric",
    "cycloid",
    "3d",
    "menelaus",
    "tree",
  ];
  const fileName = exampleNames[order - 1];

  fetch(`examples/${fileName}.gcl`)
    .then()
    .then((response) => response.text())
    .then((text: string) => {
      FileName = fileName;
      document.title = fileName + " • GCLC";
      setCode(text);
      closePopups();
      resetOutputView();
      resetTimer();
      resetTerminal();
    })
    .catch(console.log);
};

const getFileExtension = (code: OutputType): String => {
  switch (code) {
    case "PIC":
    case "TIKZ":
    case "PST":
      return "tex";
    case "SVG":
      return "svg";
    case "EPS":
      return "eps";
  }
};

const exportPicture = () => {
  const filenameInput = document.getElementById(
    "filenameInput"
  ) as HTMLInputElement;

  const formatsSelect = document.getElementById(
    "formatsSelect"
  ) as HTMLSelectElement;

  const outputFormat = formatsSelect.value as OutputType;

  const code = getCode();
  const [outputData, log] = compile(
    code,
    getFileName(),
    outputFormat,
    "None",
    false
  );

  const errors = printLog(log);
  if (errors.length > 0) {
    return;
  }

  const file = new Blob([outputData], {
    type: "text/plain",
  });

  window.URL = window.URL || window.webkitURL;

  const fileName =
    (filenameInput.value || "gclcExport") +
    "." +
    getFileExtension(outputFormat);

  const downloadLink = document.createElement("a");
  downloadLink.setAttribute("href", window.URL.createObjectURL(file));
  downloadLink.setAttribute("download", fileName);
  downloadLink.style.display = "none";

  downloadLink.click();

  closePopups();
};

const setFileUI = () => {
  document.getElementById("buttonSave")?.addEventListener("click", saveCode);

  document.getElementById("buttonOpen")?.addEventListener("click", loadCode);

  document
    .getElementById("openExportPopupButton")
    ?.addEventListener("click", () => {
      const filenameInput = document.getElementById(
        "filenameInput"
      ) as HTMLInputElement;
      filenameInput.value = getFileName();

      showPopup("exportPopup")();
    });

  document
    .getElementById("openExamplesPopupButton")
    ?.addEventListener("click", showPopup("examplesPopup"));

  for (let i = 1; i <= numberOfExamples; i++) {
    document
      .getElementById(`example${i}`)
      ?.addEventListener("click", loadExample(i));
  }

  document
    .getElementById("moreExamplesButton")
    ?.addEventListener(
      "click",
      openLink("https://github.com/janicicpredrag/gclc/tree/master/samples")
    );

  document
    .getElementById("exportButton")
    ?.addEventListener("click", exportPicture);
};

export { getFileName, setFileUI };
