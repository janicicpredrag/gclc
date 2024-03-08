import Panzoom, { PanzoomObject } from "@panzoom/panzoom";

let panzoom: PanzoomObject;
let svgBox: HTMLElement;

const setOutputViewUI = () => {
  svgBox = document.getElementById("svgBox")!;
  panzoom = Panzoom(svgBox, {
    maxScale: 5,
    minScale: 0.5,
    animate: true,
  });
  svgBox.parentElement?.addEventListener("wheel", panzoom.zoomWithWheel);

  document
    .getElementById("buttonReset")
    ?.addEventListener("click", (_) => panzoom.reset());
};

const setOutputView = (svgSource: string) => {
  svgBox!.innerHTML = svgSource.replace(
    `<?xml version="1.0" encoding="UTF-8"?> `,
    ""
  );

  setOutputSize();
};

const setOutputSize = () => {
  const dim = document.getElementById("input")!.getBoundingClientRect();

  let vw = Math.max(
    document.documentElement.clientWidth || 0,
    window.innerWidth || 0
  );

  const outputDiv = document.getElementById("output")!;
  outputDiv.style.width = `${vw - dim.width}px`;
};

const resetOutputView = () => {
  svgBox!.innerHTML = "";
  panzoom?.reset();
};

export { setOutputViewUI, setOutputView, resetOutputView, setOutputSize };
