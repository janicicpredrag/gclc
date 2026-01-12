import createModule from "./gclc.js";

interface Pointer {}

interface FS {
  readdir: (path: string) => string[];
  readFile: Function;
  unlink: (path: string) => void;
}

interface BaseWASMModule {
  _malloc: (size: number) => Pointer;
  _calloc: (size: number) => Pointer;
  _free: (ptr: Pointer) => void;
  HEAPU8: Uint8Array;
  cwrap: (
    name: string,
    returnType: string | null,
    argTypes: string[]
  ) => Function;
  lengthBytesUTF8: Function;
  stringToUTF8: Function;
  getValue: Function;
  AsciiToString: (ptr: Pointer) => string;
  FS: FS;
}

type Render = (
  inputPtr: Pointer,
  filenamePtr: Pointer,
  outputPtr: Pointer,
  outputType: number,
  proofType: number,
  deductionControl: number,
  logPtr: Pointer
) => void;

type FastRender = (inputPtr: Pointer, outputPtr: Pointer) => void;

let Module: BaseWASMModule, render: Render, fastRender: FastRender;

let allocatedInputStringPtr: Pointer | 0 = 0;
let allocatedInputStringSize: number = -1;

const setupCompiler = () => {
  if (Module != undefined) {
    return;
  }

  (createModule() as Promise<BaseWASMModule>).then((module: BaseWASMModule) => {
    Module = module;

    render = Module.cwrap("render", null, [
      "number", // char* input
      "number", // char* filename
      "number", // char** outputPtr
      "number", // int outputType
      "number", // int proofType
      "number", // int deductionControl
      "number", // char **logPtr
    ]) as Render;

    fastRender = Module.cwrap("fastRender", null, [
      "number", // char* input
      "number", // char** outputPtr
    ]) as FastRender;
  });
};

const setInputMemory = (code: string): Pointer => {
  const codeSize = Module.lengthBytesUTF8(code) + 1;

  if (allocatedInputStringPtr == 0 || codeSize > allocatedInputStringSize) {
    if (allocatedInputStringPtr != 0) {
      Module._free(allocatedInputStringPtr);
      allocatedInputStringPtr = 0;
    }

    const newSize = Math.max(codeSize, allocatedInputStringSize * 2);

    try {
      allocatedInputStringPtr = Module._malloc(newSize);
      allocatedInputStringSize = newSize;
    } catch {
      allocatedInputStringPtr = 0;
      allocatedInputStringSize = 0;
    }
  }

  if (allocatedInputStringPtr == 0) {
    throw new Error("Cant allocate memory");
  }

  Module.stringToUTF8(code, allocatedInputStringPtr, codeSize);

  return allocatedInputStringPtr;
};

const compile = (
  code: string,
  filename: string,
  outputType: OutputType,
  proofMethod: ProofMethod,
  deductionControl: boolean
): [string, string, string, string] => {
  const inputPtr = setInputMemory(code);

  const filenameSize = Module.lengthBytesUTF8(filename) + 1;
  const filenamePtr = Module._malloc(filenameSize);
  Module.stringToUTF8(filename, filenamePtr, filenameSize);

  const outputPtr = Module._calloc(4);
  const logPtr = Module._calloc(4);

  render(
    inputPtr,
    filenamePtr,
    outputPtr,
    outputTypeValue(outputType),
    proofMethodValue(proofMethod),
    deductionControl ? 1 : 0,
    logPtr
  );

  Module._free(filenamePtr);

  let output = "";
  const outputPtrValue = Module.getValue(outputPtr, "i32");
  Module._free(outputPtr);
  if (outputPtrValue != 0) {
    output = Module.AsciiToString(outputPtrValue);
    Module._free(outputPtrValue);
  }

  let log = "";
  const logPtrValue = Module.getValue(logPtr, "i32");
  Module._free(logPtr);
  if (logPtrValue != 0) {
    log = Module.AsciiToString(logPtrValue);
    Module._free(logPtrValue);
  }

  let latexProof = "";
  let xmlProof = "";
  if (proofMethod != "None") {
    const files = Module.FS.readdir(".");
    const latexFile = files.filter((s) => s.includes(".tex"));
    if (latexFile.length == 1) {
      const path = "./" + latexFile[0];
      latexProof = Module.FS.readFile(path, { encoding: "utf8" });
      Module.FS.unlink(path);
    }

    const xmlFile = files.filter((s) => s.includes(".xml"));
    if (xmlFile.length == 1) {
      const path = "./" + xmlFile[0];
      xmlProof = Module.FS.readFile(path, { encoding: "utf8" });
      Module.FS.unlink(path);
    }
  }

  return [output, log, latexProof, xmlProof];
};

const fastCompile = (code: string): [string] => {
  const inputPtr = setInputMemory(code);
  const outputPtr = Module._calloc(4);

  fastRender(inputPtr, outputPtr);

  let output = "";
  const outputPtrValue = Module.getValue(outputPtr, "i32");
  Module._free(outputPtr);
  if (outputPtrValue != 0) {
    output = Module.AsciiToString(outputPtrValue);
    Module._free(outputPtrValue);
  }

  return [output];
};

const proofMethodValue = (method: ProofMethod): number => {
  switch (method) {
    case "None":
      return 0;
    case "Area":
      return 1;
    case "Wu":
      return 2;
    case "Grobner":
      return 2;
  }
};

const outputTypeValue = (code: OutputType): number => {
  switch (code) {
    case "PIC":
      return 0;
    case "TIKZ":
      return 1;
    case "PST":
      return 2;
    case "SVG":
      return 4;
    case "EPS":
      return 5;
  }
};

export { compile, setupCompiler, fastCompile };
