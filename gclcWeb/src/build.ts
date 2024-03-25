import { compile } from "./compiler";
import { getCode } from "./editor";
import { getFileName } from "./file";
import { setOutputView } from "./outputView";
import { openPane } from "./pane/panes";
import { printLog } from "./pane/terminal";
import {
  enableTheoremExport,
  getSelectedProofMethod,
  processAndDisplayXmlDocument,
  setProofs,
} from "./proof";

const displaySpinner = (display: boolean) => {
  const buildButton = document.getElementById("buildButton")!;
  const buildingButton = document.getElementById("buildingButton")!;

  if (display) {
    buildingButton.style.display = "flex";
    buildButton.style.display = "none";
  } else {
    buildButton.style.display = "flex";
    buildingButton.style.display = "none";
  }
};

const buildEventHandler = () => {
  displaySpinner(true);

  setTimeout(() => {
    const code = getCode();

    const [output, log, latexProof, xmlProof] = compile(
      code,
      getFileName(),
      "SVG",
      getSelectedProofMethod()
    );

    setProofs(latexProof, xmlProof);
    processAndDisplayXmlDocument();

    setOutputView(output);

    openPane("terminal")();

    displaySpinner(false);

    const errors = printLog(log);
    if (errors.length > 0) {
      return;
    }

    enableTheoremExport(true);
  }, 1);
};

const setBuildUI = () => {
  document
    .getElementById("buildButton")
    ?.addEventListener("click", buildEventHandler);
};

export { setBuildUI };
