/*=============================================================================
 * Copyright (c) 2018 rafaelkrause
 *
 * @Script: UI_Style.h
 * @Author: rafaelkrause
 * @Email: rafaelkrause@gmail.com
 * @Create At: XXXX
 * @Last Modified By: rafaelkrause
 * @Last Modified At: 
 * @Description: Use Interface Resources Manager Class
 *============================================================================*/

// =============================================================================
//                                  INCLUDES
// =============================================================================
#include "os_generic.h"
#ifdef WIN32
	#include "gl.h"
	#include "wglext.h"
	#include "gl3w.h"
#else
	#include "gl3w.h"
	//#include "gl.h"
	//#include <GL/glx.h>
	//#include "wglext.h"

#endif
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui/misc/freetype/imgui_freetype.h"
#include "../assets/Fonts/font_resources.h"						// Compressed Fonts (To Embedded fonts into Application)
#include "../assets/Images/image_resources.h"					// Embeded Images
#include "log.h"
#include "UI_Style.h"

//  STB_IMAGE.H INCLUDES
#include <stb_image.h>											//Used to Load Images to OpenGL Textures 										// Used to Load Images to OpenGL Textures 


//===================================================================================
// 									LOCAL CONSTANTS
//===================================================================================
#define UI_STYLE_LIB_NAME			"UI_STYLE"


//===================================================================================		
//  								GLOBAL VARIABLES
//===================================================================================
static UI_Style::ui_style_ctx_t	g_ui_style_ctx;
static bool						g_ui_style_IsInitialized;

// =============================================================================
//                             FOWARD DECLARATIONS
//              (avoid unecessary header declaration / Local functions)
// =============================================================================
static void		InitStyleData();
static void		LoadImages();
static void		LoadFonts();
static void		SetImguiConfigs();
static void		SetImguiColors();
static void		SetAppConfigs();
static void 	GetExePath();
static void		SetStandardStyle();
static GLuint	LoadImageAsTexture(const char* path);
static GLuint	LoadImageAsTextureFromMemory(const unsigned char* data, int data_len);

//===================================================================================
//									PUBLIC FUNCTIONS
//===================================================================================

void UI_Style::Init()
{
	g_ui_style_IsInitialized = false;
	InitStyleData();
}

bool UI_Style::Isinitialized()
{
	return g_ui_style_IsInitialized;
}

UI_Style::ui_style_ctx_t*	UI_Style::GetContext()
{
	return &g_ui_style_ctx; 
}

UI_Style::UI_Style_css_t*	UI_Style::GetAppStyle()
{
	return &g_ui_style_ctx.style;
}

GLuint UI_Style::GetImgFromAtlas(int id) 
{ 
	return g_ui_style_ctx.Images[id]; 
}

void UI_Style::SetFont(FONT_TYPE fonttype) 
{ 
	ImGui::SetCurrentFont(g_ui_style_ctx.Fonts[fonttype]); 
}


//===================================================================================
//								LOCAL FUNCTIONS
//===================================================================================
static void InitStyleData()
{
	GetExePath();
	SetStandardStyle();
	LoadImages();
	LoadFonts();
	SetImguiConfigs();
	SetImguiColors();
	SetAppConfigs();
	g_ui_style_IsInitialized = true;
	   	
	return;
}

static void LoadImages()
{
	g_ui_style_ctx.Images[0] = LoadImageAsTextureFromMemory(&leise_logo_data[0], leise_logo_data_size);
	g_ui_style_ctx.Images[1] = LoadImageAsTextureFromMemory(&leise_logo_black_data[0], leise_logo_black_data_size);

}

