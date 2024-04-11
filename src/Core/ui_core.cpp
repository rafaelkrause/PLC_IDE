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
#include "gl3w.h"    // Initialize with gl3wInit()
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "os_generic.h"
#include <time.h>

#include "log.h"
#include "UI_Style.h"
#include "cJSON.h"

#include "image_resources.h"
#include "stb_image.h"
#include "IecProject.hpp"
#include "ui_core.hpp"
#include "UI_Windows.h"


 //===================================================================================
// 									LOCAL CONSTANTS
//===================================================================================
#define UI_CORE_LIB_NAME			"UI_CORE"
#define UI_CORE_APP_STATE_PATH		"app.state"
#define UI_CORE_RECENT_PROJECT_MAX	10

//===================================================================================		
//  								LOCAL STRUCTS / ENUMS
//===================================================================================
struct AppConfiguration
{
	int RefreshRate;



	bool Save(const char* filepath)
	{


		return true;
	}

	bool Load(const char* filepath)
	{

		return true;
	}
};


struct AppState
{
	int			WindowSize[2];
	int			WindowPosition[2];
	bool		IsAppMaximized;
	bool		IsMovingWindow;
	int			Busy;
	int			ResizeWindowState;
	string_t	AppTitle;
	string_t	ProgramName;

	string_t	RecentProjetsPath[UI_CORE_RECENT_PROJECT_MAX];

	bool		IsSavePending;

	string_t	StatusBarMsg;
	float_t		StatusBarMsgStarTime;

	float_t		ProjectTreePanelSize;
	float_t		ConsolePanelSize;

	IecProject* Project;
 

	bool Save(const char* filepath)
	{
		string_t tmp;
		cJSON *appStateJSON = cJSON_CreateObject();

		char *result = NULL;

		if (appStateJSON == NULL)
			return false;

		cJSON_AddNumberToObject(appStateJSON, "WindowSize_x", WindowSize[0]);
		cJSON_AddNumberToObject(appStateJSON, "WindowSize_y", WindowSize[1]);
		cJSON_AddNumberToObject(appStateJSON, "WindowPosition_x", WindowPosition[0]);
		cJSON_AddNumberToObject(appStateJSON, "WindowPosition_y", WindowPosition[1]);
		cJSON_AddBoolToObject(appStateJSON, "IsAppMaximized", IsAppMaximized);

		cJSON_AddNumberToObject(appStateJSON, "ProjectTreePanelSize", ProjectTreePanelSize);
		cJSON_AddNumberToObject(appStateJSON, "ConsolePanelSize", ConsolePanelSize);
		
		cJSON_AddStringToObject(appStateJSON, "ProgramName", ProgramName.c);

		for (size_t i = 0; i < UI_CORE_RECENT_PROJECT_MAX; i++)
		{
			if (strlen(RecentProjetsPath[i].c) > 1)
			{
				sprintf(tmp.c, "RecentProjetsPath_%ld", i);
				cJSON_AddStringToObject(appStateJSON, tmp.c, RecentProjetsPath[i].c);
			}
		}

		result = cJSON_Print(appStateJSON);
		cJSON_Delete(appStateJSON);

		if (result)
		{
			if (os_SaveToFile(filepath, result, strlen(result)))
			{
				free(result);
				return true;
			}
			free(result);
		}

		return false;
	}
	
