import { setTimeUI } from "./time";

const paneNames = ["terminal", "time", "settings"];

const openPane = (name: string) => () => {
  for (const paneName of paneNames) {
    const pane = document.getElementById(paneName + "Pane");
    if (pane == null) {
      continue;
    }
    const button = document.getElementById(paneName + "PaneButton");
    if (button == null) {
      continue;
    }
    if (paneName == name) {
      pane.classList.add("selectedPane");
      pane.classList.remove("nonSelectedPane");
      button.classList.add("selectedButton");
    } else {
      pane.classList.remove("selectedPane");
      pane.classList.add("nonSelectedPane");
      button.classList.remove("selectedButton");
    }
  }
};

const setPanesUI = () => {
  for (const pane of paneNames) {
    const button = document.getElementById(pane + "PaneButton");
    if (button != null) {
      button.addEventListener("click", openPane(pane));
    }
  }

  setTimeUI();
};

export { setPanesUI, openPane };