static void LoadFonts()
{

	ImGuiIO& io = ImGui::GetIO();
	UI_Style::UI_Style_css_t *ui_style = &g_ui_style_ctx.style;

	//Load App Fonts
	//-----------------------------------------------------------

	ImFontConfig config, config2;
	config.MergeMode = false;
	config.OversampleV = 3;
	config.OversampleH = 3;

	config2.MergeMode = true;
	config2.GlyphOffset = ImVec2(0, 3);
	config2.GlyphExtraSpacing = ImVec2(2, 0);
	config2.OversampleH = 3;
	config2.PixelSnapH = true;


	ImVector<ImWchar> ranges;
	ImFontGlyphRangesBuilder builder;
	
	ImFontGlyphRangesBuilder builder_codicon;
	//To decrease to amouth of RAN usage, We load only the icons used. 

	//Add All Icons Used
	builder.AddText(
					UI_ICON_MD_MY_LOCATION
					UI_ICON_MD_OPEN_WITH
					UI_ICON_MD_SEND
					UI_ICON_MD_SETTINGS
					UI_ICON_MD_DELETE
					UI_ICON_MD_ADD
					UI_ICON_MD_CLOSE
					UI_ICON_MD_SAVE
					UI_ICON_MD_WARNING
					UI_ICON_MD_ERROR
					UI_ICON_MD_DONE
					UI_ICON_MD_CHECK
					UI_ICON_MD_CANCEL
					UI_ICON_MD_CONTENT_COPY
					UI_ICON_MD_CONTENT_PASTE
					UI_ICON_MD_FOLDER
					UI_ICON_MD_INSERT_DRIVE_FILE
					UI_ICON_MD_CLEAR
					UI_ICON_MD_FOLDER_OPEN
					UI_ICON_MD_REMOVE
					UI_ICON_MD_REFRESH
					UI_ICON_MD_MORE_HORIZ
					UI_ICON_MD_EXPAND_LESS
					UI_ICON_MD_EXPAND_MORE
					UI_ICON_MD_CHEVRON_LEFT
					UI_ICON_MD_CHEVRON_RIGHT
					UI_ICON_MD_OPEN_IN_NEW
					UI_ICON_MD_CHECK_CIRCLE
					UI_ICON_MD_ERROR_OUTLINE 
					UI_ICON_MD_FILTER_NONE
					UI_ICON_MD_WEB_ASSET
					UI_ICON_MD_VIEW_ARRAY
					UI_ICON_MD_CONTENT_CUT
					UI_ICON_MD_SELECT_ALL
					UI_ICON_MD_SEARCH
					UI_ICON_MD_FIND_REPLACE
					UI_ICON_MD_ARROW_DOWNWARD
					UI_ICON_MD_ARROW_UPWARD
					UI_ICON_MD_CYCLE
					UI_ICON_MD_DEPLOYED_CODE
					UI_ICON_MD_TOKEN
					UI_ICON_MD_SETTINGS_APPLICATIONS
					UI_ICON_MD_SOURCE_NOTES

	);

	builder_codicon.AddText(
					UI_ICON_VS_CHROME_CLOSE
					UI_ICON_VS_CHROME_MAXIMIZE
					UI_ICON_VS_CHROME_MINIMIZE
					UI_ICON_VS_CHROME_RESTORE
					UI_ICON_VS_DEBUG_START
					UI_ICON_VS_SYMBOL_METHOD
					UI_ICON_VS_SYMBOL_ENUM
					UI_ICON_VS_SYMBOL_CLASS
					UI_ICON_VS_GLOBE
					UI_ICON_VS_SYMBOL_FILE
					UI_ICON_VS_FILE_SYMLINK_FILE
					UI_ICON_VS_GITHUB_ACTION
					UI_ICON_VS_DISCARD
					UI_ICON_VS_REDO
					UI_ICON_VS_FILE_CODE
					UI_ICON_VS_SYNC
					UI_ICON_VS_SYMBOL_MISC
					);


	ImVector<ImWchar> material_icons_ranges;
	builder.BuildRanges(&material_icons_ranges);

	ImVector<ImWchar> codicon_icons_ranges;
	builder_codicon.BuildRanges(&codicon_icons_ranges);

	io.Fonts->Clear();
	g_ui_style_ctx.Fonts[UI_Style::FONT_TYPE::SMALL] = io.Fonts->AddFontFromMemoryCompressedTTF(roboto_regular_compressed_data, roboto_regular_compressed_size, ui_style->FontSmallSize, &config);
	io.Fonts->AddFontFromMemoryCompressedTTF(material_icons_compressed_data, material_icons_compressed_size, ui_style->FontSmallSize + 2, &config2, material_icons_ranges.Data);
	io.Fonts->AddFontFromMemoryCompressedTTF(codicon_icon_compressed_data, codicon_icon_compressed_size, ui_style->FontSmallSize + 2, &config2, codicon_icons_ranges.Data);

	g_ui_style_ctx.Fonts[UI_Style::FONT_TYPE::DEFAULT] = io.Fonts->AddFontFromMemoryCompressedTTF(roboto_regular_compressed_data, roboto_regular_compressed_size, ui_style->FontDefaultSize, &config);
	io.Fonts->AddFontFromMemoryCompressedTTF(material_icons_compressed_data, material_icons_compressed_size, ui_style->FontDefaultSize + 4, &config2, material_icons_ranges.Data);
	io.Fonts->AddFontFromMemoryCompressedTTF(codicon_icon_compressed_data, codicon_icon_compressed_size, ui_style->FontDefaultSize + 4, &config2, codicon_icons_ranges.Data);
	
	config2.MergeMode = false;
	g_ui_style_ctx.Fonts[UI_Style::FONT_TYPE::MEDIUM] = io.Fonts->AddFontFromMemoryCompressedTTF(codicon_icon_compressed_data, codicon_icon_compressed_size, ui_style->FontMediumSize, &config2, codicon_icons_ranges.Data);
	//config2.MergeMode = true;
	//io.Fonts->AddFontFromMemoryCompressedTTF(material_icons_compressed_data, material_icons_compressed_size, ui_style->FontMediumSize, &config2, material_icons_ranges.Data);
	//io.Fonts->AddFontFromMemoryCompressedTTF(codicon_icon_compressed_data, codicon_icon_compressed_size, ui_style->FontMediumSize, &config2, codicon_icons_ranges.Data);

	//g_ui_style_ctx.Fonts[UI_Style::FONT_TYPE::BIG] = io.Fonts->AddFontFromMemoryCompressedTTF(ROBOTO_Medium_compressed_data, ROBOTO_Medium_compressed_size, ui_style->FontBigSize);
	//io.Fonts->AddFontFromMemoryCompressedTTF(material_icons_compressed_data, material_icons_compressed_size, ui_style->FontBigSize, &config2, material_icons_ranges.Data);

	//g_ui_style_ctx.Fonts[UI_Style::FONT_TYPE::EXTRA_BIG] = io.Fonts->AddFontFromMemoryCompressedTTF(ROBOTO_Medium_compressed_data, ROBOTO_Medium_compressed_size, ui_style->FontExtraBigSize, &config);
	//io.Fonts->AddFontFromMemoryCompressedTTF(material_icons_compressed_data, material_icons_compressed_size, ui_style->FontBigSize, &config2, material_icons_ranges.Data);

	g_ui_style_ctx.Fonts[UI_Style::FONT_TYPE::DEFAULT_BOLD] = io.Fonts->AddFontFromMemoryCompressedTTF(roboto_regular_compressed_data, roboto_regular_compressed_size, ui_style->FontDefaultSize, &config);
	io.Fonts->AddFontFromMemoryCompressedTTF(material_icons_compressed_data, material_icons_compressed_size, ui_style->FontDefaultSize, &config2, material_icons_ranges.Data);
	io.Fonts->AddFontFromMemoryCompressedTTF(codicon_icon_compressed_data, codicon_icon_compressed_size, ui_style->FontDefaultSize + 4, &config2, codicon_icons_ranges.Data);

	g_ui_style_ctx.Fonts[UI_Style::FONT_TYPE::MEDIUM_BOLD] = io.Fonts->AddFontFromMemoryCompressedTTF(roboto_regular_compressed_data, roboto_regular_compressed_size, ui_style->FontMediumSize, &config);
	io.Fonts->AddFontFromMemoryCompressedTTF(material_icons_compressed_data, material_icons_compressed_size, ui_style->FontMediumSize, &config2, material_icons_ranges.Data);
	io.Fonts->AddFontFromMemoryCompressedTTF(codicon_icon_compressed_data, codicon_icon_compressed_size, ui_style->FontDefaultSize + 4, &config2, codicon_icons_ranges.Data);

	//g_ui_style_ctx.Fonts[UI_Style::FONT_TYPE::BIG_BOLD] = io.Fonts->AddFontFromMemoryCompressedTTF(ROBOTO_BOLD_compressed_data, ROBOTO_BOLD_compressed_size, ui_style->FontBigSize, &config);
	//io.Fonts->AddFontFromMemoryCompressedTTF(material_icons_compressed_data, material_icons_compressed_size, ui_style->FontBigSize, &config2, material_icons_ranges.Data);


	g_ui_style_ctx.Fonts[UI_Style::FONT_TYPE::CONSOLE] = io.Fonts->AddFontFromMemoryCompressedTTF(robot_mono_reg_compressed_data, robot_mono_reg_compressed_size, ui_style->FontConsoleSize,&config);


	/*ImVector<ImWchar> font_big_range;
	builder.Clear();
	builder.AddText(u8"0123456789,.%%");
	builder.BuildRanges(&font_big_range);

	ImVector<ImWchar> font_big_range_md;
	ImFontGlyphRangesBuilder builder_big_md;
	builder_big_md.Clear();
	builder_big_md.AddText(UI_ICON_MD_CHECK);
	builder_big_md.BuildRanges(&font_big_range_md);

	g_ui_style_ctx.Fonts[UI_Style::FONT_TYPE::EXTRA_BIG] = io.Fonts->AddFontFromMemoryCompressedTTF(ROBOTO_Medium_compressed_data, ROBOTO_Medium_compressed_size, ui_style->FontExtraBigSize, &config, font_big_range.Data);
	io.Fonts->AddFontFromMemoryCompressedTTF(material_icons_compressed_data, material_icons_compressed_size, ui_style->FontExtraBigSize, &config2, font_big_range_md.Data);*/


	ImGuiFreeType::BuildFontAtlas(io.Fonts, ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_Bitmap);

}