	bool Load(const char* filepath)
	{
		string_t tmp;

		//Get Data from file
		char *file_data = os_LoadFromFile(filepath, NULL);
		if ((file_data == NULL) || (file_data[0] == '\0') || (file_data[1] == '\0'))
		{
			LOG_ERROR(UI_CORE_LIB_NAME, "Cannot load data from %s", filepath);
			return false;
		}

		//Parse Json
		cJSON *cmdJSON = cJSON_Parse(file_data);
		if (cmdJSON == NULL)
		{
			const char *error_ptr = cJSON_GetErrorPtr();
			if (error_ptr != NULL)
			{
				LOG_ERROR(UI_CORE_LIB_NAME, "Error to Parse Program data before: %s", error_ptr);
				cJSON_Delete(cmdJSON);
				free(file_data);
			}

			return false;
		}

		cJSON* dataJSON = cJSON_GetObjectItem(cmdJSON, "WindowSize_x");
		if (dataJSON && cJSON_IsNumber(dataJSON))
			WindowSize[0] = dataJSON->valueint;
			
		dataJSON = cJSON_GetObjectItem(cmdJSON, "WindowSize_y");
		if (dataJSON && cJSON_IsNumber(dataJSON))
			WindowSize[1] = dataJSON->valueint;

		dataJSON = cJSON_GetObjectItem(cmdJSON, "WindowPosition_x");
		if (dataJSON && cJSON_IsNumber(dataJSON))
			WindowPosition[0] = dataJSON->valueint;
		
		dataJSON = cJSON_GetObjectItem(cmdJSON, "WindowPosition_y");
		if (dataJSON && cJSON_IsNumber(dataJSON))
			WindowPosition[1] = dataJSON->valueint;

		dataJSON = cJSON_GetObjectItem(cmdJSON, "IsAppMaximized");
		if (dataJSON && (cJSON_IsBool(dataJSON) || cJSON_IsNumber(dataJSON)))
			IsAppMaximized = (bool_t)dataJSON->valueint;
				
		dataJSON = cJSON_GetObjectItem(cmdJSON, "ProjectTreePanelSize");
		if (dataJSON && cJSON_IsNumber(dataJSON))
			ProjectTreePanelSize = dataJSON->valuedouble;

		dataJSON = cJSON_GetObjectItem(cmdJSON, "ConsolePanelSize");
		if (dataJSON && cJSON_IsNumber(dataJSON))
			ConsolePanelSize = dataJSON->valuedouble;



		dataJSON = cJSON_GetObjectItem(cmdJSON, "ProgramName");
		if (dataJSON && cJSON_IsString(dataJSON))
			strcpy(ProgramName.c, dataJSON->valuestring);

		
		for (size_t i = 0; i < UI_CORE_RECENT_PROJECT_MAX; i++)
		{
			if (strlen(RecentProjetsPath[i].c) > 1)
			{
				sprintf(tmp.c, "RecentProjetsPath_%ld", i);
				dataJSON = cJSON_GetObjectItem(cmdJSON, tmp.c);
				if (dataJSON && cJSON_IsString(dataJSON))
					strcpy(RecentProjetsPath[i].c, dataJSON->valuestring);
			}
		}


		cJSON_Delete(cmdJSON);
		free(file_data);

		return true;
	}
};

//===================================================================================		
//  								GLOBAL VARIABLES
//===================================================================================
static bool				g_AppShouldClose = false;
static GLFWwindow*		g_glfwindow = NULL;
static AppConfiguration g_appcfg;
static AppState			g_appstate;

// =============================================================================
//                             FOWARD DECLARATIONS
// =============================================================================

static void glfw_error_callback(int error, const char* description);

static void SetAppTitle();
static void HandleWindowResize();


//===================================================================================
//									PUBLIC FUNCTIONS
//===================================================================================

