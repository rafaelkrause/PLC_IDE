/*=============================================================================
 * Copyright (c) 2018 rafaelkrause
 *
 * @Script: UI_Style.h
 * @Author: rafaelkrause
 * @Email: rafaelkrause@gmail.com
 * @Create At: XXXX
 * @Last Modified By: rafaelkrause
 * @Last Modified At: 
 * @Description: Use Interface Style and Resources Manager Class
 *============================================================================*/
#ifndef _UI_STYLE_H_
#define _UI_STYLE_H_
// =============================================================================
//                                  INCLUDES
// =============================================================================
#include "gl3w.h"    		// To prevent Error, must add gl3w.h before gl.h
#include "os_generic.h"
#include "imgui.h"
#include "GL/gl.h"
#include "UI_Icons.h"

namespace UI_Style
{
	// =============================================================================
	//                                  DEFINES/MACROS
	// =============================================================================
	#define UI_STYLE_FILE_PATH		"data\\uicss.data"
	//Set Standard Colors
	
	#define  UI_COLOR_TRANS			ImVec4(0.000F, 0.000F, 0.000F, 0.0F)	//RGB 0,0,0
	#define  UI_COLOR_BLACK			ImVec4(0.000F, 0.000F, 0.000F, 1.0F)	//RGB 0,0,0
	#define  UI_COLOR_LIGHT_BLACK	ImVec4(0.149F, 0.149F, 0.149F, 1.0F)	//RGB 38,38,38
	#define  UI_COLOR_LIGHT_BLACK_2	ImVec4(0.120F, 0.120F, 0.120F, 1.0F)	//RGB 38,38,38	
	#define  UI_COLOR_LIGHT_BLACK_TRANS	ImVec4(0.149F, 0.149F, 0.149F, 0.7F)	//RGB 38,38,38
	#define  UI_COLOR_WHITE			ImVec4(1.000F, 1.000F, 1.000F, 1.0F)	//RGB 255,255,255
	#define  UI_COLOR_WHITE_DISABLE	ImVec4(0.800F, 0.800F, 0.800F, 0.8F)	
	#define  UI_COLOR_OFF_WHITE		ImVec4(0.900F, 0.900F, 0.900F, 1.0F)	//RGB 250,250,250
	#define  UI_COLOR_GRAY			ImVec4(0.502F, 0.502F, 0.502F, 1.0F)	//RGB 128,128,128
	#define  UI_COLOR_DARK_GRAY		ImVec4(0.412F, 0.412F, 0.412F, 1.0F)	//RGB 105,105,105
	#define  UI_COLOR_LIGHT_GRAY	ImVec4(0.80F, 0.80F, 0.80F, 1.0F)	//RGB 220,220,220
	#define  UI_COLOR_LIGHT_GRAY_TRANS	ImVec4(0.80F, 0.80F, 0.80F, 0.5F)	//RGB 220,220,220
	#define  UI_COLOR_GREEN			ImVec4(0.00f, 0.43f, 0.24f, 1.00f)	//RGB 45,190,80
	#define  UI_COLOR_LIGHT_GREEN	ImVec4(0.565F, 0.933F, 0.565F, 1.0F)	//RGB 144,238,144
	#define  UI_COLOR_DARK_GREEN	ImVec4(0.000F, 0.392F, 0.000F, 1.0F)	//RGB 0,100,0
	#define  UI_COLOR_RED			ImVec4(1.000F, 0.000F, 0.000F, 1.0F)	//RGB 255,0,0
	#define  UI_COLOR_LIGHT_RED		ImVec4(0.941F, 0.502F, 0.502F, 1.0F)	//RGB 240,128,128 
	#define  UI_COLOR_DARK_RED		ImVec4(0.647F, 0.000F, 0.000F, 1.0F)	//RGB 165,0,0
	#define  UI_COLOR_YELLOW		ImVec4(1.000F, 1.000F, 0.000F, 1.0F)	//RGB 255,255,0 
	#define  UI_COLOR_LIGHT_YELLOW	ImVec4(1.000F, 1.000F, 0.600F, 1.0F)	//RGB 255,255,153
	#define  UI_COLOR_DARK_YELLOW	ImVec4(0.800F, 0.800F, 0.000F, 1.0F)	//RGB 204,204,0
	#define  UI_COLOR_BLUE			ImVec4(0.000F, 0.000F, 1.000F, 1.0F)	//RGB 0,0,255
	#define  UI_COLOR_LIGHT_BLUE	ImVec4(0.118F, 0.565F, 1.000F, 1.0F)	//RGB 30,144,255
	#define  UI_COLOR_DARK_BLUE		ImVec4(0.000F, 0.000F, 0.545F, 1.0F)	//RGB 0,0,139
	