static void SetImguiConfigs()
{
	ImGuiStyle * style = &ImGui::GetStyle();
	UI_Style::UI_Style_css_t *ui_style = &g_ui_style_ctx.style;

	style->WindowPadding = ImVec2(ui_style->WindowPadding_x, ui_style->WindowPadding_y);
	style->WindowRounding = ui_style->WindowRouding;
	style->WindowBorderSize = ui_style->WindowBoarderSize;
	style->WindowMinSize = ImVec2(ui_style->WindowMinSize_x, ui_style->WindowMinSize_y);
	style->WindowTitleAlign = ImVec2(ui_style->WindowTileAlign_x, ui_style->WindowTileAlign_y);
	

	style->PopupRounding = ui_style->PopupRouding;
	style->PopupBorderSize = ui_style->PopupBoardSize;

	style->ChildRounding = ui_style->ChildRouding;
	style->ChildBorderSize = ui_style->ChildBorderSize;

	style->FrameBorderSize = ui_style->FrameBorderSize;
	style->FrameRounding = ui_style->FrameRounding;
	style->FramePadding = ImVec2(ui_style->FramePadding_x, ui_style->FramePadding_y);
	

	style->ScrollbarSize = ui_style->ScrollbarSize;
	style->ScrollbarRounding = ui_style->ScrollbarRounding;

	style->ItemSpacing = ImVec2(ui_style->ItemSpacing_x, ui_style->ItemSpacing_y);
	style->ItemInnerSpacing = ImVec2(ui_style->ItemSpacingInner_x, ui_style->ItemSpacingInner_y);
	style->IndentSpacing = ui_style->IndentSpacing;

	style->AntiAliasedFill = ui_style->AntiAliasedFill;
	style->AntiAliasedLines = ui_style->AntiAliasedLines;
	style->CurveTessellationTol = ui_style->CurveTessellationTol;
	style->Alpha = ui_style->GlobalAlpha;

	style->GrabMinSize = ui_style->GrabMinSize;
	style->GrabRounding = ui_style->GrabRouding;

	style->ButtonTextAlign = ImVec2(ui_style->ButtonTextAlign_x, ui_style->ButtonTextAlign_y);

	style->SelectableTextAlign = ImVec2(ui_style->SelectableTextAlign_x, ui_style->SelectableTextAlign_y);

	style->TabRounding = ui_style->TabRouding;

	style->TouchExtraPadding = ImVec2(ui_style->TouchExtraPadding_x, ui_style->TouchExtraPadding_y);


	

}

