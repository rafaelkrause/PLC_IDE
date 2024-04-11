/*=============================================================================
 * Copyright (c) 2022 - Leise Motion Controller
 *
 * @Script: UI_MainGL.cpp
 * @Author: rafaelkrause
 * @Email: rafael@leise.com.br
 * @Create At: 07/12/2022
 * @Last Modified By: rafaelkrause
 * @Last Modified At:
 * @Description: UI Loader
 *============================================================================*/
 // =============================================================================
 //                                  INCLUDES
 // =============================================================================
#include "gl3w.h"    // Initialize with gl3wInit()
#include <locale.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


//  STB_IMAGE.H INCLUDES
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>											//Used to Load Images to OpenGL Textures 
#endif // !STB_IMAGE_IMPLEMENTATION

namespace std
{
#include <cstdlib>
};



#include "ui_core.hpp"

//
// Aplication Entry
//------------------------------------------------------------------------------
#ifdef WIN32
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
#else
int main()
{
#endif

	//TRICK: Enable windows to use UTF8 charecters on paths. fopen etc...
	setlocale(LC_CTYPE, ".UTF8");

	if (!UI_Core::AppInit())
		return -1;
	
	UI_Core::AppLoop();
	
	return 1;
}



