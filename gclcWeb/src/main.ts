import { setShareUI } from "./share";
import { setPopupUI } from "./popup";
import { setProofUI } from "./proof";
import { setEditorUI } from "./editor";
import { setFileUI } from "./file";
import { setOutputViewUI } from "./outputView";
import { setInfoUI } from "./info";
import { setupCompiler } from "./compiler";
import { setPanesUI } from "./pane/panes";
import { initSettings, setSettingsUI } from "./pane/settings";
import { setBuildUI } from "./build";

const setUI = () => {
  setProofUI();
  setEditorUI();
  setFileUI();
  setPopupUI();
  setShareUI();
  setOutputViewUI();
  setInfoUI();
  setPanesUI();
  setSettingsUI();
  setBuildUI();
};

initSettings();
setUI();
setupCompiler();
