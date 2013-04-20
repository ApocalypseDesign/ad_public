# Microsoft Developer Studio Project File - Name="DEMO_SW" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DEMO_SW - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DEMO_SW.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DEMO_SW.mak" CFG="DEMO_SW - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DEMO_SW - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DEMO_SW - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DEMO_SW - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseSW"
# PROP Intermediate_Dir "ReleaseSW"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MD /W3 /Gi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 fmod.lib libpng.lib libjpeg.lib libsw_Release.lib comctl32.lib d3d8.lib dxguid.lib ddraw.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /machine:I386 /nodefaultlib:"libc"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DEMO_SW - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugSW"
# PROP Intermediate_Dir "DebugSW"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 fmod.lib libpng.lib libjpeg.lib libsw_Debug.lib comctl32.lib d3d8.lib dxguid.lib ddraw.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DEMO_SW - Win32 Release"
# Name "DEMO_SW - Win32 Debug"
# Begin Group "Demo System"

# PROP Default_Filter ""
# Begin Group "AD Render LIB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\LIB_aux\include\ADrender.h
# End Source File
# End Group
# Begin Group "Main Program No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ADdemo.cpp
# End Source File
# Begin Source File

SOURCE=.\ADmain.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ADdemo.rc
# End Source File
# End Group
# Begin Source File

SOURCE=.\dtimer.cpp
# End Source File
# Begin Source File

SOURCE=.\dtimer.h
# End Source File
# Begin Source File

SOURCE=.\efx2D.cpp
# End Source File
# Begin Source File

SOURCE=.\efx2D.h
# End Source File
# Begin Source File

SOURCE=.\utility.cpp
# End Source File
# Begin Source File

SOURCE=.\utility.h
# End Source File
# End Group
# Begin Group "Effetti 2D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Efx2D\blur2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\blur2D.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\bump2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\bump2D.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\fade2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\fade2D.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\flash2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\flash2D.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\lowpass2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\lowpass2D.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\noise.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\noise.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\painter2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\painter2D.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\plasm2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\plasm2D.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\postfilters2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\postfilters2D.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\quadro2d.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\quadro2d.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\spline.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\spline.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\sprite.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\TextScroller2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\TextScroller2D.h
# End Source File
# Begin Source File

SOURCE=.\Efx2D\tomato2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Efx2D\tomato2D.h
# End Source File
# End Group
# Begin Group "Effetti 3D"

# PROP Default_Filter ""
# End Group
# Begin Group "Engine3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engines\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\Camera.h
# End Source File
# Begin Source File

SOURCE=.\Engines\Keyframe.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\Keyframe.h
# End Source File
# Begin Source File

SOURCE=.\Engines\EngineSW\lights.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\lights.h
# End Source File
# Begin Source File

SOURCE=.\Engines\EngineSW\Object3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\object3D.h
# End Source File
# Begin Source File

SOURCE=.\Engines\OSMobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\OSMobj.h
# End Source File
# Begin Source File

SOURCE=.\Engines\EngineSW\particle.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\particle.h
# End Source File
# Begin Source File

SOURCE=.\Engines\EngineSW\patch.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\patch.h
# End Source File
# Begin Source File

SOURCE=.\Engines\EngineSW\Scene3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\Scene3d.h
# End Source File
# Begin Source File

SOURCE=.\Engines\EngineSW\triangle.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\triangle.h
# End Source File
# Begin Source File

SOURCE=.\Engines\UtilsA3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\UtilsA3D.h
# End Source File
# Begin Source File

SOURCE=.\Engines\utilsscript.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\utilsscript.h
# End Source File
# Begin Source File

SOURCE=.\Engines\WSMobj.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\WSMobj.h
# End Source File
# End Group
# Begin Group "Engine_Raytracing"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engines\Raytracing\rayt_kernel.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\Raytracing\rayt_kernel.h
# End Source File
# Begin Source File

SOURCE=.\Engines\Raytracing\rayt_material.h
# End Source File
# Begin Source File

SOURCE=.\Engines\Raytracing\rayt_object.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\Raytracing\rayt_object.h
# End Source File
# Begin Source File

SOURCE=.\Engines\Raytracing\rayt_scene.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\Raytracing\rayt_scene.h
# End Source File
# Begin Source File

SOURCE=.\Engines\Raytracing\rayt_utilsscript.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\Raytracing\rayt_utilsscript.h
# End Source File
# Begin Source File

SOURCE=.\Engines\Raytracing\utilsRAY.cpp
# End Source File
# Begin Source File

SOURCE=.\Engines\Raytracing\utilsRAY.h
# End Source File
# End Group
# Begin Group "Math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Math\fastmath.cpp
# End Source File
# Begin Source File

SOURCE=.\Math\fastmath.h
# End Source File
# Begin Source File

SOURCE=.\Math\matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Math\matrix.h
# End Source File
# Begin Source File

SOURCE=.\Math\quater.cpp
# End Source File
# Begin Source File

SOURCE=.\Math\quater.h
# End Source File
# Begin Source File

SOURCE=.\Math\vect3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Math\vect3d.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Target
# End Project
