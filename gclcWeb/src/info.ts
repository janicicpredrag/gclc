import { showPopup } from "./popup";

const openLink = (url: string) => () => {
  window.open(url, "_blank")?.focus();
};

const setInfoUI = () => {
  document
    .getElementById("aboutButton")
    ?.addEventListener("click", showPopup("aboutPopup"));

  document
    .getElementById("manualButton")
    ?.addEventListener(
      "click",
      openLink(
        "http://poincare.matf.bg.ac.rs/~predrag.janicic/gclc/gclc_man.pdf"
      )
    );

  document
    .getElementById("githubButton")
    ?.addEventListener(
      "click",
      openLink("https://github.com/janicicpredrag/gclc")
    );
};

export { setInfoUI, openLink };