	//Back Ground Colors
	#define  UI_COLOR_BG_LEVEL_0		ImVec4(0.03f, 0.03f, 0.03f, 1.00f) // Almost Black
	#define  UI_COLOR_BG_LEVEL_1		ImVec4(0.09f, 0.09f, 0.09f, 1.00f) // POPUP BG
	#define  UI_COLOR_BG_LEVEL_2		ImVec4(0.12f, 0.12f, 0.12f, 1.00f) // Work bg
	#define  UI_COLOR_BG_LEVEL_3		ImVec4(0.20f, 0.20f, 0.20f, 1.00f) // Window bg
	#define  UI_COLOR_BG_LEVEL_4		ImVec4(0.22f, 0.22f, 0.22f, 1.00f) // Side Menu bg
	#define  UI_COLOR_BG_LEVEL_5		ImVec4(0.24f, 0.24f, 0.24f, 1.00f) // Top Menu
	#define  UI_COLOR_BG_LEVEL_6		ImVec4(0.24f, 0.24f, 0.24f, 1.00f) // Widget BG

		//Text
		//------------
	#define  UI_COLOR_TEXT_COLOR		ImVec4(1.0f, 1.0f, 1.0f, 1.00f)
	#define  UI_COLOR_TEXT_COLOR_DISABLED	ImVec4(0.52f, 0.52f, 0.52f, 1.00f)


		//Alarm Banner/Table
		//-------------
	#define  UI_COLOR_BANNER_SUCESS_BG	ImVec4(0.00f, 0.74f, 0.55f, 1.00f)
	#define  UI_COLOR_BANNER_WARNING_BG	ImVec4(0.95f, 0.61f, 0.07f, 1.00f)
	#define  UI_COLOR_BANNER_ERROR_BG	ImVec4(0.91f, 0.30f, 0.24f, 1.00f)
	#define  UI_COLOR_BANNER_INFO_BG	ImVec4(0.00f, 0.48f, 0.80f, 1.00f)
	#define  UI_COLOR_BANNER_TEXT		ImVec4(1.0f, 1.0f, 1.0f, 1.00f)

	#define  UI_COLOR_BANNER_SUCESS_BLINK_BG	ImVec4(0.00f, 1.00f, 0.75f, 1.00f)
	#define  UI_COLOR_BANNER_WARNING_BLINK_BG	ImVec4(1.00f, 0.64f, 0.07f, 1.00f)
	#define  UI_COLOR_BANNER_ERROR_BLINK_BG		ImVec4(1.00f, 0.33f, 0.26f, 1.00f)
	#define  UI_COLOR_BANNER_INFO_BLINK_BG		ImVec4(0.00f, 0.60f, 1.00f, 1.00f)

