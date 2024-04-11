# Set CMake Version
cmake_minimum_required(VERSION 3.2.0)

#include("tools/cmake_tools/munkei-cmake/Munkei.cmake" )
#include( MunkeiVersionFromGit )

#version_from_git(LOG ON TIMESTAMP "%Y%m%d%H%M%S")

# Set Project Name and Version from Github
#project(L-PLC_IDE VERSION ${VERSION})
project(L-PLC_IDE VERSION 1.0)

#Write version and rc Files for PLCCOMM
#include("add_version.cmake" )

#enable debuf mode
set(CMAKE_BUILD_TYPE Release)

#set ToolChain
#set (CMAKE_VS_PLATFORM_TOOLSET "Visual Studio 2017 - Windows XP (v160_xp)") #Maybe change to V140_XP

#Set c++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#note: nEED THIS COMPILE FLAGS RTO TO RUN uuid_v4.h
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif ()


IF(MSVC) 
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd") 
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT") 
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd") 
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT") 
    # Remove .exe.manifest File
    set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
endif()


#Remove MSC Security Warnings
IF(MSVC) 
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

#add_definitions(-D_ALLOW_KEYWORD_MACROS)

#Remove Default Library
#SET(CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB:\"MSVCRT.lib\" ")

# we need threads
#if(UNIX)
#  find_package(Threads REQUIRED)
#endif()

#Change Executable Path
IF(MSVC) 
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/Win32")
Else()
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/unix")
Endif()


#Set Compilers Flags for Unix systems
#Set Compilers Flags for Unix systems
if(UNIX)
  set(CMAKE_C_FLAGS "-std=c11 -D_POSIX_C_SOURCE=200809L -lm -ldl -D_GNU_SOURCE")
endif()

#VC Headers
#if(MSVC)
#	include_directories("C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC\\include")
#endif()

#Remove Default Library
#SET(CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB: \" MSVCRT.lib\" ")

#Add Include Directoies
include_directories ("${PROJECT_SOURCE_DIR}/lib")
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty")
if(MSVC)
	include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/dirent")
endif()
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/freetype")
if(MSVC)
	include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/GL/win32")
else()
	include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/GL/unix")
endif()
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/glfw")
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/imgui")
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/imgui/misc")
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/imgui/misc/freetype")
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/JSON")
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/SHA2")
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/sqlite")
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/minizip")
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/whereami")
include_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/uuid_v4")
include_directories ("${PROJECT_SOURCE_DIR}/assets/Images")
include_directories ("${PROJECT_SOURCE_DIR}/assets/Fonts")
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/src/Core")
include_directories ("${PROJECT_SOURCE_DIR}/src/Editors")
include_directories ("${PROJECT_SOURCE_DIR}/src/Widgets")
include_directories ("${PROJECT_SOURCE_DIR}/src/Windows")

#Library Directories
link_directories("${PROJECT_SOURCE_DIR}/lib/ThirdParty/glfw")
link_directories("${PROJECT_SOURCE_DIR}/lib/ThirdParty/freetype")
link_directories ("${PROJECT_SOURCE_DIR}/lib/ThirdParty/minizip")

#Get all Files to be Compiled
aux_source_directory("${PROJECT_SOURCE_DIR}/lib"  SRC_INTERNAL_LIBS)
if(MSVC)
	aux_source_directory("${PROJECT_SOURCE_DIR}/lib/ThirdParty/GL/win32"  SRC_LIB_GL)
else()
	aux_source_directory("${PROJECT_SOURCE_DIR}/lib/ThirdParty/GL/unix"  SRC_LIB_GL)
