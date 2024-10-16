# GCLC

GCLC is a mathematical software for producing high-quality mathematical
illustrations, for teaching mathematics, and for automated proving of
geometry theorems.

GCLC has been developed, with some idle years, since 1995. Its first
release was made public in 1996. There were several substantial
code revisions. The original version was written in pure C, and over time it
turned into C++ (while some C remains are still there).

The application was open-sourced in 2020, 25 years after its very first version.
In 2024, GCLC got ported to the web.

GCLC has a command line and a graphical (GUI) version. The standalone graphical
version is based on the Qt framework, while the web graphical version is custom-built.

GCLC source code is available on the [GitHub](https://github.com/janicicpredrag/gclc).
Executables for Windows and Linux are available on
the [GCLC web page](http://www.matf.bg.ac.rs/~janicic/gclc/)
and [Github Release page](https://github.com/janicicpredrag/gclc/releases).

## Input

GCLC accepts files in GC (Geometry Constructions) language, but can also import [JavaView](http://www.javaview.de/) files.

## Output

GCLC processes GCL files and can export generated illustrations in several
formats: simple LaTeX, LaTeX TikZ, LaTeX PSTricks, EPS, SVG and XML (textual specification).

Proofs by the automated theorem provers are generated in a LaTeX form.

## Usage with command line

```
gclc FILE [-b] [-a|-w|-g] [-d] [-pic|-tikz|-pst|-eps|-svg|-xml] [-help] [-version]
```

- `FILE` the name of the input file
- `-b` enables batch mode
- `-a` selects the theorem prover based on the area method
- `-w` selects the theorem prover based on Wu's method
- `-g` selects the theorem prover based on the Gröbner bases method
- `-d` enables deduction control
- `-pic` selects the simple LaTeX format for the output; this is the default value for output format
- `-tikz` selects the LaTeX TikZ format for the output
- `-pst` selects the LaTeX PStrics format
- `-eps` selects EPS format for the output
- `-svg` selects SVG format for the output
- `-xml` selects XML for the output
- `-help` list command line options and explanations
- `-version` print GCLC version and exit

For more information see [manual](manual/gclc_man.pdf).

## Executables

You can download releases from the
[Release page](https://github.com/janicicpredrag/gclc/releases).

Currently, there are four builds provided:

- `linux.dynamic.zip` - Linux version, dynamically linked to the Qt libraries
- `linux.static.zip` - Linux version with statically linked Qt libraries
- `windows.zip` - Windows version with statically linked Qt libraries
- `web.zip` - web version with all assets required

The Linux and Windows releases contain the command line and GUI executables, as well as
a documentation, samples, and packages for LaTeX support.

The Linux version that uses dynamic linking is smaller in size, but requires
dynamic Qt libraries to be available on the system. [ If your Qt6 libs have 
not been installed in `/lib/x86...` but in `/home/.../Qt/...` 
and they cannot be dynamically linked, this could help:
`sudo ldconfig /home/.../Qt/6.7.0/gcc_64/lib` ]

Executables also can be built from the available source code following the
instructions provided below.

## Web version

Since 2024, GCLC can be run inside a web browser.
GCLC web version works thanks to the [Emscripten](https://emscripten.org/)
compiler which can compile C/C++ code to a [WASM](https://webassembly.org/)
binary. WASM then can be run inside any major browser.

The web graphical interface mimics the Qt graphical interface, but it is
independent of it. The web interface is written from scratch in Typescript.
This graphical interface communicates with the console version of GCLC compiled to WASM.
However, the web interface leans heavily on [Codemirror](https://codemirror.net/) for
code input and code completion.

Typescript code is available in the 'gclcWeb/' directory. File 'source/wGCLCmain.cpp'
is the main file for the WASM build.

The web interface currently doesn't support export to raster formats, import from JavaView,
free points, watch window, `animation_frames` and `trace` commands.

## Building from source code

GCLC is written in the C++ programming language, so a C++ compiler is needed.

GCLC with the graphical user interface uses Qt libraries, so
building from source code requires [Qt Library](https://www.qt.io/)
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

For the GUI version you will have to have `Qt >=6.2` available on your system.

Inside the project root directory run:

```bash
cmake -B build -S .
cmake --build build
```

The executable `gclc-gui` will be created in the folder 'build/source'.

The executables `gclc` and `gclc-gui` will be stored into the directory 'executable/' by running:

```bash
cmake --build build --target install
```

### Using QTCreator

You can also load 'CMakeLists.txt' in QtCreator and build project from there.

### Building the web version

The web version is build in two steps:

1.  **Compiling the GCLC to a WASM file**. This step is done via Emscripten
2.  **Compiling the Typescript code** This step is done via `npm`
    (using [Vite](https://vitejs.dev/)).

First, you will need to setup locally
[`npm`](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm) and
[`emcc`](https://emscripten.org/docs/getting_started/downloads.html). Once you have `emcc` available,
in the project root directory configure project with `emcmake` provided by Emscripten, and compile it:

```bash
emcmake cmake -B build -S gclcWeb
cmake --build build
```

This step produces 'gclc.wasm' and 'gclc.js' inside 'build/'. Copy those two files to the 'src' directory:

```bash
cp build/gclc.js build/gclc.wasm gclcWeb/src/
```

This concludes the first step. Now you have to install `npm` dependencies (only need to be done the first time).
Inside 'gclcWeb/' run:

```bash
npm install
```

After that, run

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
reloads when one of `ts` (`js`) files is changed.
Changes to `.wasm` and `.grammar` files are not tracked.

## License

This software is licensed under the [MIT License](LICENSE.md).
