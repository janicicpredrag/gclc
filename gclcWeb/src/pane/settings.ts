import { repaintEditor } from "../editor";

const initSettings = () => {
  let theme = localStorage.getItem("theme");
  if (theme === null) {
    const systemSettingDark = window.matchMedia("(prefers-color-scheme: dark)");
    if (systemSettingDark.matches) {
      theme = "dark";
    } else {
      theme = "light";
    }

    localStorage.setItem("theme", theme);
  }

  document.querySelector("body")?.setAttribute("data-theme", theme);
  (document.getElementById("darkThemeCheckbox") as HTMLInputElement).checked =
    theme == "dark";

  let fontSize = localStorage.getItem("fontSize");
  if (fontSize == null) {
    fontSize = "12";
  }

  (document.getElementById("input") as HTMLElement).style.fontSize =
    fontSize + "px";

  (document.getElementById("fontSizeInput") as HTMLInputElement).value =
    fontSize;
};

const changeThemeEvent = (e: Event) => {
  if (e.target == null) {
    return;
  }

  const target = e.target as HTMLInputElement;

  let theme = target.checked ? "dark" : "light";
  document.querySelector("body")?.setAttribute("data-theme", theme);
  localStorage.setItem("theme", theme);
};

const changeFontSizeEvent = (e: Event) => {
  if (e.target == null) {
    return;
  }

  const target = e.target as HTMLInputElement;
  const fontSize = target.value;

  (document.getElementById("input") as HTMLElement).style.fontSize =
    fontSize + "px";

  repaintEditor();

  localStorage.setItem("fontSize", fontSize);
};

const setSettingsUI = () => {
  document
    .getElementById("darkThemeCheckbox")
    ?.addEventListener("change", changeThemeEvent);

  document
    .getElementById("fontSizeInput")
    ?.addEventListener("change", changeFontSizeEvent);
};

export { setSettingsUI, initSettings };