	//Shade
	#define  UI_COLOR_LIGHT_BLACK_SHADE		ImVec4(0.03f, 0.03f, 0.03f, 0.2f)


#define UI_COLOR_TOOL_WINDOW_BG			ImVec4(0.145f, 0.145f, 0.145f, 1.0f)
#define UI_COLOR_EDITOR_WINDOW_BG		ImVec4(0.117f, 0.117f, 0.117f, 1.0f)
#define UI_COLOR_TOP_MENU_BG			ImVec4(0.196f, 0.196f, 0.196f, 1.0f)
#define UI_COLOR_WORKSPACE_BG			ImVec4(0.145f, 0.145f, 0.145f, 1.0f)
#define UI_COLOR_SPLITER				ImVec4(0.2,0.2,0.2,1.0)

#define UI_COLOR_FOLDER					ImVec4(1.0f, 0.84f ,0.4f, 1.0f) 
#define UI_COLOR_STRUCT					ImVec4(0.84f,0.67f,0.4f, 1.0f)


	// =============================================================================
	//                                  ENUMS
	// =============================================================================
	// Define Fonts Types
	enum FONT_TYPE { DEFAULT = 0, DEFAULT_BOLD, MEDIUM, MEDIUM_BOLD, BIG, BIG_BOLD, CONSOLE, SMALL, EXTRA_BIG };


