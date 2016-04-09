# Microsoft Developer Studio Project File - Name="FileFinder_MSVC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FileFinder_MSVC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FileFinder_MSVC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FileFinder_MSVC.mak" CFG="FileFinder_MSVC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FileFinder_MSVC - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FileFinder_MSVC - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FileFinder_MSVC - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\Core\Includes" /I "..\..\..\File\Includes" /I "..\..\..\GUI\Includes" /I "..\..\..\Codecs\Includes" /I "..\..\..\..\Libs\regexpp2\include" /I "..\..\_base\Sources" /FI"..\..\..\Targets\Includes\TargetAPI_Win32.h" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "FileFinder_MSVC - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\Core\Includes" /I "..\..\..\File\Includes" /I "..\..\..\GUI\Includes" /I "..\..\..\Codecs\Includes" /I "..\..\..\..\Libs\regexpp2\include" /I "..\..\_base\Sources" /FI"..\..\..\Targets\Includes\TargetAPI_Win32_Debug.h" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FileFinder_MSVC - Win32 Release"
# Name "FileFinder_MSVC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\_base\Sources\CreateSkin.cpp
# End Source File
# Begin Source File

SOURCE=..\Sources\FileFinder.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\_base\Sources\CreateSkin.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\Resources\App.rc
# End Source File
# End Group
# Begin Group "Lib Files"

# PROP Default_Filter "lib"
# Begin Group "Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Core\MSVC_Proj\Debug\XSP_Core.lib

!IF  "$(CFG)" == "FileFinder_MSVC - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FileFinder_MSVC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\File\MSVC_Proj\Debug\XSP_File.lib

!IF  "$(CFG)" == "FileFinder_MSVC - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FileFinder_MSVC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Codecs\MSVC_Proj\Debug\XSP_Codecs.lib

!IF  "$(CFG)" == "FileFinder_MSVC - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FileFinder_MSVC - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\MSVC_Proj\Debug\XSP_GUI.lib

!IF  "$(CFG)" == "FileFinder_MSVC - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FileFinder_MSVC - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Release"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Core\MSVC_Proj\Release\XSP_Core.lib

!IF  "$(CFG)" == "FileFinder_MSVC - Win32 Release"

!ELSEIF  "$(CFG)" == "FileFinder_MSVC - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\File\MSVC_Proj\Release\XSP_File.lib

!IF  "$(CFG)" == "FileFinder_MSVC - Win32 Release"

!ELSEIF  "$(CFG)" == "FileFinder_MSVC - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\GUI\MSVC_Proj\Release\XSP_GUI.lib

!IF  "$(CFG)" == "FileFinder_MSVC - Win32 Release"

!ELSEIF  "$(CFG)" == "FileFinder_MSVC - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\Codecs\MSVC_Proj\Release\XSP_Codecs.lib

!IF  "$(CFG)" == "FileFinder_MSVC - Win32 Release"

!ELSEIF  "$(CFG)" == "FileFinder_MSVC - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\Libs\regexpp2\lib\vc6\Release\RegExpWin32.lib
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\..\Program Files\MSVC\VC98\Lib\SHELL32.LIB"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\..\Program Files\MSVC\VC98\Lib\ADVAPI32.LIB"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\..\Program Files\MSVC\VC98\Lib\MSIMG32.LIB"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\..\Program Files\MSVC\VC98\Lib\OLE32.LIB"
# End Source File
# End Group
# End Target
# End Project