bool UI_Core::AppInit()
{

	//Init log server
	log_init("ui.log", 100);
	//os_sleep(30);

	//Load App Config
	g_appcfg.RefreshRate = 1;

	//Load App State
	if (!g_appstate.Load(UI_CORE_APP_STATE_PATH))
	{
		g_appstate.WindowSize[0] = 800;
		g_appstate.WindowSize[1] = 600;
		g_appstate.IsAppMaximized = true;
		g_appstate.ProjectTreePanelSize = 0.3;
		g_appstate.ConsolePanelSize = 0.3;

	}

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		LOG_ERROR("MAIN", "Error to Init Graphics Library");
		os_sleep(30);  //Give Some Time to Log Srv, Before Finish
		log_finish();
		return false;
	}

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	
	g_glfwindow = glfwCreateWindow(g_appstate.WindowSize[0], g_appstate.WindowSize[1], "LEISE - IDE", NULL, NULL);

	SetAppTitle();

	if (g_glfwindow == NULL)
	{
		LOG_ERROR("MAIN", "Error Create Window");
		os_sleep(30);  //Give Some Time to Log Srv, Before Finish
		log_finish();
		return false;
	}

	//Set Window Icon
	GLFWimage icons[1];
	icons[0].pixels = stbi_load_from_memory(&leise_logo_data[0], leise_logo_data_size, &icons[0].width, &icons[0].height, NULL, 4);
	glfwSetWindowIcon(g_glfwindow, 1, icons);

	free(icons[0].pixels);

	//Set Refresh Rate
	glfwMakeContextCurrent(g_glfwindow);
	glfwSwapInterval(g_appcfg.RefreshRate); // Enable vsync

	if (gl3wInit() != 0)
	{
		LOG_ERROR("MAIN", "Can't Initialize GL3W");
		os_sleep(30);  //Give Some Time to Log Srv, Before Finish
		log_finish();
		return false;
	}

	if (g_appstate.IsAppMaximized)
		glfwMaximizeWindow(g_glfwindow);
	else
		glfwSetWindowPos(g_glfwindow, g_appstate.WindowPosition[0], g_appstate.WindowPosition[1]);




	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.IniSavingRate = 60.0;					// Save Cavans to file, in Seconds
	io.IniFilename = NULL;						// Don't save Screen files
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		//Enable Docking

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(g_glfwindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//Init Application Graphic Resources
	UI_Style::Init();

//#define TOF( V ) ((V/255.0F))
//
//	LOG_ERROR("", "Text: #%08x", ImGui::ColorConvertFloat4ToU32(ImVec4(TOF(220), TOF(220), TOF(220), 1.f)));
//	LOG_ERROR("", "Keyword: #%08x", ImGui::ColorConvertFloat4ToU32(ImVec4(TOF(86), TOF(156), TOF(214), 1.f)));
//	LOG_ERROR("", "Comments: #%08x", ImGui::ColorConvertFloat4ToU32(ImVec4(TOF(87), TOF(166), TOF(74), 1.f)));
//	LOG_ERROR("", "Number: #%08x", ImGui::ColorConvertFloat4ToU32(ImVec4(TOF(184), TOF(215), TOF(163), 1.f)));
//	LOG_ERROR("", "Systemfun: #%08x", ImGui::ColorConvertFloat4ToU32(ImVec4(TOF(78), TOF(201), TOF(176), 1.f)));
//	LOG_ERROR("", "UserFun: #%08x", ImGui::ColorConvertFloat4ToU32(ImVec4(TOF(200), TOF(200), TOF(200), 1.f)));
//	LOG_ERROR("", "String: #%08x", ImGui::ColorConvertFloat4ToU32(ImVec4(TOF(214), TOF(157), TOF(133), 1.f)));
//	LOG_ERROR("", "Number: #%08x", ImGui::ColorConvertFloat4ToU32(ImVec4(TOF(181), TOF(206), TOF(168), 1.f)));
//
//
//#undef TOF(V)
//

	return true;
}

void UI_Core::AppLoop()
{
	ImVec4 clear_color = UI_COLOR_BG_LEVEL_0;

	// Main loop
	while (!g_AppShouldClose && !glfwWindowShouldClose(g_glfwindow))
	{

		//Got From: https://github.com/hanatos/vkdt/blob/master/src/gui/main.c#L71

		glfwPollEvents();

		// block and wait for one event instead of polling all the time to save on
		// gpu workload. might need an interrupt for "render finished" etc. we might
		// do that via glfwPostEmptyEvent()
		//if (g_appstate.Busy > 0) g_appstate.Busy--;
		//if (vkdt.state.anim_playing) // should redraw because animation is playing?
		//	vkdt.wstate.busy = vkdt.state.anim_max_frame == 0 ? 3 : vkdt.state.anim_max_frame - vkdt.state.anim_frame + 1;
		//if (g_appstate.Busy > 0) glfwPostEmptyEvent();
		//else g_appstate.Busy = 30;
		
		//glfwWaitEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		
		UI_Windows::MainWindow();

		//ImGui::ShowDemoWindow();

		// Rendering
		ImGui::Render();
		glfwGetFramebufferSize(g_glfwindow, &g_appstate.WindowSize[0], &g_appstate.WindowSize[1]);
		glViewport(0, 0, g_appstate.WindowSize[0], g_appstate.WindowSize[1]);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(g_glfwindow);
		HandleWindowResize();

	}

	// Cleanup
	g_appstate.Save(UI_CORE_APP_STATE_PATH);
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(g_glfwindow);
	glfwTerminate();




	log_finish();
}

