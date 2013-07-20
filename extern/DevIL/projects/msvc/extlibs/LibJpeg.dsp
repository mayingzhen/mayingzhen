# Microsoft Developer Studio Project File - Name="libjpeg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libjpeg - Win32 Release_GraphicEngine
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LibJpeg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LibJpeg.mak" CFG="libjpeg - Win32 Release_GraphicEngine"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libjpeg - Win32 Release_GraphicEngine" (based on "Win32 (x86) Static Library")
!MESSAGE "libjpeg - Win32 Debug_GraphicEngine" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "libjpeg"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "$(ConfigurationName)"
# PROP BASE Intermediate_Dir "$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "LibJpeg_Release"
# PROP Intermediate_Dir "LibJpeg_Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /Fp".\libjpeg___Win32_Release/LibJpeg.pch" /YX /Fo".\libjpeg___Win32_Release/" /GF /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /Fp".\LibJpeg_Release/LibJpeg.pch" /YX /GF /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"$(ConfigurationName)\LibJpegMD.lib"
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "$(ConfigurationName)"
# PROP BASE Intermediate_Dir "$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LibJpeg_Debug"
# PROP Intermediate_Dir "LibJpeg_Debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /FR /Fp".\libjpeg_Debug/LibJpeg.pch" /YX /Fo".\libjpeg_Debug/" /Fd".\libjpeg_Debug/" /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /Fp".\libjpeg_Debug/LibJpeg.pch" /YX /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"$(ConfigurationName)\$(ProjectName)_DBG.lib"
# ADD LIB32 /nologo /out:"LibJpeg_Debug\libjepg.lib"

!ENDIF 

# Begin Target

# Name "libjpeg - Win32 Release_GraphicEngine"
# Name "libjpeg - Win32 Debug_GraphicEngine"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcapimin.c
DEP_CPP_JCAPI=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcapistd.c
DEP_CPP_JCAPIS=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jccoefct.c
DEP_CPP_JCCOE=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jccolor.c
DEP_CPP_JCCOL=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcdctmgr.c
DEP_CPP_JCDCT=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdct.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jchuff.c
DEP_CPP_JCHUF=\
	"..\..\..\extlibs\Libjpeg\jchuff.h"\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcinit.c
DEP_CPP_JCINI=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcmainct.c
DEP_CPP_JCMAI=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcmarker.c
DEP_CPP_JCMAR=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcmaster.c
DEP_CPP_JCMAS=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcomapi.c
DEP_CPP_JCOMA=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcparam.c
DEP_CPP_JCPAR=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcphuff.c
DEP_CPP_JCPHU=\
	"..\..\..\extlibs\Libjpeg\jchuff.h"\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcprepct.c
DEP_CPP_JCPRE=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jcsample.c
DEP_CPP_JCSAM=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jctrans.c
DEP_CPP_JCTRA=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdapimin.c
DEP_CPP_JDAPI=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdapistd.c
DEP_CPP_JDAPIS=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdatadst.c
DEP_CPP_JDATA=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdatasrc.c
DEP_CPP_JDATAS=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdcoefct.c
DEP_CPP_JDCOE=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdcolor.c
DEP_CPP_JDCOL=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jddctmgr.c
DEP_CPP_JDDCT=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdct.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdhuff.c
DEP_CPP_JDHUF=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdhuff.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdinput.c
DEP_CPP_JDINP=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdmainct.c
DEP_CPP_JDMAI=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdmarker.c
DEP_CPP_JDMAR=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdmaster.c
DEP_CPP_JDMAS=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdmerge.c
DEP_CPP_JDMER=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdphuff.c
DEP_CPP_JDPHU=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdhuff.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdpostct.c
DEP_CPP_JDPOS=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdsample.c
DEP_CPP_JDSAM=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdtrans.c
DEP_CPP_JDTRA=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jerror.c
DEP_CPP_JERRO=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	"..\..\..\extlibs\Libjpeg\jversion.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jfdctflt.c
DEP_CPP_JFDCT=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdct.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jfdctfst.c
DEP_CPP_JFDCTF=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdct.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jfdctint.c
DEP_CPP_JFDCTI=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdct.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jidctflt.c
DEP_CPP_JIDCT=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdct.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jidctfst.c
DEP_CPP_JIDCTF=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdct.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jidctint.c
DEP_CPP_JIDCTI=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdct.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jidctred.c
DEP_CPP_JIDCTR=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jdct.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jmemmgr.c
DEP_CPP_JMEMM=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmemsys.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jmemnobs.c
DEP_CPP_JMEMN=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmemsys.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jquant1.c
DEP_CPP_JQUAN=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jquant2.c
DEP_CPP_JQUANT=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jutils.c
DEP_CPP_JUTIL=\
	"..\..\..\extlibs\Libjpeg\jconfig.h"\
	"..\..\..\extlibs\Libjpeg\jerror.h"\
	"..\..\..\extlibs\Libjpeg\jinclude.h"\
	"..\..\..\extlibs\Libjpeg\jmorecfg.h"\
	"..\..\..\extlibs\Libjpeg\jpegint.h"\
	"..\..\..\extlibs\Libjpeg\jpeglib.h"\
	

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# ADD CPP /nologo /GX /O2

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# ADD CPP /nologo /GX /Od /FR /GZ

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jchuff.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jconfig.h

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jconfig.h
InputName=jconfig

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" ..\include

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jconfig.h
InputName=jconfig

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" ..\include

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdct.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jdhuff.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jerror.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jinclude.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jmemsys.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jmorecfg.h

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jmorecfg.h
InputName=jmorecfg

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" ..\include

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jmorecfg.h
InputName=jmorecfg

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" ..\include

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jpegint.h
# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jpeglib.h

!IF  "$(CFG)" == "libjpeg - Win32 Release_GraphicEngine"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jpeglib.h
InputName=jpeglib

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" ..\include

# End Custom Build

!ELSEIF  "$(CFG)" == "libjpeg - Win32 Debug_GraphicEngine"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\..\extlibs\Libjpeg\jpeglib.h
InputName=jpeglib

"..\include\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "$(InputPath)" ..\include

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\extlibs\Libjpeg\jversion.h
# End Source File
# End Group
# End Target
# End Project
