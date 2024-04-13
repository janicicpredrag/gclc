import { getFileName } from "./file";
import { closePopups, showPopup } from "./popup";

let SelectedProofMethod: ProofMethod = "None";
let LatexProof: string, XmlProof: string;
let DeductionControl: boolean;

const setProverMethod = (method: ProofMethod) => () => {
  SelectedProofMethod = method;

  const buttonIds = [
    "areaMethodButton",
    "wuMethodButton",
    "grobnerMethodButton",
  ];
  for (const buttonId of buttonIds) {
    const button = document.getElementById(buttonId);
    if (button != null) {
      if (method.toLowerCase() + "MethodButton" == buttonId) {
        button.classList.add("selectedMethod");
      } else {
        button.classList.remove("selectedMethod");
      }
    }
  }
};

const setProver = (status: boolean) => () => {
  const dropdown = document.getElementById("proverDropdownContent")!;
  const buildButtonSpan = document.getElementById("buildButtonSpan")!;

  if (status) {
    dropdown.classList.add("proverEnabled");
    buildButtonSpan.innerText = "Build and prove";
    setProverMethod("Area")();
  } else {
    dropdown.classList.remove("proverEnabled");
    buildButtonSpan.innerText = "Build";
  }

  const dropdownButton = document.getElementById(
    "proverDropdownButton"
  ) as HTMLButtonElement;
  dropdownButton.focus();
};

const setProofs = (latex: string, xml: string) => {
  LatexProof = latex;
  XmlProof = xml;
};

const getProofs = (): [string, string] => {
  return [LatexProof, XmlProof];
};

const getSelectedProofMethod = (): ProofMethod => {
  return SelectedProofMethod;
};

const setDeductionControlHandler = (control: boolean) => () => {
  DeductionControl = control;

  const enableButton = document.getElementById(
    "enableDeductionControlButton"
  ) as HTMLElement;
  const disableButton = document.getElementById(
    "disableDeductionControlButton"
  ) as HTMLElement;

  if (control) {
    enableButton.style.display = "none";
    disableButton.style.display = "block";
  } else {
    enableButton.style.display = "block";
    disableButton.style.display = "none";
  }
};

const getDeductionControl = (): boolean => {
  return DeductionControl;
};

const enableTheoremExport = (enable: boolean) => {
  const button = document.getElementById(
    "openExportProofPopupButton"
  ) as HTMLButtonElement;

  button.disabled = !enable;
};

const exportProof = () => {
  const exportType = (
    document.getElementById("formatsProofSelect") as HTMLSelectElement
  ).value;

  saveProof(exportType);
  closePopups();
};

const saveProof = (exportType: string) => {
  const proof = exportType == "LATEX" ? LatexProof : XmlProof;

  const file = new Blob([proof], {
    type: "text/plain",
  });

  window.URL = window.URL || window.webkitURL;

  const fileName = getFileName() + (exportType == "LATEX" ? ".tex" : ".xml");

  const downloadLink = document.createElement("a");
  downloadLink.setAttribute("href", window.URL.createObjectURL(file));
  downloadLink.setAttribute("download", fileName);
  downloadLink.style.display = "none";

  downloadLink.click();
  closePopups();
};

const processAndDisplayXmlDocument = () => {
  if (XmlProof == "" || SelectedProofMethod == "None") {
    return;
  }

  showPopup("proofViewPopup")();
};

const setProofUI = () => {
  document
    .getElementById("enableProverButton")
    ?.addEventListener("click", setProver(true));

  document
    .getElementById("disableProverButton")
    ?.addEventListener("click", setProver(false));

  document
    .getElementById("areaMethodButton")
    ?.addEventListener("click", setProverMethod("Area"));

  document
    .getElementById("wuMethodButton")
    ?.addEventListener("click", setProverMethod("Wu"));

  document
    .getElementById("grobnerMethodButton")
    ?.addEventListener("click", setProverMethod("Grobner"));

  document
    .getElementById("openExportProofPopupButton")
    ?.addEventListener("click", () => {
      const filenameProofInput = document.getElementById(
        "filenameProofInput"
      ) as HTMLInputElement;
      filenameProofInput.value = getFileName();

      showPopup("exportProofPopup")();
    });

  document
    .getElementById("exportProofButton")
    ?.addEventListener("click", exportProof);

  document
    .getElementById("proofViewExportXmlButton")
    ?.addEventListener("click", () => saveProof("XML"));

  document
    .getElementById("proofViewExportLaTexButton")
    ?.addEventListener("click", () => saveProof("LATEX"));

  document
    .getElementById("enableDeductionControlButton")
    ?.addEventListener("click", setDeductionControlHandler(true));

  document
    .getElementById("disableDeductionControlButton")
    ?.addEventListener("click", setDeductionControlHandler(false));
};

export {
  setProofUI,
  setProofs,
  getProofs,
  getSelectedProofMethod,
  processAndDisplayXmlDocument,
  enableTheoremExport,
  getDeductionControl,
};
