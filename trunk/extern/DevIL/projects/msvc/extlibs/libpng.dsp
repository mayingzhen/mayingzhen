# Microsoft Developer Studio Project File - Name="libpng" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libpng - Win32 Release_GraphicEngine
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libpng.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libpng.mak" CFG="libpng - Win32 Release_GraphicEngine"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libpng - Win32 Release_GraphicEngine" (based on "Win32 (x86) Static Library")
!MESSAGE "libpng - Win32 Debug_GraphicEngine" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "$(ConfigurationName)"
# PROP BASE Intermediate_Dir "$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Libpng_Release"
# PROP Intermediate_Dir "Libpng_Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\..\extlibs\zlib" /D "WIN32" /D "NDEBUG" /D "_LIB" /Fp".\libpng___Win32_Release/libpng.pch" /Yu"png.h" /Fo".\libpng___Win32_Release/" /GF /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\..\extlibs\zlib" /D "WIN32" /D "NDEBUG" /D "_LIB" /GF /c
# ADD BASE RSC /l 0x409 /i "..\.." /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\.." /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"$(ConfigurationName)\LibpngMD.lib"
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "$(ConfigurationName)"
# PROP BASE Intermediate_Dir "$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Libpng_Debug"
# PROP Intermediate_Dir "Libpng_Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\.." /I "..\..\..\extlibs\zlib" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "DEBUG" /D PNG_DEBUG=1 /Fp".\libpng___Win32_Debug/libpng.pch" /Yu"png.h" /Fo".\libpng___Win32_Debug/" /Fd".\libpng___Win32_Debug/" /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\.." /I "..\..\..\extlibs\zlib" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "DEBUG" /D PNG_DEBUG=1 /Fp".\Debug/libpng.pch" /Yu"png.h" /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"$(ConfigurationName)\$(ProjectName)_DBG.lib"
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libpng - Win32 Release_GraphicEngine"
# Name "libpng - Win32 Debug_GraphicEngine"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\png.c
DEP_CPP_PNG_C=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNG_C=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\projects\msvc\png32ms.def
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngerror.c
DEP_CPP_PNGER=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGER=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngget.c
DEP_CPP_PNGGE=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGGE=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngmem.c
DEP_CPP_PNGME=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGME=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngpread.c
DEP_CPP_PNGPR=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGPR=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngread.c
DEP_CPP_PNGRE=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGRE=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngrio.c
DEP_CPP_PNGRI=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGRI=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngrtran.c
DEP_CPP_PNGRT=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGRT=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngrutil.c
DEP_CPP_PNGRU=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGRU=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngset.c
DEP_CPP_PNGSE=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGSE=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngtrans.c
DEP_CPP_PNGTR=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGTR=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngvcrd.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngwio.c
DEP_CPP_PNGWI=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGWI=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngwrite.c
DEP_CPP_PNGWR=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGWR=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngwtran.c
DEP_CPP_PNGWT=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGWT=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngwutil.c
DEP_CPP_PNGWU=\
	"..\..\..\extlibs\LibPNG\png.h"\
	"..\..\..\extlibs\LibPNG\pngconf.h"\
	"..\..\..\extlibs\zlib\zconf.h"\
	"..\..\..\extlibs\zlib\zlib.h"\
	
NODEP_CPP_PNGWU=\
	"..\..\..\extlibs\LibPNG\pngusr.h"\
	

!IF  "$(CFG)" == "libpng - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /GZ
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\png.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\LibPNG\pngconf.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=readme.txt
# PROP Exclude_From_Build 1
# End Source File
# End Target
# End Project
