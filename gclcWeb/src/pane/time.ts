import { compile, fastCompile } from "../compiler";
import { getCode } from "../editor";
import { getFileName } from "../file";
import { setOutputView } from "../outputView";
import { openPane } from "./panes";
import { printLog, resetTerminal } from "./terminal";

type LoopMode = "once" | "loop" | "oscillate";

let playing: Boolean = false;
let currentTime: number = 0;
let lastTimeStamp: DOMHighResTimeStamp | undefined = undefined;
let speed: number = 1;
let loopMode: LoopMode = "loop";

let sliderMax: number = 5000;
let slider: HTMLInputElement;

const speeds: [number, string][] = [0.5, 1, 2, 3].map((s) => [
  s,
  `${s}`.replace(".", ""),
]);

const loops: LoopMode[] = ["loop", "once", "oscillate"];

const movablePointRgx =
  /\bpoint\s+(\w+)\s+(-?\d+(\.\d+)?)\s+(-?\d+(\.\d+)?)\s+(-?\d+(\.\d+)?)\s+(-?\d+(\.\d+)?)/;

let tempCode = "";

const onSliderInput = (e: Event) => {
  const target = e?.target;
  if (target == null) {
    return;
  }

  currentTime = Number((target as HTMLInputElement).value) / sliderMax;

  tempCode = getCode();

  if ((tempCode.match(movablePointRgx) || []).length == 0) {
    return;
  }

  const [_, log] = compile(tempCode, getFileName(), "SVG", "None");

  const errors = printLog(log);
  if (errors.length > 0) {
    resetTimer();
    openPane("terminal")();
    return;
  }

  renderAtTime();
};

const renderAtTime = () => {
  const newCode = tempCode.replace(movablePointRgx, (...args): string => {
    if (args.length > 10) {
      const name = args[1];
      const x1 = Number(args[2]);
      const y1 = Number(args[4]);
      const x2 = Number(args[6]);
      const y2 = Number(args[8]);

      const x3 = x1 + (x2 - x1) * currentTime;
      const y3 = y1 + (y2 - y1) * currentTime;

      return `point ${name} ${x3} ${y3}`;
    } else {
      return args[0];
    }
  });

  const [output] = fastCompile(newCode);
  setOutputView(output);
};

const pausePlaying = () => {
  playing = false;

  const playButton = document.getElementById("playButton")!;
  playButton.style.display = "flex";

  const playButton2 = document.getElementById("buttonPlay")!;
  playButton2.style.display = "flex";

  const pauseButton = document.getElementById("pauseButton")!;
  pauseButton.style.display = "none";

  const pauseButton2 = document.getElementById("buttonPause")!;
  pauseButton2.style.display = "none";
};

const startPlaying = () => {
  resetTerminal();

  if (playing) {
    return;
  }

  tempCode = getCode();

  if ((tempCode.match(movablePointRgx) || []).length == 0) {
    printLog("Error: nothing to animate");
    openPane("terminal")();
    return;
  }

  const [_, log] = compile(tempCode, getFileName(), "SVG", "None");

  const errors = printLog(log);
  if (errors.length > 0) {
    resetTimer();
    openPane("terminal")();
    return;
  }

  if (loopMode == "once" && currentTime == 1) {
    currentTime = 0;
  }

  const playButton = document.getElementById("playButton")!;
  playButton.style.display = "none";

  const playButton2 = document.getElementById("buttonPlay")!;
  playButton2.style.display = "none";

  const pauseButton = document.getElementById("pauseButton")!;
  pauseButton.style.display = "flex";

  const pauseButton2 = document.getElementById("buttonPause")!;
  pauseButton2.style.display = "flex";

  openPane("time")();

  playing = true;

  lastTimeStamp = undefined;
  window.requestAnimationFrame(nextFrame);
};

const nextFrame = (timeStamp: DOMHighResTimeStamp) => {
  if (!playing) {
    return;
  }

  if (lastTimeStamp != undefined) {
    currentTime += ((timeStamp - lastTimeStamp) / 10000) * speed;
    lastTimeStamp = timeStamp;
  } else {
    lastTimeStamp = timeStamp;
  }

  renderAtTime();

  if (currentTime >= 1) {
    switch (loopMode) {
      case "once":
        currentTime = 1;
        pausePlaying();
        break;
      case "loop":
        currentTime = 0;
        break;
      case "oscillate":
        speed = -1 * speed;
        break;
    }
  }

  if (currentTime < 0) {
    switch (loopMode) {
      case "once":
      case "loop":
        currentTime = 0;
        pausePlaying();
        break;
      case "oscillate":
        speed = -1 * speed;
        break;
    }
  }

  slider.value = `${currentTime * sliderMax}`;

  window.requestAnimationFrame(nextFrame);
};

const resetTimer = () => {
  pausePlaying();
  currentTime = 0;
  slider.value = "0";
};

const setSpeed = (id: string, newSpeed: number) => () => {
  for (let s of speeds) {
    const button = document.getElementById(`speed${s[1]}Button`)!;

    if (s[1] == id) {
      button.classList.add("selectedButton");
    } else {
      button.classList.remove("selectedButton");
    }
  }

  speed = newSpeed;
};

const setLoop = (id: LoopMode) => () => {
  for (let l of loops) {
    const button = document.getElementById(`${l}LoopButton`)!;

    if (l == id) {
      button.classList.add("selectedButton");
    } else {
      button.classList.remove("selectedButton");
    }
  }

  loopMode = id;
};

const setTimeUI = () => {
  slider = document.getElementById("slider") as HTMLInputElement;
  slider.addEventListener("input", onSliderInput);

  document
    .getElementById("playButton")
    ?.addEventListener("click", () => startPlaying());

  document
    .getElementById("buttonPlay")
    ?.addEventListener("click", () => startPlaying());

  document
    .getElementById("pauseButton")
    ?.addEventListener("click", () => pausePlaying());

  document
    .getElementById("buttonPause")
    ?.addEventListener("click", () => pausePlaying());

  for (let s of speeds) {
    document
      .getElementById(`speed${s[1]}Button`)
      ?.addEventListener("click", setSpeed(s[1], s[0]));
  }

  for (let l of loops) {
    document
      .getElementById(`${l}LoopButton`)
      ?.addEventListener("click", setLoop(l));
  }
};

export { setTimeUI, resetTimer };
