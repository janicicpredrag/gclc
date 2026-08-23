import Panzoom, { PanzoomObject } from "@panzoom/panzoom";
import { getCode, repositionPoint, repositionPoint2 } from "./editor";

type Coordinate = {
  x: number;
  y: number;
};

const POINT_RADIUS = 6;

let panzoom: PanzoomObject;
let svgBox: HTMLElement;
let freePointsShown: boolean;
let svgWidthMM: number, svgHeightMM: number;
let svgWidthPx: number, svgHeightPx: number;

const setOutputViewUI = () => {
  svgBox = document.getElementById("svgBox")!;
  panzoom = Panzoom(svgBox, {
    maxScale: 5,
    minScale: 0.5,
    animate: true,
    overflow: "hidden",
  });

  svgBox.parentElement?.addEventListener("wheel", (e) => {
    panzoom.zoomWithWheel(e);
  });

  document.getElementById("buttonReset")?.addEventListener("click", (_) => {
    panzoom.reset();
  });

  document
    .getElementById("showFreePointsButton")
    ?.addEventListener("click", showFreePointsHandler);

  freePointsShown = true;
};

const showFreePointsHandler = () => {
  const buildButtonSpan = document.getElementById("showFreePointsSpan")!;

  freePointsShown = !freePointsShown;

  if (freePointsShown) {
    buildButtonSpan.textContent = "Hide free points";
    svgBox.classList.remove("hideFreePoints");
  } else {
    buildButtonSpan.textContent = "Show free points";
    svgBox.classList.add("hideFreePoints");
  }
};

const setOutputView = (svgSource: string) => {
  svgBox!.innerHTML = svgSource.replace(
    `<?xml version="1.0" encoding="UTF-8"?> `,
    ""
  );

  const dimRegex = /<svg width="(\d+)mm" height="(\d+)mm"/g;

  const svgDimensionMatch = [...svgSource.matchAll(dimRegex)][0];

  if (svgDimensionMatch == undefined || svgDimensionMatch == null) {
    return;
  }

  svgWidthMM = parseFloat(svgDimensionMatch[1]);
  svgHeightMM = parseFloat(svgDimensionMatch[2]);

  setOutputSize();

  // clientWidth/Height ignore CSS transforms, so zoom does not
  // inflate the millimetre-to-pixel mapping used for free points.
  svgWidthPx = svgBox.clientWidth;
  svgHeightPx = svgBox.clientHeight;

  if (freePointsShown) {
    constructFreePoints();
  }
};

const constructFreePoints = () => {
  const code = getCode();
  const number = String.raw`(-?\d+(?:\.\d+)?)`;
  const pointRgx = new RegExp(
    String.raw`\bpoint\s+(\w+)\s+${number}\s+${number}(?:\s+${number}\s+${number})?`,
    "g"
  );

  for (const c of code.matchAll(pointRgx)) {
    if (c[4] !== undefined) {
      constructFreePoint2({
        name: c[1],
        x: parseFloat(c[2]),
        y: parseFloat(c[3]),
        z: parseFloat(c[4]),
        w: parseFloat(c[5]),
      });
    } else {
      constructFreePoint({
        name: c[1],
        x: parseFloat(c[2]),
        y: parseFloat(c[3]),
      });
    }
  }
};

const constructFreePoint = ({
  name,
  x,
  y,
}: {
  name: string;
  x: number;
  y: number;
}) => {
  createDraggablePoint({
    name,
    x,
    y,
    classNames: ["freePoint", "panzoom-exclude"],
    onDragEnd: (pos) => {
      const mm = px2mm(pos);
      repositionPoint(name, mm.x, mm.y);
    },
  });
};

const constructFreePoint2 = ({
  name,
  x,
  y,
  z,
  w,
}: {
  name: string;
  x: number;
  y: number;
  z: number;
  w: number;
}) => {
  createDraggablePoint({
    name: name + "1",
    x,
    y,
    classNames: ["freePoint", "panzoom-exclude"],
    onDragEnd: (pos) => {
      const mm = px2mm(pos);
      repositionPoint2(name, mm.x, mm.y, z, w);
    },
  });

  createDraggablePoint({
    name: name + "2",
    x: z,
    y: w,
    classNames: ["freePoint", "doubleFreePoint", "panzoom-exclude"],
    onDragEnd: (pos) => {
      const mm = px2mm(pos);
      repositionPoint2(name, x, y, mm.x, mm.y);
    },
  });
};

const createDraggablePoint = ({
  name,
  x,
  y,
  classNames,
  onDragEnd,
}: {
  name: string;
  x: number;
  y: number;
  classNames: string[];
  onDragEnd: (pos: Coordinate) => void;
}) => {
  const pointElement = document.createElement("div");
  pointElement.classList.add(...classNames);

  const elementName = document.createElement("div");
  elementName.innerText = name;
  pointElement.appendChild(elementName);
  svgBox.appendChild(pointElement);

  let pos = mm2px({ x, y });
  setPointPosition(pointElement, pos);

  pointElement.addEventListener("pointerdown", (event) => {
    if (event.button !== 0) {
      return;
    }

    event.preventDefault();
    event.stopPropagation();
    pointElement.setPointerCapture(event.pointerId);

    const scale = panzoom.getScale();
    const start = { ...pos };
    const originX = event.clientX;
    const originY = event.clientY;

    const onMove = (moveEvent: PointerEvent) => {
      pos = {
        x: start.x + (moveEvent.clientX - originX) / scale,
        y: start.y + (moveEvent.clientY - originY) / scale,
      };
      setPointPosition(pointElement, pos);
    };

    const onUp = () => {
      pointElement.removeEventListener("pointermove", onMove);
      pointElement.removeEventListener("pointerup", onUp);
      pointElement.removeEventListener("pointercancel", onUp);
      onDragEnd(pos);
    };

    pointElement.addEventListener("pointermove", onMove);
    pointElement.addEventListener("pointerup", onUp);
    pointElement.addEventListener("pointercancel", onUp);
  });
};

const setPointPosition = (element: HTMLElement, { x, y }: Coordinate) => {
  element.style.transform = `translate(${x}px, ${y}px)`;
};

const mm2px = ({ x, y }: Coordinate): Coordinate => {
  const xpx = (x / svgWidthMM) * svgWidthPx;
  const ypx = svgHeightPx - (y / svgHeightMM) * svgHeightPx;

  return {
    x: xpx - POINT_RADIUS,
    y: ypx - POINT_RADIUS,
  };
};

const px2mm = ({ x, y }: Coordinate): Coordinate => {
  const xmm = ((x + POINT_RADIUS) / svgWidthPx) * svgWidthMM;
  const ymm = ((svgHeightPx - (y + POINT_RADIUS)) / svgHeightPx) * svgHeightMM;

  return {
    x: xmm,
    y: ymm,
  };
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