bool UI_Core::GetIsMaximized()				
{ 
	return g_appstate.IsAppMaximized; 
}

void UI_Core::SetIsMaximized(bool state)	
{ 
	if (!g_glfwindow)
		return;

	g_appstate.IsAppMaximized = state;

	if (g_appstate.IsAppMaximized)
		glfwMaximizeWindow(g_glfwindow);
	else
		glfwRestoreWindow(g_glfwindow);

}

void UI_Core::MinimizeApplication()
{
	if (!g_glfwindow)
		return;

	glfwIconifyWindow(g_glfwindow);
}

void UI_Core::MoveWindow(bool isMouseDown)
{
	if (!g_glfwindow || g_appstate.ResizeWindowState != 0)
		return;

	static float mouseStartPos_x, mouseStartPos_y;
	static bool mouseDownEdge = false;

	double mouse_x, mouse_y;
	int window_x, window_y;

	if (!isMouseDown)
		mouseDownEdge = false;


	if (isMouseDown)
	{
		g_appstate.IsMovingWindow = true;

		glfwGetCursorPos(g_glfwindow, &mouse_x, &mouse_y);
		glfwGetWindowPos(g_glfwindow, &window_x, &window_y);

		if (!mouseDownEdge)
		{
			mouseDownEdge = true;
			mouseStartPos_x = mouse_x;
			mouseStartPos_y = mouse_y;

			if (UI_Core::GetIsMaximized())
				UI_Core::SetIsMaximized(false);

		}
		else
		{
			mouse_x += (window_x - mouseStartPos_x);
			mouse_y += (window_y - mouseStartPos_y);


			glfwSetWindowPos(g_glfwindow, mouse_x, mouse_y);
			g_appstate.WindowPosition[0] = mouse_x;
			g_appstate.WindowPosition[1] = mouse_y;
		}
	}
	else
	{
		g_appstate.IsMovingWindow = false;
	}
}

void UI_Core::CloseApplication()
{
	g_AppShouldClose = true;
}

void UI_Core::SetStatusMsg(const char* msg)
{
	memset(g_appstate.StatusBarMsg.c, 0, sizeof(g_appstate.StatusBarMsg.c));
	strncpy(g_appstate.StatusBarMsg.c,msg, STR_LEN - 1);

	g_appstate.StatusBarMsgStarTime = ImGui::GetTime();
}

const char* UI_Core::GetStatusMsg()
{
	if (g_appstate.StatusBarMsg.c[0] != 0 && ImGui::GetTime() < g_appstate.StatusBarMsgStarTime + 3)
		return &g_appstate.StatusBarMsg.c[0];
	else
		return " ";
	
}

const char* UI_Core::GetAppTitle()
{
	return &g_appstate.AppTitle.c[0];
}

float UI_Core::GetProjectTreeSize()
{
	return g_appstate.ProjectTreePanelSize;
}

void UI_Core::SetProjectTreeSize(float size)
{
	g_appstate.ProjectTreePanelSize = iec_max(0.01,size);
}

float UI_Core::GetConsoleSize()
{
	return g_appstate.ConsolePanelSize;
}

void UI_Core::SetConsoleSize(float size)
{
	g_appstate.ConsolePanelSize = iec_max(0.01, size);
}

void UI_Core::NewProject()
{
	g_appstate.Project = new IecProject();
}

IecProject* UI_Core::GetProject()
{
	return g_appstate.Project;
}

//===================================================================================
//								LOCAL FUNCTIONS
//===================================================================================

static void glfw_error_callback(int error, const char* description)
{
	LOG_ERROR("MAIN", "Glfw Error %d: %s", error, description);
}

