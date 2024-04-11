include("tools/cmake_tools/munkei-cmake/Munkei.cmake" )
include( MunkeiVersionFromGit )

version_from_git(
  LOG       ON
  TIMESTAMP "%Y%m%d%H%M%S"
)

#Write Version file
file(WRITE ${PROJECT_SOURCE_DIR}/src/version.h
[[
#ifndef _VERISON_H_
#define _VERISON_H
]]
"\n#define PLCCOMM_VERSION \"${VERSION}\" \n"
"#endif")

#Write Resources file
IF(MSVC) 
file(WRITE "${PROJECT_SOURCE_DIR}/src/plccomm.rc" 
[[

// Microsoft Visual C++ generated resource script.
//
#include "resource.h"

#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#include "afxres.h"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
// English (United States) resources

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)
LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US
#pragma code_page(1252)

#ifdef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// TEXTINCLUDE
//

1 TEXTINCLUDE
BEGIN
"resource.h\0"
END

2 TEXTINCLUDE
BEGIN
"#include ""afxres.h""\r\n"
"\0"
END

3 TEXTINCLUDE
BEGIN
"\r\n"
"\0"
END

#endif    // APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
//
// Icon
//

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDI_TRAYICON            ICON                    "plccomm_icon.ico"


/////////////////////////////////////////////////////////////////////////////
//
// Version
//

VS_VERSION_INFO VERSIONINFO
FILEVERSION 1, 0, 0, 1
PRODUCTVERSION 1, 0, 0, 1
FILEFLAGSMASK 0x17L
#ifdef _DEBUG
FILEFLAGS 0x1L
#else
FILEFLAGS 0x0L
#endif
FILEOS 0x4L
FILETYPE 0x1L
FILESUBTYPE 0x0L
BEGIN
BLOCK "StringFileInfo"
BEGIN
BLOCK "040904b0"
BEGIN
VALUE "CompanyName", "LEISE Automação e Robótica"
VALUE "FileVersion", "1, 0, 0, 1"
VALUE "InternalName", "PLCCCOMM"
VALUE "LegalCopyright", "Copyright(C) 2013 - 2020 Leise"
VALUE "OriginalFilename", "PLCCOMM_SRV.exe"
VALUE "ProductName", "PLCCOMM Service"
VALUE "ProductVersion",]] "\"${VERSION} \" "  [[
END
END
BLOCK "VarFileInfo"
BEGIN
VALUE "Translation", 0x409, 1200
END
END

#endif    // English (United States) resources
/////////////////////////////////////////////////////////////////////////////

]])
ENDIF()