	// =============================================================================
	//                                  STRUCTS
	// =============================================================================
	typedef struct _uires_css
	{
		//Standard Sizes
		//-----------------------------------
		
		//Fonts Sizes
		float	FontSmallSize;
		float	FontDefaultSize;
		float	FontMediumSize;
		float	FontBigSize;
		float	FontConsoleSize;
		float	FontExtraBigSize;

		//IMGUI
		float	WindowPadding_x;
		float	WindowPadding_y;
		float	WindowRouding;
		float	WindowBoarderSize;
		float	WindowMinSize_x;
		float	WindowMinSize_y;
		float	WindowTileAlign_x;
		float	WindowTileAlign_y;
	
		float	PopupRouding;
		float	PopupBoardSize;

		float	ChildRouding;
		float	ChildBorderSize;
		
		float	FrameBorderSize;
		float	FrameRounding;
		float	FramePadding_x;
		float	FramePadding_y;
		
		float	ScrollbarSize;
		float	ScrollbarRounding;

		float	ItemSpacing_x;
		float	ItemSpacing_y;
		float	ItemSpacingInner_x;
		float	ItemSpacingInner_y;
		float	IndentSpacing;

		bool	AntiAliasedFill;
		bool	AntiAliasedLines;
		float	CurveTessellationTol;
		float	GlobalAlpha;
		
		float	GrabMinSize;
		float	GrabRouding;

		float	ButtonTextAlign_x;
		float	ButtonTextAlign_y;
		
		float	SelectableTextAlign_x;
		float	SelectableTextAlign_y;

		float	TabRouding;

		float	TouchExtraPadding_x;
		float	TouchExtraPadding_y;
		

		//Application size
		float	BtDefaultSize_x;
		float	BtDefaultSize_y;
		float	BtMediumSize_x;
		float	BtMediumSize_y;
		float	BtBigSize_x;
		float	BtBigSize_y;
		float	BtKeyBoard_x;
		float	BtKeyBoard_y;

		float	AppHeaderHeight;
		float	AppLeftMenuLenght;
		float	AppRightMenuLenght;		//New
		float	AppBottonMenuHeight;	//New

		float	PopUpWindowSize_x;
		float	PopUpWindowSize_y;


		//Standard Colors
		//-------------------------------------------
		//ImGUI Colors
		ImVec4	ColorText;
		ImVec4	ColorTextDisabled;
		ImVec4	ColorTextSelectedBg;
		ImVec4	ColorWindowBg;
		ImVec4	ColorChildBg;
		ImVec4	ColorPopUpBg;
		ImVec4	ColorBorder;
		ImVec4	ColorBorderShadow;
		ImVec4	ColorFrameBg;
		ImVec4	ColorFrameBgHovered;
		ImVec4	ColorFrameBgActive;
		ImVec4	ColorTitleBg;			//Use Same for Collapsed and Actived;
		ImVec4	ColorMenuBarBg;
		ImVec4	ColorScrollBarBg;
		ImVec4	ColorScrollBarGrab;
		ImVec4	ColorScrollBarGrabHovered;
		ImVec4	ColorScrollBarGrbActive;
		ImVec4	ColorCheckMark;
		ImVec4	ColorSliderGrab;
		ImVec4	ColorSliderGrabActive;
		ImVec4	ColorButtonBg;
		ImVec4	ColorButtonBgHovered;
		ImVec4	ColorButtonBgActive;
		ImVec4	ColorHeader;
		ImVec4	ColorHeaderHovered;
		ImVec4	ColorHeaderActive;
		ImVec4	ColorSeparator;
		ImVec4	ColorSeparatorHovered;
		ImVec4	ColorSeparatorActive;
		ImVec4	ColorGrip;
		ImVec4	ColorGripHovered;
		ImVec4	ColorGripActive;
		ImVec4	ColorTab;
		ImVec4	ColorTabHovered;
		ImVec4	ColorTabActive;
		ImVec4	ColorTabUnfocused;
		ImVec4	ColorPlotLines;
		ImVec4	ColorPlotHovered;
		ImVec4	ColorPlotHistogram;
		ImVec4	ColorPlotHistogramHovered;
		ImVec4	ColorDragDropTarget;
		ImVec4	ColorNavHighLight;
		ImVec4	ColorWindowingHighLight;
		ImVec4	ColorWindowingDimBg;
		ImVec4	ColorModalDimBg;
		
		//Stard Application Colors		
		ImVec4	ColorTextSucess;
		ImVec4	ColorTextWarning;
		ImVec4	ColorTextDanger;
		ImVec4	ColorTextInfo;

		ImVec4	ColorPrimary;
		ImVec4	ColorSecondary;
		ImVec4	ColorSucess;
		ImVec4	ColorDanger;
		ImVec4	ColorWarning;
		ImVec4	ColorInfo;

		ImVec4	ColorAlertSucessBorder;
		ImVec4 ColorAlertSucessBg;
		ImVec4 ColorAlertSucessText;

		ImVec4 ColorAlertDangerBorder;
		ImVec4 ColorAlertDangerBg;
		ImVec4 ColorAlertDangerText;


		ImVec4 ColorAlertWarningBorder;
		ImVec4 ColorAlertWarningBg;
		ImVec4 ColorAlertWarningText;


		ImVec4 ColorAlertInfoBorder;
		ImVec4 ColorAlertInfoBg;
		ImVec4 ColorAlertInfoText;


		ImVec4 ColorAlarmErrorBg;
		ImVec4 ColorAlarmErrorText;
		ImVec4 ColorAlarmWarningBg;
		ImVec4 ColorAlarmWarningText;
		ImVec4 ColorAlarmInfoBg;
		ImVec4 ColorAlarmInfoText;

		//Aplication Configs
		bool	IsTouchEnable;
		float	ReloadInterval;

	}UI_Style_css_t;
	

	typedef struct _uires_ctx
	{
		UI_Style_css_t	style;

		ImVec2			BtDefaultSz;
		ImVec2			BtMediumSz;
		ImVec2			BtBigSz;
		ImVec2			BtKeyBoard;

		GLuint          Images[11];
		ImFont*			Fonts[10];
		char			AppPath[1024];
		FILE* 			plogFile;

	}ui_style_ctx_t;
	//extern ui_style_ctx_t g_ui_style_ctx;

	// =============================================================================
	//                      PUBLIC FUNCTIONS DECLARATIONS
	// =============================================================================
	
	void			Init();
	bool			Isinitialized();
	void			SaveCss();
	void			LoadCss();
	ui_style_ctx_t*	GetContext();
	UI_Style_css_t*	GetAppStyle();
	GLuint			GetImgFromAtlas(int id);
	void			SetFont(FONT_TYPE fonttype);

	
}

#endif