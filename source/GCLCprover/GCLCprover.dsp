# Microsoft Developer Studio Project File - Name="GCLCprover" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=GCLCprover - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GCLCprover.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GCLCprover.mak" CFG="GCLCprover - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GCLCprover - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "GCLCprover - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GCLCprover - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../bin/GCLCprover/ReleaseOutput"
# PROP Intermediate_Dir "../../bin/GCLCprover/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x81a /d "NDEBUG"
# ADD RSC /l 0x81a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "GCLCprover - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../bin/GCLCprover/DebugOutput"
# PROP Intermediate_Dir "../../bin/GCLCprover/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x81a /d "_DEBUG"
# ADD RSC /l 0x81a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GCLCprover - Win32 Release"
# Name "GCLCprover - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "AlgebraicMethods"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\Groebner.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\ITimeOut.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\Log.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\Object.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\ObjectBank.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\Polynomial.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\PolyReader.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\Power.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\Prover.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\Reduce.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\StringBuilder.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\Term.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\TermStorage.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\TermStorageAvl.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\TermStorageVector.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\UPolynomial.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\UPolynomialFraction.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\UTerm.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\Wu.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\xpolynomial.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgebraicMethods\XTerm.cpp
# End Source File
# End Group
# Begin Group "Utils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Engine\Utils\Timer.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\Utils\Utils.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgMethod.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AlgMethodReducible.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\AreaMethod.cpp
# End Source File
# Begin Source File

SOURCE=.\GCLCprover.cpp
# End Source File
# Begin Source File

SOURCE=.\GCLCprover.h
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\GroebnerMethod.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\ProverExpression.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\TheoremProver.cpp
# End Source File
# Begin Source File

SOURCE=..\Engine\TheoremProver\WuMethod.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
