# GCLC

GCLC is a mathematical software for producing high-quality mathematical 
illustrations, for teaching mathematics, and for automated proving of 
geometry theorems. 

GCLC has been developed, with some idle years, since 1995. Its first 
release made public (web) was built in 1996. There were several substational 
code revisions. The original version was written in pure C, and over time it 
turned into C++ (while some C remains are still there). The current version 
with graphical user interface uses Qt libraries.

The application turned open-source in 2020, some 25 years after its very 
first version.

GCLC has a command-line version and a version with graphical user interface.

The source code and executables for Windows and Linux are available from 
the GCLC web page.

GCLC web page: 
    http://www.matf.bg.ac.rs/~janicic/gclc/


## Licence

This software is licensed under the MIT Licence. 


## Executables

Pre-built executables (both the command line version and the version
with graphical user interface, both for Windows and for Linux) are 
available in the folder "executable". Running these executables may
requires installing some libraries (for instance, by 
"sudo apt-get install qt5-default" on linux).

Executables can also be built from the available source code.


## Building from source code

GCLC is written in the C++ programming language and uses Qt libraries. 
GNU C++ g++ compiler is needed (or some other C++ compiler, but then
Makefile and .pro files should be updated). Building GCLC with the
graphical user interface requires Qt Library (https://www.qt.io/) 
version 5.10 or higher.

In the following, it is assumed that 'make' is available.
Under Windows, one can use 'mingw32-make' instead, for instance.


### Building gclc (command line version) from the terminal using the provided Makefile

Under Linux, go to the folder 'gclc', then:

> make 

Under Windows, go to the folder 'gclc', then:

> mingw32-make -f Makefile.win

The executable (release version) 'gclc' will be created in the folder gclc/executable. 


### Building gclc (command line version) from the terminal using qmake:

Go to the folder 'gclc/source', then:

> qmake cGCLC.pro

> make

The executable (release version) 'gclc' will be created in the folder gclc/executable. 


### Building gclc-gui (version with the graphical user interface) from the terminal 

Go to the folder 'gclc/source', then:

> qmake gGCLC.pro 

> make

The executable (release version) 'gclc-gui' will be created in the folder gclc/executable. 

(For debug version:
qmake: qmake gGCLC.pro CONFIG+=debug CONFIG+=qml_debug )


### Building gclc (command line version) from Qt Creator

Load the project cGCLC.pro from the folder 'gclc/source', 
then run 'Build/Run qmake', then run 'Build/Build project cGCLC'.
Within the Qt Creator choose Release or Debug version.
The executable 'gclc' will be  created in the folder gclc/executable. 

The executable (release version) 'gclc' will be created in the folder gclc/executable. 


### Building gclc-gui (version with the graphical user interface) from Qt Creator

Load the project gGCLC.pro from the folder 'gclc/source', 
then run 'Build/Run qmake', then run 'Build/Build project gGCLC'.
Within the Qt Creator choose Release or Debug version.

The executable (release version) 'gclc-gui' will be created in the folder gclc/executable. 


### Multiple makefiles

Note that all of the above 'qmake' invokations produce files named
Makefile in the same directory ('gclc/source'). Each invokation of 
'qmake' will overwrite the existing Makefile, so if one 'Makefile' 
is to be used afterwards, it should be renamed, for instance to 
'Makefile-gui-release', and then make could be invoked as follows:

> make -f Makefile-gui-release

If you build different makefiles, then after using qmake, use 
'make clean' before 'make'.


## Input
 
GCLC accepts files in GCL language, but can also import JavaView 
files coherent logic form.


## Output

GCLC processes GCL files and can export generated illustrations in several 
formats: simple LaTeX, LaTeX TikZ, LaTeX PSTricks, EPS, XML (textual 
specification).

Proofs by the automated theorem provers are generated in LaTeX form.


## Usage 

`gclc filename -a|w|g -pic|tikz|pst|eps|svg|xml`

   filename                    the name of the input file 

   -a|w|g                      selects the theorem prover used: the one based 
                               on the area method (a), the one based on Wu's 
                               method (w), or the one based on the Groebner 
                               bases method (g)

   -pic|tikz|pst|eps|svg|xml   selects the format for the output: simple LaTeX 
                               format (pic), LaTeX PStrics format (pst), LaTeX 
                               TikZ format (tikz), EPS format (eps), SVG format 
                               (svg), XML textual specification (xml) 
                               (default value: -pic)