static void SetAppTitle()
{
	if (!g_glfwindow)
		return;
	
	if (g_appstate.ProgramName.c[0] != '\0')
			sprintf(g_appstate.AppTitle.c, u8"Leise IDE - %s", g_appstate.ProgramName.c);
	else
		sprintf(g_appstate.AppTitle.c, u8"Leise IDE");

	glfwSetWindowTitle(g_glfwindow, g_appstate.AppTitle.c);

}

static void HandleWindowResize()
{
	static ImVec2 clickedPos;
	static int originalSize[2];
	static int originalPos[2];
	float dif = 0;
	double mouse_x, mouse_y;
	int window_x, window_y;

	if (g_appstate.IsMovingWindow || UI_Core::GetIsMaximized())
		return;

	ImVec2 guiMousePos = ImGui::GetIO().MousePos;
	ImVec2 guiWindowBorder = ImVec2(g_appstate.WindowSize[0] - 5, g_appstate.WindowSize[1] - 5);
	
	if (g_appstate.ResizeWindowState == 1 || g_appstate.ResizeWindowState == 3)
	{
		glfwGetCursorPos(g_glfwindow, &mouse_x, &mouse_y);
		glfwGetWindowPos(g_glfwindow, &window_x, &window_y);

		mouse_x += window_x;
		mouse_y += window_y;
	}

	switch (g_appstate.ResizeWindowState)
	{
	case 1:
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		glfwSetWindowSize(g_glfwindow, originalSize[0] + ((originalPos[0] + clickedPos.x) - mouse_x), originalSize[1]);
		glfwSetWindowPos(g_glfwindow, mouse_x, window_y);
		break;
	case 2:
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		dif = guiMousePos.x - clickedPos.x;
		glfwSetWindowSize(g_glfwindow, originalSize[0] + dif, originalSize[1]);
		break;
	case 3:
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		glfwSetWindowSize(g_glfwindow, originalSize[0], originalSize[1] + ((originalPos[1] + clickedPos.y) - mouse_y));
		glfwSetWindowPos(g_glfwindow, window_x, mouse_y);
		break;
	case 4:
		dif = guiMousePos.y - clickedPos.y;
		glfwSetWindowSize(g_glfwindow, originalSize[0], originalSize[1] + dif);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		break;
	default:
		break;
	}


	if (g_appstate.ResizeWindowState == 0)
	{
		//if(guiMousePos.x <= 5 && guiMousePos.y <= 5)										{ ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE); }   //UPDATE WHEN Get GLFW 3.4
		//else if (guiMousePos.x >= guiWindowBorder.x && guiMousePos.y >= guiWindowBorder.y)	{ ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE); }
		//else if (guiMousePos.x >= guiWindowBorder.x && guiMousePos.y <= 5)					{ ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNESW); }
		//	else if (guiMousePos.x <= 5 && guiMousePos.y >= guiWindowBorder.y)					{ ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNESW); }
		if (guiMousePos.x <= 5)							{ ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW); if (ImGui::GetIO().MouseDown[0]) { g_appstate.ResizeWindowState = 1; } }
		else if (guiMousePos.x >= guiWindowBorder.x)	{ ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW); if (ImGui::GetIO().MouseDown[0]) { g_appstate.ResizeWindowState = 2; } }
		else if (guiMousePos.y <= 5)					{ ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS); if (ImGui::GetIO().MouseDown[0]) { g_appstate.ResizeWindowState = 3; } }
		else if (guiMousePos.y >= guiWindowBorder.y)	{ ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS); if (ImGui::GetIO().MouseDown[0]) { g_appstate.ResizeWindowState = 4; } }

		if (g_appstate.ResizeWindowState != 0)
		{
			clickedPos = ImGui::GetIO().MousePos;
			glfwGetWindowPos(g_glfwindow, &originalPos[0], &originalPos[1]);
			glfwGetFramebufferSize(g_glfwindow, &originalSize[0], &originalSize[1]);
		}
	}

	if (!ImGui::GetIO().MouseDown[0] && g_appstate.ResizeWindowState != 0)
		g_appstate.ResizeWindowState = 0;


}


