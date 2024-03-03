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
gclc filename -a|w|g -pic|tikz|pst|eps|svg|xml
```

 + `filename` the name of the input file 
 + `-a` selects the theorem prover based on the area method
 + `-w` selects the theorem prover based on Wu's method
 + `-g` selects the theorem prover based on the Gröbner bases method
 + `-pic` selects the simple LaTeX format for the output; this is the default value for output format
 + `-tikz` selects the LaTeX TikZ format for the output
 + `-pst` selects the LaTeX PStrics format 
 + `-eps` selects EPS format for the output
 + `-svg` selects SVG format for the output
 + `-xml` selects XML for the output.

For more information see [manual](manual/gclc_man.pdf).

## Executables

Pre-built executables (both the command line version and the version
with graphical user interface, both for Windows and for Linux) are 
available in the folder 'executable'. Running these executables may
require installing some libraries (for instance, by 
`sudo apt-get install qt5-default` on linux).

You can download latest executables (without code) from
[Github Release page](https://github.com/janicicpredrag/gclc/releases).
Package `windows.zip` contains Qt DLLs that can help you
with running GUI version of GCLC.

Executables also can be built from the available source code.

## Building from source code

GCLC is written in the C++ programming language and uses Qt libraries.
GNU C++ g++ compiler is needed (or some other C++ compiler, but then
Makefile and .pro files should be updated). Building GCLC with the
graphical user interface requires [Qt Library](https://www.qt.io/)
version 6.4 or higher.

In the following, it is assumed that `make` is available.
Under Windows, one can use `mingw32-make` instead, for instance.

### Building gclc (command line version) from the terminal using the provided Makefile

Under Linux, go to the folder 'gclc', then:

```
make
```

Under Windows, go to the folder 'gclc', then:

```
mingw32-make -f Makefile.win
```

The executable (release version) 'gclc' will be created in the folder 'gclc/executable'.


### Building gclc (command line version) from the terminal using qmake:

Go to the folder 'gclc/source', then:

```
qmake cGCLC.pro
make
```

The executable (release version) 'gclc' will be created in the folder 'gclc/executable'.

### Building gclc-gui (version with the graphical user interface) from the terminal

Go to the folder 'gclc/source', then:

```
qmake gGCLC.pro
make
```

The executable (release version) 'gclc-gui' will be created in the folder 'gclc/executable'.

(For debug version: `qmake gGCLC.pro CONFIG+=debug CONFIG+=qml_debug`)

### Building gclc (command line version) from Qt Creator

Load the project cGCLC.pro from the folder 'gclc/source', 
then run *Build/Run qmake*, then run *Build/Build project cGCLC*.
Within the Qt Creator choose Release or Debug version.

The executable 'gclc' will be created in the folder 'gclc/executable'. 

### Building gclc-gui (version with the graphical user interface) from Qt Creator

Load the project gGCLC.pro from the folder 'gclc/source', 
then run *Build/Run qmake*, then run *Build/Build project gGCLC*.
Within the Qt Creator choose Release or Debug version.

The executable (release version) 'gclc-gui' will be created in the folder 'gclc/executable'. 

### Multiple makefiles

Note that all of the above `qmake` invocation produce files named
Makefile in the same directory ('gclc/source'). Each invocation of 
`qmake` will overwrite the existing Makefile, so if one 'Makefile' 
is to be used afterwards, it should be renamed, for instance to 
'Makefile-gui-release', and then make could be invoked as follows:

```
make -f Makefile-gui-release
```

If you build different makefiles, then after using qmake, use `make clean` before `make`.

## License

This software is licensed under the [MIT License](LICENSE.md).
