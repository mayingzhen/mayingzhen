# Microsoft Developer Studio Project File - Name="zlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=zlib - Win32 Debug_GraphicEngine
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zlib.mak" CFG="zlib - Win32 Debug_GraphicEngine"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zlib - Win32 Release_GraphicEngine" (based on "Win32 (x86) Static Library")
!MESSAGE "zlib - Win32 Debug_GraphicEngine" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zlib - Win32 Release_GraphicEngine"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "$(ConfigurationName)"
# PROP BASE Intermediate_Dir "$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Zlib_Release"
# PROP Intermediate_Dir "Zlib_Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /Fo".\zlib_Release/" /GF /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /Fo".\zlib_Release/" /GF /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"$(ConfigurationName)\ZlibMD.lib"
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "zlib - Win32 Debug_GraphicEngine"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "$(ConfigurationName)"
# PROP BASE Intermediate_Dir "$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Zlib_Debug"
# PROP Intermediate_Dir "Zlib_Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /Fo".\zlib___Win32_Debug/" /Fd".\zlib___Win32_Debug/" /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /Fo".\zlib_Debug/" /Fd".\zlib_Debug/" /GZ /c
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

# Name "zlib - Win32 Release_GraphicEngine"
# Name "zlib - Win32 Debug_GraphicEngine"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\gzio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\infback.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=zlib.def
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\msdos\zlib.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\zutil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\crc32.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inflate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\zlib\zutil.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