endif()
aux_source_directory("${PROJECT_SOURCE_DIR}/lib/ThirdParty/imgui"  SRC_LIB_IMGUI)
aux_source_directory("${PROJECT_SOURCE_DIR}/lib/ThirdParty/imgui/misc"  SRC_LIB_IMGUI_MISC)
aux_source_directory("${PROJECT_SOURCE_DIR}/lib/ThirdParty/imgui/misc/freetype" SRC_LIB_IMGUI_FREETYPE)
aux_source_directory("${PROJECT_SOURCE_DIR}/lib/ThirdParty/JSON" SRC_LIB_CJSON)
aux_source_directory("${PROJECT_SOURCE_DIR}/lib/ThirdParty/SHA2" SRC_LIB_SHA2)
aux_source_directory("${PROJECT_SOURCE_DIR}/lib/ThirdParty/sqlite" SRC_LIB_SQLITE3)
LIST(REMOVE_ITEM SRC_LIB_SQLITE3 "${PROJECT_SOURCE_DIR}/lib/ThirdParty/sqlite/shell.c")
aux_source_directory("${PROJECT_SOURCE_DIR}/lib/ThirdParty/minizip" SRC_LIB_MINIZIP)
aux_source_directory("${PROJECT_SOURCE_DIR}/lib/ThirdParty/whereami" SRC_LIB_WHEAMI)
aux_source_directory("${PROJECT_SOURCE_DIR}/src" SRC_MAIN)
aux_source_directory("${PROJECT_SOURCE_DIR}/src/Core" SRC_CORE)
aux_source_directory("${PROJECT_SOURCE_DIR}/src/Editors" SRC_EDITORS)
aux_source_directory("${PROJECT_SOURCE_DIR}/src/Widgets" SRC_WIDGETS)
aux_source_directory("${PROJECT_SOURCE_DIR}/src/Windows" SRC_WINDOWS)


#Set Resources Files
set (RESORUCE_UI "${PROJECT_SOURCE_DIR}/src/RESOURCES.rc" "${PROJECT_SOURCE_DIR}/assets/Images/LeiseSymbolBlue.ico")

#Set Flags
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

#finput-charset=UTF-8 -fextended-identifiers
#Inform MVC We Use utf-8
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# User Interface 
#***********************************************************************
if(MSVC)
 
	add_executable(L-PLC_IDE	WIN32
				${SRC_MAIN}
				${SRC_CORE}
				${SRC_INTERNAL_LIBS} 
				${SRC_LIB_GL} 
				${SRC_LIB_IMGUI}
				${SRC_LIB_IMGUI_MISC}
				${SRC_LIB_IMGUI_FREETYPE} 
				${SRC_LIB_CJSON}
				${SRC_LIB_SHA2}
				${SRC_LIB_SQLITE3} 
				${SRC_LIB_WHEAMI}
				${SRC_EDITORS}
				${SRC_WIDGETS}
				${SRC_WINDOWS}
				${RESORUCE_UI}
	)

	target_link_libraries(L-PLC_IDE glfw3 opengl32 gdi32 kernel32 user32 freetype msvcrt msvcmrt)


else()
add_executable(L-PLC_IDE 
				${SRC_MAIN}
				${SRC_CORE}
				${SRC_INTERNAL_LIBS} 
				${SRC_LIB_GL} 
				${SRC_LIB_IMGUI}
				${SRC_LIB_IMGUI_MISC}
				${SRC_LIB_IMGUI_FREETYPE} 
				${SRC_LIB_CJSON}
				${SRC_LIB_SHA2}
				${SRC_LIB_SQLITE3} 
				${SRC_LIB_WHEAMI}
				${SRC_EDITORS}
				${SRC_WIDGETS}
				${SRC_WINDOWS}
				${RESORUCE_UI}
			)
	target_link_libraries(L-PLC_IDE pthread dl m glfw3 GL X11 freetype)
    #link to Libraries
    #set_target_properties(L-PLC_IDE PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif()

#Copy files to Bin Directory
if(MSVC)
  add_custom_command(
        TARGET L-PLC_IDE POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/lib/ThirdParty/freetype/freetype.dll"
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/freetype.dll")
else()
#  add_custom_command(
#        TARGET LSPRING_UI POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        "${PROJECT_SOURCE_DIR}/lib/ThirdParty/freetype/freetype.so"
#		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/freetype.so")
endif()