static void SetImguiColors()
{

	ImVec4* colors = ImGui::GetStyle().Colors;
	UI_Style::UI_Style_css_t *ui_style = &g_ui_style_ctx.style;


	colors[ImGuiCol_Text]				= ui_style->ColorText;
	colors[ImGuiCol_TextDisabled]		= ui_style->ColorTextDisabled;
	colors[ImGuiCol_WindowBg]			= ui_style->ColorWindowBg;
	colors[ImGuiCol_ChildBg]			= ui_style->ColorChildBg;
	colors[ImGuiCol_PopupBg]			= ui_style->ColorPopUpBg;
	colors[ImGuiCol_Border]				= ui_style->ColorBorder;
	colors[ImGuiCol_BorderShadow]		= ui_style->ColorBorderShadow;
	colors[ImGuiCol_FrameBg]			= ui_style->ColorFrameBg;
	colors[ImGuiCol_FrameBgHovered]		= ui_style->ColorFrameBgHovered;
	colors[ImGuiCol_FrameBgActive]		= ui_style->ColorFrameBgActive;
	colors[ImGuiCol_TitleBg]			= ui_style->ColorTitleBg;
	colors[ImGuiCol_TitleBgActive]		= ui_style->ColorTitleBg;
	colors[ImGuiCol_TitleBgCollapsed]	= ui_style->ColorTitleBg;
	colors[ImGuiCol_MenuBarBg]			= ui_style->ColorMenuBarBg;
	colors[ImGuiCol_ScrollbarBg]		= ui_style->ColorScrollBarBg;
	colors[ImGuiCol_ScrollbarGrab]		= ui_style->ColorScrollBarGrab;
	colors[ImGuiCol_ScrollbarGrabHovered] = ui_style->ColorScrollBarGrabHovered;
	colors[ImGuiCol_ScrollbarGrabActive] = ui_style->ColorScrollBarGrbActive;
	colors[ImGuiCol_CheckMark]			= ui_style->ColorCheckMark;
	colors[ImGuiCol_SliderGrab]			= ui_style->ColorSliderGrab;
	colors[ImGuiCol_SliderGrabActive]	= ui_style->ColorSliderGrabActive;
	colors[ImGuiCol_Button]				= ui_style->ColorButtonBg;
	colors[ImGuiCol_ButtonHovered]		= ui_style->ColorButtonBgActive;
	colors[ImGuiCol_ButtonActive]		= ui_style->ColorButtonBgHovered;
	colors[ImGuiCol_Header]				= ui_style->ColorHeader;
	colors[ImGuiCol_HeaderHovered]		= ui_style->ColorHeaderHovered;
	colors[ImGuiCol_HeaderActive]		= ui_style->ColorHeaderActive;
	colors[ImGuiCol_Separator]			= ui_style->ColorSeparator;
	colors[ImGuiCol_SeparatorHovered]	= ui_style->ColorSeparatorHovered;
	colors[ImGuiCol_SeparatorActive]	= ui_style->ColorSeparatorActive;
	colors[ImGuiCol_ResizeGrip]			= ui_style->ColorGrip;
	colors[ImGuiCol_ResizeGripHovered]	= ui_style->ColorGripHovered;
	colors[ImGuiCol_ResizeGripActive]	= ui_style->ColorGripActive;
	colors[ImGuiCol_Tab]				= ui_style->ColorTab;
	colors[ImGuiCol_TabHovered]			= ui_style->ColorTabHovered;
	colors[ImGuiCol_TabActive]			= ui_style->ColorTabActive;
	colors[ImGuiCol_TabUnfocused]		= ui_style->ColorTabUnfocused;
	colors[ImGuiCol_TabUnfocusedActive] = ui_style->ColorTabUnfocused;
	colors[ImGuiCol_PlotLines]			= ui_style->ColorPlotLines;
	colors[ImGuiCol_PlotLinesHovered]	= ui_style->ColorPlotHovered;
	colors[ImGuiCol_PlotHistogram]		= ui_style->ColorPlotHistogram;
	colors[ImGuiCol_PlotHistogramHovered] = ui_style->ColorPlotHistogramHovered;
	colors[ImGuiCol_TextSelectedBg]		= ui_style->ColorTextSelectedBg;
	colors[ImGuiCol_DragDropTarget]		= ui_style->ColorDragDropTarget;
	colors[ImGuiCol_NavHighlight]		= ui_style->ColorNavHighLight;
	colors[ImGuiCol_NavWindowingHighlight] = ui_style->ColorWindowingHighLight;
	colors[ImGuiCol_NavWindowingDimBg]	= ui_style->ColorWindowingDimBg;
	colors[ImGuiCol_ModalWindowDimBg]	= ui_style->ColorModalDimBg;
	colors[ImGuiCol_MenuBarBg]			= ui_style->ColorMenuBarBg;
	colors[ImGuiCol_ScrollbarBg]		= ui_style->ColorScrollBarBg;
	colors[ImGuiCol_ScrollbarGrab]		= ui_style->ColorScrollBarGrab;
	colors[ImGuiCol_ScrollbarGrabHovered] = ui_style->ColorScrollBarGrabHovered;
	colors[ImGuiCol_ScrollbarGrabActive] = ui_style->ColorScrollBarGrbActive;
	colors[ImGuiCol_SliderGrab]			= ui_style->ColorSliderGrab;
	colors[ImGuiCol_SliderGrabActive]	= ui_style->ColorSliderGrabActive;
	colors[ImGuiCol_Header]				= ui_style->ColorHeader;
	colors[ImGuiCol_HeaderHovered]		= ui_style->ColorHeaderHovered;
	colors[ImGuiCol_HeaderActive]		= ui_style->ColorHeaderActive;

}

