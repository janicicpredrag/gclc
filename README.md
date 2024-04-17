# GCLC

GCLC is a mathematical software for producing high-quality mathematical
illustrations, for teaching mathematics, and for automated proving of
geometry theorems.

GCLC has been developed, with some idle years, since 1995. Its first
release made public (web) was built in 1996. There were several substantial
code revisions. The original version was written in pure C, and over time it
turned into C++ (while some C remains are still there). The current version
with graphical user interface uses Qt libraries.

The application was open-sourced in 2020, 25 years after its very first version.

GCLC has a command-line and a graphical version.

The source code and executables for Windows and Linux are available from
the [GCLC web page](http://www.matf.bg.ac.rs/~janicic/gclc/)
or from the [Github Release page](https://github.com/janicicpredrag/gclc/releases).

## Input

GCLC accepts files in GCL language, but can also import [JavaView](http://www.javaview.de/) files.

## Output

GCLC processes GCL files and can export generated illustrations in several
formats: simple LaTeX, LaTeX TikZ, LaTeX PSTricks, EPS, SVG and XML (textual specification).

Proofs by the automated theorem provers are generated in LaTeX form.

## Usage with command line

```
gclc filename -a|w|g -d -pic|tikz|pst|eps|svg|xml
```

- `filename` the name of the input file
- `-a` selects the theorem prover based on the area method
- `-w` selects the theorem prover based on Wu's method
- `-g` selects the theorem prover based on the Gröbner bases method
- `-d` enables deduction control
- `-pic` selects the simple LaTeX format for the output; this is the default value for output format
- `-tikz` selects the LaTeX TikZ format for the output
- `-pst` selects the LaTeX PStrics format
- `-eps` selects EPS format for the output
- `-svg` selects SVG format for the output
- `-xml` selects XML for the output.

For more information see [manual](manual/gclc_man.pdf).

## Executables

Pre-built executables (both the command line version and the version
with graphical user interface, both for Windows and for Linux) are
available in the folder 'executable'. Running these executables may
require installing some libraries (as `qt6`).

You can download latest executables (without code) from
[Github Release page](https://github.com/janicicpredrag/gclc/releases).
Package `windows.zip` contains Qt DLLs that can help you
with running GUI version of GCLC.

Executables also can be built from the available source code.

## Web version

Since 2024, GCLC can be run inside web browser.
GCLC web version works thanks to the [Emscripten](https://emscripten.org/)
compiler which can compile C/C++ code to a [WASM](https://webassembly.org/)
binary. WASM then can be run inside any major browser.

The web graphical interface mimics Qt graphical interface, but it is
independent form it. The web interface is written from scratch in Typescript.
This graphical interface communicates with console version of GCLC compiled to WASM.
However, web interface leans heavily on [Codemirror](https://codemirror.net/) for
code input and code completion.

Typescript code available in 'gclcWeb/' directory. File 'source/wGCLCmain.cpp'
is the main file for WASM build.

Web interface currently doesn't support export to raster formats, import from JavaView,
free points, watch window, deduction control, `animation_frames` and `trace` commands.

## Building from source code

GCLC is written in the C++ programming language and uses Qt libraries.
A C++ compiler is needed. Building GCLC with the
graphical user interface requires [Qt Library](https://www.qt.io/)
version 6.2 or higher.

In the following, it is assumed that `cmake` is available.

### Building the command line version

Inside the project root directory run:

```bash
cmake -B build -S .
cmake --build build --target gclc
```

The executable `gclc` will be created in the folder 'build'.

### Building the GUI version

Inside the project root directory run:

```bash
cmake -B build -S .
cmake --build build
```

The executable `gclc-gui` will be created in the folder 'build/source'.

### Using QTCreator

You can also load 'CMakeLists.txt' in QTCreator and build project from there.

### Building web version

Web version is build in three steps:

1.  **Compiling the GCLC to a WASM file**. This step is done via `emcc`,
    and described 'Makefile.web'.
2.  **Compiling the Typescript code** This is step is done via `npm`
    (using [Vite](https://vitejs.dev/)).
3.  Adding assets and deploying.

First, you will need to setup locally
[`npm`](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm) and
[`emcc`](https://emscripten.org/docs/getting_started/downloads.html). Once you have `emcc` available,
in the project root directory run:

```bash
make -f Makefile.web
```

This step produces 'gclc.wasm' and 'gclc.js' inside 'gclcWeb/'.

Now you have to install `npm` dependencies (only need to be done the first time).
Inside 'gclcWeb/' run:

```bash
npm install
```

After that run

```bash
npm run build
```

Whole website will be generated inside 'gclcWeb/dist' directory. You can publish
whole directory to the public server.

#### Editor grammar

Code editor is based on [Codemirror](https://codemirror.net/) library. Parser for
editor is generated from 'gclcWeb/gclcLanguage/gclc.grammar' file with `lezer-generator`:

```bash
npx lezer-generator gclc.grammar -o lang.js
```

#### Development server

While developing, you can start a development server with `npm run dev`. Dev server
reloads only when one of `ts` (`js`) files is changed.
`.wasm` and `.grammar` are not tracked.

## License

This software is licensed under the [MIT License](LICENSE.md).
