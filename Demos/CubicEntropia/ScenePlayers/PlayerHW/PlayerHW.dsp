# Microsoft Developer Studio Project File - Name="PlayerHW" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PlayerHW - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PlayerHW.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PlayerHW.mak" CFG="PlayerHW - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PlayerHW - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PlayerHW - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PlayerHW - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 Bass.lib Strmiids.lib D3dx8.lib RenderLib_Release.lib libjpeg.lib libpng.lib fmod.lib d3d8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /machine:I386 /nodefaultlib:"libc" /out:"Release/PlayerHW_Release.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PlayerHW - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Bass.lib Strmiids.lib D3dx8.lib libpng.lib libjpeg.lib fmod.lib RenderLib_Debug.lib d3d8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /out:"Debug/PlayerHW_Debug.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PlayerHW - Win32 Release"
# Name "PlayerHW - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\ADdemo.cpp
# End Source File
# Begin Source File

SOURCE=..\ADmain.h
# End Source File
# Begin Source File

SOURCE=..\..\DemoSystem\cpu_detect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\DemoSystem\cpu_detect.h
# End Source File
# Begin Source File

SOURCE=..\..\DemoSystem\dtimer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\DemoSystem\dtimer.h
# End Source File
# Begin Source File

SOURCE=..\..\DemoSystem\efx2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\DemoSystem\efx2D.h
# End Source File
# Begin Source File

SOURCE=..\resource.h
# End Source File
# Begin Source File

SOURCE=..\resrc1.h
# End Source File
# Begin Source File

SOURCE=..\..\DemoSystem\utility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\DemoSystem\utility.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\ADdemo.rc
# End Source File
# Begin Source File

SOURCE=..\cursor1.cur
# End Source File
# Begin Source File

SOURCE=..\icon1.ico
# End Source File
# End Group
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\NewEngine\BoundPrimitives.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\BoundPrimitives.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\BoundVolumes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\BoundVolumes.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Camera.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\GeometricObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\GeometricObject.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Helpers.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Helpers.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Lights.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Lights.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Mesh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Mesh.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\OSM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\OSM.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\ParticleSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\ParticleSystem.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Scene3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Scene3d.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\Triangle.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\UtilsA3D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\UtilsA3D.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\UtilsA3D_LoadSpecific.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\UtilsScript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\UtilsScript.h
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\WSMObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\NewEngine\WSMObject.h
# End Source File
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

SOURCE=..\..\Math\Keyframe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Math\Keyframe.h
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
# Begin Group "Efx3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Efx3D\TexFont.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Efx3D\TexFont.h
# End Source File
# End Group
# Begin Group "Efx2D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Efx2D\fade2D.h
# End Source File
# Begin Source File

SOURCE=..\..\Efx2D\fade2D_hw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Efx2D\flash2D.h
# End Source File
# Begin Source File

SOURCE=..\..\Efx2D\Flash2D_hw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Efx2D\Sprite2D.h
# End Source File
# Begin Source File

SOURCE=..\..\Efx2D\Sprite2D_hw.cpp
# End Source File
# End Group
# End Target
# End Project