static void SetAppConfigs()
{
	UI_Style::UI_Style_css_t *ui_style = &g_ui_style_ctx.style;

	//g_ui_style_ctx.BtSmallSz = ImVec2(ui_style->Bt);
	g_ui_style_ctx.BtDefaultSz = ImVec2(ui_style->BtDefaultSize_x, ui_style->BtDefaultSize_y);
	g_ui_style_ctx.BtMediumSz = ImVec2(ui_style->BtMediumSize_x, ui_style->BtMediumSize_y);
	g_ui_style_ctx.BtBigSz = ImVec2(ui_style->BtBigSize_x, ui_style->BtBigSize_y);
	g_ui_style_ctx.BtKeyBoard = ImVec2(ui_style->BtKeyBoard_x, ui_style->BtKeyBoard_y);

}

static void GetExePath()
{
#ifdef WIN32 	
	TCHAR szEXEPath[1024];
    GetModuleFileName( NULL, szEXEPath, 1024 );

    //Convert TCHAR to char
    char buf[1024]={0};
    if (sizeof(TCHAR)!=sizeof(CHAR))
         wctomb(&buf[0], szEXEPath[0]);            // Project has UNICODE settings
    else
        strcpy(buf,&szEXEPath[0]);   // Project has ANSI/MCBS settings

    //GetFile Path
    char *ptr = NULL;
    //find the last occurance of '\' to replace
    ptr = strrchr(buf, '\\');
    //continue to change only if there is a match found
    if (ptr)
       _snprintf(g_ui_style_ctx.AppPath, (ptr-buf)+1, "%s", buf);

	//Replace \ by /
	os_str_replace_all(&g_ui_style_ctx.AppPath[0], '\\', '/');
#else


#endif

}

