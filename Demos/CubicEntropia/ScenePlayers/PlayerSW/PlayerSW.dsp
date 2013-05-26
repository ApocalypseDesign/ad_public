# Microsoft Developer Studio Project File - Name="PlayerSW" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PlayerSW - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PlayerSW.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PlayerSW.mak" CFG="PlayerSW - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PlayerSW - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PlayerSW - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PlayerSW - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 libjpeg.lib libpng.lib fmod.lib libsw_Release.lib d3d8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PlayerSW - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libjpeg.lib libpng.lib fmod.lib libsw_Debug.lib d3d8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PlayerSW - Win32 Release"
# Name "PlayerSW - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\ADdemo.cpp
# End Source File
# Begin Source File

SOURCE=..\ADmain.h
# End Source File
# Begin Source File

SOURCE=..\..\cpu_detect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cpu_detect.h
# End Source File
# Begin Source File

SOURCE=..\..\dtimer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\dtimer.h
# End Source File
# Begin Source File

SOURCE=..\..\efx2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\efx2D.h
# End Source File
# Begin Source File

SOURCE=..\resource.h
# End Source File
# Begin Source File

SOURCE=..\resrc1.h
# End Source File
# Begin Source File

SOURCE=..\..\utility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\utility.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\ADdemo.rc
# End Source File
# End Group
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Engines\Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\Camera.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\Keyframe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\Keyframe.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\lights.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\object3D.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\OSMobj.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\OSMobj.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\particle.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\patch.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\Scene3d.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\triangle.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\UtilsA3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\UtilsA3D.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\utilsscript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\utilsscript.h
# End Source File
# Begin Source File

SOURCE=..\..\Engines\WSMobj.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\WSMobj.h
# End Source File
# End Group
# Begin Group "SW"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Engines\EngineSW\lights.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\EngineSW\Object3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\EngineSW\particle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\EngineSW\patch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\EngineSW\Scene3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Engines\EngineSW\triangle.cpp
# End Source File
# End Group
# End Group
# Begin Group "Math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Math\fastmath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\fastmath.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\matrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\matrix.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\quater.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\quater.h
# End Source File
# Begin Source File

SOURCE=..\..\Math\vect3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\vect3d.h
# End Source File
# End Group
# End Target
# End Project