static GLuint LoadImageAsTexture(const char* path)
{

	int w;
	int h;
	int comp;
	GLuint m_texture = 0;
	string_t tmp;

	sprintf(tmp.c, "%s/%s", g_ui_style_ctx.AppPath, path);

	unsigned char* image = stbi_load(tmp.c, &w, &h, &comp, 4);

	if (image == NULL){
		LOG_ERROR("UI_Style","Error to Load Image: %s (%s)", stbi_failure_reason(), tmp.c);
		return m_texture;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

#ifdef WIN32 
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //Thia is very important!!!!
#endif
	
	if (comp == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else if (comp == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);

	return m_texture;

}

static GLuint LoadImageAsTextureFromMemory(const unsigned char* data ,int data_len)
{
	int w;
	int h;
	int comp;
	GLuint m_texture = 0;

	unsigned char* image = stbi_load_from_memory(data, data_len, &w, &h, &comp, 4);


	if (image == NULL) {
		LOG_ERROR("UI_Style", "Error to Load Image: %s", stbi_failure_reason());
		return m_texture;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

#ifdef WIN32 
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //Thia is very important!!!!
#endif


	if (comp == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else if (comp == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);


	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);

	return m_texture;
}

static void SetStandardStyle()
{
	UI_Style::UI_Style_css_t *ui_style = &g_ui_style_ctx.style;

	//Fonts Sizes
	ui_style->FontSmallSize = 10;
	ui_style->FontDefaultSize = 13;
	ui_style->FontMediumSize = 16;
	ui_style->FontBigSize = 35;
	ui_style->FontConsoleSize = 13;
	ui_style->FontExtraBigSize = 120;

	//IMGUI
	ui_style->WindowPadding_x = 4;
	ui_style->WindowPadding_y = 4;
	ui_style->WindowRouding = 5;
	ui_style->WindowBoarderSize = 2;
	ui_style->WindowMinSize_x = 100;
	ui_style->WindowMinSize_y = 0;
	ui_style->WindowTileAlign_x = 0;
	ui_style->WindowTileAlign_y = 0.5;

	ui_style->PopupRouding = 5;
	ui_style->PopupBoardSize = 2;

	ui_style->ChildRouding = 0;
	ui_style->ChildBorderSize = 1;

	ui_style->FrameBorderSize = 0;
	ui_style->FrameRounding = 3;
	ui_style->FramePadding_x = 2;
	ui_style->FramePadding_y = 2;

	ui_style->ScrollbarSize = 20;
	ui_style->ScrollbarRounding = 10;

	ui_style->ItemSpacing_x = 2;
	ui_style->ItemSpacing_y = 2 ;
	ui_style->ItemSpacingInner_x = 2;
	ui_style->ItemSpacingInner_y = 2;
	ui_style->IndentSpacing = 2; // old 21

	ui_style->AntiAliasedFill = true;;
	ui_style->AntiAliasedLines = true;;
	ui_style->CurveTessellationTol = 0.1;
	ui_style->GlobalAlpha = 1;

	ui_style->GrabMinSize = 2;
	ui_style->GrabRouding = 3;

	ui_style->ButtonTextAlign_x = 0.5;
	ui_style->ButtonTextAlign_y = 0.5;

	ui_style->SelectableTextAlign_x = 0.0;
	ui_style->SelectableTextAlign_y = 0.5;

	ui_style->TabRouding = 0;

	ui_style->TouchExtraPadding_x = 2;
	ui_style->TouchExtraPadding_y = 2;


	//Application size
	ui_style->BtDefaultSize_x = 130;
	ui_style->BtDefaultSize_y = 45;
	ui_style->BtMediumSize_x = 170;
	ui_style->BtMediumSize_y = 55;
	ui_style->BtBigSize_x = 230;
	ui_style->BtBigSize_y = 80;
	ui_style->BtKeyBoard_x = 80;
	ui_style->BtKeyBoard_y = 80;

	ui_style->AppHeaderHeight = 60;
	ui_style->AppLeftMenuLenght = 60;
	ui_style->AppRightMenuLenght = 200;		//New
	ui_style->AppBottonMenuHeight = 60;	//New

	ui_style->PopUpWindowSize_x = 400;
	ui_style->PopUpWindowSize_y = 300;


	//Standard Colors
	//-------------------------------------------
	//ImGUI Colors
	ui_style->ColorText = UI_COLOR_TEXT_COLOR;
	ui_style->ColorTextDisabled = UI_COLOR_TEXT_COLOR_DISABLED;
	ui_style->ColorTextSelectedBg = ImVec4(1.00f, 1.00f, 1.00f, 0.18f);
	ui_style->ColorWindowBg = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	ui_style->ColorChildBg = UI_COLOR_BG_LEVEL_5;
	ui_style->ColorPopUpBg = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	ui_style->ColorBorder = ImVec4(0.23f, 0.23f, 0.23f, 0.78f);
	ui_style->ColorBorderShadow = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	ui_style->ColorFrameBg = ImVec4(0.43f, 0.43f, 0.43f, 0.50f);
	ui_style->ColorFrameBgHovered = ImVec4(0.43f, 0.43f, 0.43f, 0.80f);
	ui_style->ColorFrameBgActive = ImVec4(0.39f, 0.39f, 0.39f, 0.70f);
	ui_style->ColorTitleBg = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	ui_style->ColorMenuBarBg = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
	ui_style->ColorScrollBarBg = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
	ui_style->ColorScrollBarGrab = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	ui_style->ColorScrollBarGrabHovered = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	ui_style->ColorScrollBarGrbActive = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	ui_style->ColorCheckMark = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	ui_style->ColorSliderGrab = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
	ui_style->ColorSliderGrabActive = ImVec4(0.38f, 0.37f, 0.37f, 1.00f);
	ui_style->ColorButtonBg = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	ui_style->ColorButtonBgHovered = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	ui_style->ColorButtonBgActive = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
	ui_style->ColorHeader = ImVec4(0.40f, 0.40f, 0.40f, 0.45f);
	ui_style->ColorHeaderHovered = ImVec4(0.59f, 0.59f, 0.59f, 0.80f);
	ui_style->ColorHeaderActive = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	ui_style->ColorSeparator = UI_COLOR_SPLITER;
	ui_style->ColorSeparatorHovered = UI_COLOR_OFF_WHITE;
	ui_style->ColorSeparatorActive = UI_COLOR_LIGHT_BLUE;
	ui_style->ColorGrip = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
	ui_style->ColorGripHovered = ImVec4(0.78f, 0.82f, 1.00f, 0.60f);
	ui_style->ColorGripActive = ImVec4(0.78f, 0.82f, 1.00f, 0.90f);
	ui_style->ColorTab = ImVec4(0.34f, 0.34f, 0.68f, 0.79f);
	ui_style->ColorTabHovered = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
	ui_style->ColorTabActive = ImVec4(0.40f, 0.40f, 0.73f, 0.84f);
	ui_style->ColorTabUnfocused = ImVec4(0.28f, 0.28f, 0.57f, 0.82f);
	ui_style->ColorPlotLines = ImVec4(0.43f, 0.43f, 0.39f, 0.78f);
	ui_style->ColorPlotHovered = ImVec4(0.98f, 0.91f, 0.00f, 1.00f);
	ui_style->ColorPlotHistogram = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	ui_style->ColorPlotHistogramHovered = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	ui_style->ColorDragDropTarget = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	ui_style->ColorNavHighLight = ImVec4(0.97f, 0.97f, 0.97f, 0.80f);
	ui_style->ColorWindowingHighLight = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	ui_style->ColorWindowingDimBg = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	ui_style->ColorModalDimBg = ImVec4(0.63f, 0.63f, 0.63f, 0.35f);

	//Stard Application Colors		
	ui_style->ColorTextSucess = UI_COLOR_LIGHT_GREEN;
	ui_style->ColorTextWarning = UI_COLOR_YELLOW;
	ui_style->ColorTextDanger = UI_COLOR_LIGHT_RED;
	ui_style->ColorTextInfo = UI_COLOR_BLUE;

	ui_style->ColorPrimary = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	ui_style->ColorSecondary;
	ui_style->ColorSucess = ImVec4(0.57f, 1.00f, 0.50f, 0.50f);
	ui_style->ColorDanger = ImVec4(1.00f, 0.00f, 0.00f, 0.50f);
	ui_style->ColorWarning = ImVec4(1.00f, 1.00f, 0.00f, 0.50f);
	ui_style->ColorInfo = ImVec4(0.00f, 0.32f, 1.00f, 0.50f);

	//Alarms
	ui_style->ColorAlarmErrorBg = ImVec4(1.00f, 0.30f, 0.31f, 1.00f);
	ui_style->ColorAlarmErrorText = ImVec4(1.000F, 1.000F, 1.000F, 1.0F);

	ui_style->ColorAlarmWarningBg = ImVec4(0.98f, 0.68f, 0.08f, 1.00f);
	ui_style->ColorAlarmWarningText = ImVec4(1.000F, 1.000F, 1.000F, 1.0F);

	ui_style->ColorAlarmInfoBg = ImVec4(0.09f, 0.56f, 1.00f, 1.00f);
	ui_style->ColorAlarmInfoText = ImVec4(1.000F, 1.000F, 1.000F, 1.0F);


	//Alerts
	ui_style->ColorAlertSucessBorder = ImVec4(0.18f, 0.84f, 0.45f, 1.00f);
	ui_style->ColorAlertSucessBg = ImVec4(0.76f, 0.95f, 0.84f, 1.00f);
	ui_style->ColorAlertSucessText = ImVec4(0.14f, 0.68f, 0.36f, 1.00f);
	
	ui_style->ColorAlertDangerBorder = ImVec4(1.00f, 0.28f, 0.34f, 1.00f);
	ui_style->ColorAlertDangerBg = ImVec4(1.00f, 0.88f, 0.89f, 1.00f);
	ui_style->ColorAlertDangerText = ImVec4(1.00f, 0.28f, 0.34f, 1.00f);
	

	ui_style->ColorAlertWarningBorder = ImVec4(1.00f, 0.65f, 0.01f, 1.00f);
	ui_style->ColorAlertWarningBg = ImVec4(1.00f, 0.86f, 0.61f, 1.00f);
	ui_style->ColorAlertWarningText = ImVec4(1.00f, 1.00f, 0.00f, 0.50f);
	

	ui_style->ColorAlertInfoBorder = ImVec4(0.44f, 0.79f, 1.00f, 1.00f);
	ui_style->ColorAlertInfoBg = ImVec4(0.84f, 0.94f, 1.00f, 1.00f);
	ui_style->ColorAlertInfoText = ImVec4(0.24f, 0.71f, 1.00f, 1.00f);

		
	//Aplication Configs
	ui_style->IsTouchEnable = false;
	ui_style->ReloadInterval = 72000;

}


