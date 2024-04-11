/*=============================================================================
 * Copyright (c) 2022 Leise - Motion Controllers
 *
 * @Script: AxisView.cpp
 * @Author: rafaelkrause
 * @Email: rafaelkrause@gmail.com
 * @Create At: 2022-12-07 
 * @Last Modified By: rafaelkrause
 * @Last Modified At: 
 * @Description: Main Window
 *============================================================================*/
// =============================================================================
//                                  INCLUDES
// =============================================================================
#include <cstddef>
#pragma warning(suppress: 4005)
#include "os_generic.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "UI_Widgets.h"
#include "UI_Windows.h"
#include "UI_Style.h"
#include "standartypes.h"
#include "ui_core.hpp"

#include "Object.hpp"
#include "EditorDut.hpp"
//#include "IecProgram.hpp"

// =============================================================================
//                             DEFINES AND CONSTANTS
// =============================================================================
#define MAIN_WINDOW_HEADER_HEIGTH	30
#define STATUSBAR_HEIGTH			20



// =============================================================================
//                               LOCAL VARIABLES
// =============================================================================



// =============================================================================
//                             FOWARD DECLARATIONS
// =============================================================================
static void TopMenuBar();
static void WorkSpace();
static void StatusBar();

static void FileMenu();
static void EditMenu();

static void ProjectTree();
static void AddProjectNode(PlcEditorObject* obj);
static bool DrawNode(const char* icon, const char* label, const char* type, ImVec4* icon_color, bool haschildren);
static void EditorsTabRigthClick(IecProject* project, PlcEditorObject* editor);


// =============================================================================
//                            PUBLIC FUNCTIONS
// =============================================================================
void UI_Windows::MainWindow()
{
	const ImGuiViewport *viewport = ImGui::GetMainViewport();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(viewport->Size);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoSavedSettings;
	if (ImGui::Begin("###MainWindow", NULL, flags))
	{
		TopMenuBar();
		WorkSpace();
		StatusBar();
		ImGui::End();
	}
}

// =============================================================================
//                             LOCAL WIDGETS
// =============================================================================

static void TopMenuBar()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, UI_COLOR_BG_LEVEL_5);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, 1));

	ImVec2 windowButtonSize(40, MAIN_WINDOW_HEADER_HEIGTH);

	UI_Style::SetFont(UI_Style::DEFAULT);
	//ImGui::Dummy(ImVec2(1,1));
	ImGui::BeginGroup();

	ImGui::Dummy(ImVec2(10, 6));
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Dummy(ImVec2(4, 6 - ImGui::GetStyle().ItemSpacing.y));
	ImGui::Image((ImTextureID)UI_Style::GetImgFromAtlas(0), ImVec2(MAIN_WINDOW_HEADER_HEIGTH - 12, MAIN_WINDOW_HEADER_HEIGTH - 12));
	ImGui::Dummy(ImVec2(4, 6));
	ImGui::EndGroup();

	ImGui::SameLine(); ImGui::Dummy(ImVec2(5, 5));

	ImGui::SameLine(); FileMenu();
	ImGui::SameLine(); EditMenu();

	ImGui::PushStyleColor(ImGuiCol_Button, UI_COLOR_BG_LEVEL_5);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, UI_COLOR_BG_LEVEL_5);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, UI_COLOR_BG_LEVEL_5);
	ImGui::PushStyleColor(ImGuiCol_Text, UI_COLOR_WHITE_DISABLE);
	ImGui::SameLine();

	bool isMovingWindow = UI_Widgets::ButtonPush(UI_Core::GetAppTitle(), ImVec2(ImGui::GetContentRegionAvail().x - 3 * (40), MAIN_WINDOW_HEADER_HEIGTH));
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
	{
		UI_Core::SetIsMaximized(!UI_Core::GetIsMaximized());
		isMovingWindow = false;
	}

	UI_Core::MoveWindow(isMovingWindow);

	ImGui::PopStyleColor(4);


	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
	ImGui::PushStyleColor(ImGuiCol_Button, UI_COLOR_BG_LEVEL_5);
	UI_Style::SetFont(UI_Style::MEDIUM);
	ImGui::SameLine();
	if (ImGui::Button(UI_ICON_VS_CHROME_MINIMIZE u8"###TopMenuMinimizeBt", windowButtonSize))
		UI_Core::MinimizeApplication();

	ImGui::SameLine();
	if (UI_Core::GetIsMaximized())
	{
		if (ImGui::Button(UI_ICON_VS_CHROME_RESTORE u8"###TopMenuRestoreBt", windowButtonSize))
			UI_Core::SetIsMaximized(false);

	}
	else
	{
		if (ImGui::Button(UI_ICON_VS_CHROME_MAXIMIZE u8"###TopMenuRestoreBt", windowButtonSize))
			UI_Core::SetIsMaximized(true);
	}

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, UI_COLOR_RED);
	if (ImGui::Button(UI_ICON_VS_CHROME_CLOSE u8"###TopMenuCloseBt", windowButtonSize))
		UI_Core::CloseApplication();

	UI_Style::SetFont(UI_Style::DEFAULT);
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);

	ImGui::EndGroup();

	ImGui::PopStyleVar();


}

static void FileMenu()
{
	ImVec2 menuPos = ImGui::GetCursorPos();
	ImVec2 menuBtSize = ImVec2(50, MAIN_WINDOW_HEADER_HEIGTH);
	ImVec2 submenuBtSize = ImVec2(150, MAIN_WINDOW_HEADER_HEIGTH);


	ImGui::PushStyleColor(ImGuiCol_Button, UI_COLOR_BG_LEVEL_5);
	if (ImGui::Button("File###TopMenu_File", menuBtSize))
	{
		ImGui::SetNextWindowPos(ImVec2(menuPos.x, menuBtSize.y));
		ImGui::OpenPopup("###TopMenu_File_PopUP");
	}


	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5));
	if (ImGui::BeginPopup("###TopMenu_File_PopUP"))
	{
		if (ImGui::Button(u8"New Project###TopMenu_File_New", submenuBtSize))
		{
			UI_Core::NewProject();
			ImGui::CloseCurrentPopup();
		}


		ImGui::Button(u8"Open Project###TopMenu_File_Open", submenuBtSize);
		ImGui::Button(u8"Close Project###TopMenu_File_Close", submenuBtSize);
		ImGui::Separator();
		ImGui::Button(u8"Save Project###TopMenu_File_Save", submenuBtSize);
		ImGui::Button(u8"Save Project As ###TopMenu_File_SaveAs", submenuBtSize);
		ImGui::Separator();
		ImGui::Button(u8"Recents Projects###TopMenu_File_Recent", submenuBtSize);
		ImGui::Separator();
		ImGui::Button(u8"Exit###TopMenu_File_Exit", submenuBtSize);
		ImGui::EndPopup();
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

static void EditMenu()
{
	ImVec2 menuPos = ImGui::GetCursorPos();
	ImVec2 menuBtSize = ImVec2(50, MAIN_WINDOW_HEADER_HEIGTH);
	ImVec2 submenuBtSize = ImVec2(150, MAIN_WINDOW_HEADER_HEIGTH);
	
	UI_Style::SetFont(UI_Style::DEFAULT);
	ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0, 0.5));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

	ImGui::PushStyleColor(ImGuiCol_Separator, ImGui::GetStyleColorVec4(ImGuiCol_Border));

	ImGui::PushStyleColor(ImGuiCol_Button, UI_COLOR_BG_LEVEL_5);
	if (ImGui::Button(u8"Edit###TopMenu_Edit", menuBtSize))
	{
		ImGui::SetNextWindowPos(ImVec2(menuPos.x, menuBtSize.y));
		ImGui::OpenPopup(u8"###TopMenu_Edit_PopUP");
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5));
	if (ImGui::BeginPopup(u8"###TopMenu_Edit_PopUP"))
	{
		ImGui::Button(u8"Undo###TopMenu_Edit_Undo", submenuBtSize);
		ImGui::Button(u8"Redo###TopMenu_Edit_Redo", submenuBtSize);
		ImGui::Dummy(ImVec2(5, 5));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(5, 5));
		ImGui::Button(u8"Cut###TopMenu_Edit_Cut", submenuBtSize);
		ImGui::Button(u8"Copy###TopMenu_Edit_Copy", submenuBtSize);
		ImGui::Button(u8"Paste###TopMenu_Edit_Paste", submenuBtSize);
		ImGui::Dummy(ImVec2(5, 5));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(5, 5));
		ImGui::Button(u8"Find###TopMenu_Edit_Find", submenuBtSize);
		ImGui::Button(u8"Replace###TopMenu_Edit_Replace", submenuBtSize);

		ImGui::EndPopup();
	}

	ImGui::PopStyleVar(7);
	ImGui::PopStyleColor();

}

static void WorkSpace()
{
	const ImGuiViewport *viewport = ImGui::GetMainViewport();
	IecProject* project = UI_Core::GetProject();
	string_t tmp;

	float ProjectTreeSize_x = UI_Core::GetProjectTreeSize() * viewport->Size.x;
	static float ProjectTreeSize_x_aux = 0;
	float WorkSpaceSize_y = UI_Core::GetConsoleSize() * viewport->Size.y;
	static float WorkSpaceSize_y_aux = 0;

	//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
	UI_Widgets::Splitter("###ProjeTreeSpliter",true, 4.0f, &ProjectTreeSize_x, &ProjectTreeSize_x_aux, 100, -500);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, UI_COLOR_SPLITER);
	ImGui::BeginChild("###ProjectTreeView", ImVec2(ProjectTreeSize_x, ImGui::GetContentRegionAvail().y - STATUSBAR_HEIGTH), true);
	ImGui::PopStyleVar();

	ProjectTree();

	ImGui::EndChild();
	
	UI_Core::SetProjectTreeSize(ProjectTreeSize_x / iec_max(1, viewport->Size.x));


	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
	UI_Widgets::Splitter("###WorkSpaceSpliter", false, 4.0f, &WorkSpaceSize_y, &WorkSpaceSize_y_aux, 10.0f, -(ImGui::GetContentRegionAvail().y - 50));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, UI_COLOR_WORKSPACE_BG);
	ImGui::BeginChild("###WorkSpace", ImVec2(-1, WorkSpaceSize_y), true, ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	//Active Tabs
	ImGui::BeginGroup();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2,0));
		
	//Loop All Opened Editors
	if (project)
	{

		for (size_t i = 0; i < project->GetObjectsOpened().size(); i++)
		{
			PlcEditorObject* editor = project->GetObjectsOpened().at(i);
			if (editor)
			{
				bool isActive = (editor == project->GetActiveEditor());

				if(isActive)
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::ColorConvertU32ToFloat4(0xff1e1e1e));

				if (i != 0)
					ImGui::SameLine();

				bool_t isClose = false;
				if(UI_Widgets::TabButton(editor->GetName(), editor->GetIcon(), ImVec2(0,0), editor->GetIconColor(), NULL, &isClose))
					project->SetActiveEditor(editor);

				if(isClose)
					project->PopOpenObject(editor);

				if (isActive)
					ImGui::PopStyleColor();

				EditorsTabRigthClick(project, editor);
			}
		}
	}
	else
	{

		//ImVec2 space = ImGui::GetContentRegionAvail();
		//float minSize = iec_min(space.x,space.y);
		//ImVec2 imgSize = ImVec2( round(minSize * 0.4),round(minSize * 0.4));

		//ImGui::Dummy(ImVec2((space.x - imgSize.x) / 2, (space.y - imgSize.y) / 2));
		//ImGui::BeginGroup();
		//ImGui::Dummy(ImVec2((space.x - imgSize.x) / 2, 1));
		//ImGui::SameLine();
		//ImGui::Image((ImTextureID)UI_Style::GetImgFromAtlas(1), imgSize);
		//ImGui::EndGroup();
	}



	//Is Active, Change Color
	//ImGui::PushStyleColor(ImGuiCol_Button, ImGui::ColorConvertU32ToFloat4(0xff1e1e1e));
	//ImGui::Button(" AXIS_STATE	X ");
	//ImGui::PopStyleColor();


	//ImGui::SameLine(); ImGui::Button(" CTRL_STATE	X ");


	ImGui::EndGroup();
	ImGui::PopStyleVar(3);
	
	UI_Core::SetConsoleSize(WorkSpaceSize_y / iec_max(1, viewport->Size.y));

	//Show Active Editor
	if (project)
	{
		if (project->GetActiveEditor())
		{
			if(project->GetActiveEditor()->GetEditor())
				project->GetActiveEditor()->GetEditor()->DrawWindow(ImGui::GetContentRegionAvail());
		}
	}
		
	ImGui::EndChild();
	

	ImGui::PushStyleColor(ImGuiCol_ChildBg, UI_COLOR_WORKSPACE_BG);
	ImGui::BeginChild("###ConsoleSpace", ImVec2(-1, ImGui::GetContentRegionAvail().y - STATUSBAR_HEIGTH), true, ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PopStyleColor();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 12));
	
	//Loop all View Editors
	
	ImGui::Button("Console");
	ImGui::SameLine(); ImGui::Button("Compiler");
	ImGui::SameLine(); ImGui::Button("Watch");

	//Show Active Console Editor


	ImGui::PopStyleVar();

	ImGui::EndChild();
	
	
	ImGui::EndGroup();

   	 
}

static void StatusBar()
{
	const ImGuiViewport *viewport = ImGui::GetMainViewport();

	ImVec2 startPos = ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - STATUSBAR_HEIGTH - 2);

	// Set position to the bottom of the viewport
	ImGui::SetCursorPos(startPos);
	//ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - STATUSBAR_HEIGTH + 2));

	// Extend width to viewport width
	//ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, STATUSBAR_HEIGTH + 2));

	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0, startPos.y), ImVec2(viewport->Size.x, startPos.y + STATUSBAR_HEIGTH + 2),ImGui::ColorConvertFloat4ToU32(UI_COLOR_LIGHT_BLUE));


	//ImGui::PushStyleColor(ImGuiCol_WindowBg, UI_COLOR_LIGHT_BLUE);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));


	ImVec2 precompilerStatusSize(150, STATUSBAR_HEIGTH);
	ImVec2 compilerStatusSize(150, STATUSBAR_HEIGTH);
	ImVec2 comunicationStatusSize(150, STATUSBAR_HEIGTH);
	ImVec2 controllerStatusSize(140, STATUSBAR_HEIGTH);
	ImVec2 controllerAdressSize(150, STATUSBAR_HEIGTH);
	ImVec2 editordataSize(200, STATUSBAR_HEIGTH);
	ImVec2 endDummy(20, STATUSBAR_HEIGTH);

	ImVec2 stratusMsgSize(iec_max(10, ImGui::GetContentRegionAvail().x -
		precompilerStatusSize.x -
		compilerStatusSize.x -
		comunicationStatusSize.x -
		controllerStatusSize.x -
		controllerAdressSize.x -
		editordataSize.x -
		endDummy.x - 30)
		, STATUSBAR_HEIGTH);




	UI_Style::SetFont(UI_Style::DEFAULT);
	ImGui::BeginGroup();
	// Status Msg
	const char* statusMsg = UI_Core::GetStatusMsg();
	if (statusMsg)
		UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_WHITE, stratusMsgSize, UI_Widgets::TXT_RIGTH, false, u8" %s", statusMsg);
	else
		ImGui::Dummy(stratusMsgSize);


	//Show Editor Status
	if (UI_Core::GetProject() && UI_Core::GetProject()->GetActiveEditor())
	{
		ImGui::SameLine(); UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_WHITE, editordataSize, UI_Widgets::TXT_CENTER, false, u8"Ln:%d	col: %d		%s",
			UI_Core::GetProject()->GetActiveEditor()->GetEditor()->GetActualLineIdx(),
			UI_Core::GetProject()->GetActiveEditor()->GetEditor()->GetActualColdx(),
			"INS"
		);
	}
	//Comunication Status
	ImGui::SameLine(); UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_WHITE, comunicationStatusSize, UI_Widgets::TXT_CENTER, false, u8"Offline");

	//Controller Adress
	ImGui::SameLine(); UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_WHITE, controllerAdressSize, UI_Widgets::TXT_CENTER, false, u8"192.168.0.30");

	//Controller Status
	ImGui::SameLine(); UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_WHITE, controllerStatusSize, UI_Widgets::TXT_CENTER, false, u8"Stopped");

	//Compiler Status
	ImGui::SameLine(); UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_WHITE, ImVec2(precompilerStatusSize.x - 40, precompilerStatusSize.y), UI_Widgets::TXT_RIGTH, false, u8"Pre Compiler :");
	ImGui::SameLine(); UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_LIGHT_GREEN, ImVec2(40, precompilerStatusSize.y), UI_Widgets::TXT_LEFT, false, UI_ICON_MD_CHECK_CIRCLE);

	// Show Last Errors / Warnings Totals
	ImGui::SameLine(); UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_LIGHT_RED, ImVec2(20, compilerStatusSize.y), UI_Widgets::TXT_CENTER, false, UI_ICON_MD_ERROR);
	ImGui::SameLine(); UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_WHITE, ImVec2(compilerStatusSize.x / 2 - (20 + ImGui::GetStyle().ItemSpacing.x * 2), compilerStatusSize.y), UI_Widgets::TXT_CENTER, false, u8"%d", 10);
	ImGui::SameLine(); UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_YELLOW, ImVec2(20, compilerStatusSize.y), UI_Widgets::TXT_CENTER, false, UI_ICON_MD_WARNING);
	ImGui::SameLine(); UI_Widgets::VarDisplay(UI_COLOR_TRANS, UI_COLOR_WHITE, ImVec2(compilerStatusSize.x / 2 - (20 + ImGui::GetStyle().ItemSpacing.x * 2), compilerStatusSize.y), UI_Widgets::TXT_CENTER, false, u8"%d", 500);

	ImGui::SameLine(); ImGui::Dummy(endDummy);

	ImGui::EndGroup();


	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(1);
}

static void ProjectTree()
{
	IecProject* project = UI_Core::GetProject();

	if (!project)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(2, 2));

	bool nodeIsOpen = false;
	const char* icon = UI_ICON_MD_FOLDER;
	ImVec4 color = UI_COLOR_YELLOW;

	for (size_t i = 0; i < project->GetNumberOfChildren(); i++)
	{
		PlcEditorObject* child = project->GetChildren(i);

		if (child)
		{
			AddProjectNode(child);
		}
	}

	

	ImGui::PopStyleVar(3);

}

static void AddProjectNode(PlcEditorObject* obj)
{
	if (!obj)
		return;

	string_t strTmp;

	sprintf(strTmp.c,  u8" %s###ProjectTree_%s", obj->GetName(), obj->GetUUID());

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_FramePadding;


	bool nodeIsOpen = false;
	const char* type = obj->GetTypeAsString();
	const char* icon = obj->GetIcon();
	ImVec4 color = obj->GetIconColor();

	nodeIsOpen = DrawNode(icon, strTmp.c, type, &color, obj->GetNumberOfChilderns() > 0);

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		// Set payload to carry the index of our item (could be anything)
		ImGui::SetDragDropPayload("PlcEditorObject", obj, sizeof(PlcEditorObject));
		UI_Widgets::Label(UI_COLOR_WHITE, ImVec2(0,ImGui::GetFontSize()*2.5), UI_Widgets::TXT_LEFT, u8"%s", obj-> GetName());
		ImGui::EndDragDropSource();
	}

	if (nodeIsOpen)
	{
		//Reset Selection
		if (ImGui::IsItemClicked())
		{
			UI_Core::GetProject()->PushOpenObject(obj);
		}

		obj->RigthClickMenuItens();

		for (size_t i = 0; i < obj->GetNumberOfChilderns(); i++)
		{
			PlcEditorObject* child = obj->GetChildren(i);

			if (child)
			{
				AddProjectNode(child);
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					// Set payload to carry the index of our item (could be anything)
					ImGui::SetDragDropPayload("PlcEditorObject", child, sizeof(PlcEditorObject));
					UI_Widgets::Label(UI_COLOR_WHITE, ImVec2(0,ImGui::GetFontSize()*2.5), UI_Widgets::TXT_LEFT, u8"%s", child->GetName());
					ImGui::EndDragDropSource();
				}
			}
		}

		ImGui::TreePop();
	}
	else
	{
		obj->RigthClickMenuItens();
	}
}

static bool DrawNode(const char* icon, const char* label, const char* type, ImVec4* icon_color, bool haschildren)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	int nodeDeep = window->DC.TreeDepth;

	ImU32 id = window->GetID(label);
	ImVec2 pos = window->DC.CursorPos;
	ImRect bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + (g.FontSize * 1.3) + g.Style.FramePadding.y * 2));
	bool opened = ImGui::TreeNodeBehaviorIsOpen(id);
	bool hovered, held;
	if (ImGui::ButtonBehavior(bb, id, &hovered, &held, 0))
		window->DC.StateStorage->SetInt(id, opened ? 0 : 1);
	if (hovered || held)
		window->DrawList->AddRectFilled(bb.Min, bb.Max,  ImGui::GetColorU32(held ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered));


	ImGui::Dummy(ImVec2(nodeDeep * g.FontSize * 1.3 , 2));

	ImGui::SameLine();
	if (haschildren)
	{
		UI_Widgets::Label(ImGui::GetStyleColorVec4(ImGuiCol_Text), ImVec2(g.FontSize * 1.3, bb.GetSize().y), UI_Widgets::TXT_CENTER, u8"%s", opened ? u8"\ue5cf" : u8"\ue5cc");
	}
	else
	{
		UI_Widgets::Label(ImGui::GetStyleColorVec4(ImGuiCol_Text), ImVec2(g.FontSize * 1.3, bb.GetSize().y), UI_Widgets::TXT_CENTER, u8" ");
	}

	ImGui::SameLine();
	UI_Widgets::Label(icon_color ? *icon_color : ImGui::GetStyleColorVec4(ImGuiCol_Text), ImVec2(g.FontSize * 1.3, bb.GetSize().y), UI_Widgets::TXT_CENTER, u8"%s", icon);
	ImGui::SameLine();
	UI_Widgets::Label(ImGui::GetStyleColorVec4(ImGuiCol_Text), ImVec2(0, bb.GetSize().y), UI_Widgets::TXT_LEFT, u8"%s", label);
	//ImGui::SameLine();
	//UI_Widgets::Label(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled), ImVec2(0, bb.GetSize().y), UI_Widgets::TXT_LEFT, u8" (%s)", type);

	//ImGui::ItemSize(bb);
	ImGui::ItemAdd(bb, id);

	if (opened)
		ImGui::TreePush(label);

	return opened;

}

static void EditorsTabRigthClick(IecProject* project, PlcEditorObject* editor)
{
	if(!editor)
		return;


	UI_Style::SetFont(UI_Style::DEFAULT);
	ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0, 0.5));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

	ImGui::PushStyleColor(ImGuiCol_Separator, ImGui::GetStyleColorVec4(ImGuiCol_Border));

	string_t tmp;
	sprintf(tmp.c, u8"###rigth_click_tab__%s", editor->GetUUID());
	if (ImGui::BeginPopupContextItem(tmp.c))
	{
		if (ImGui::MenuItemEx("	Close###rigth_click_tab_close", u8"	", u8"	", false, true)){ project->PopOpenObject(editor); };
		ImGui::Dummy(ImVec2(2, 2));
		if (ImGui::MenuItemEx("	Close Others###rigth_click_tab_close_others", u8"	", u8"	", false, true)){ project->CloseOthersObjects(editor); };
		ImGui::Dummy(ImVec2(2, 2));
		if (ImGui::MenuItemEx("	Close To Rigth###rigth_click_tab_close_to_rigth", u8"	", u8"	", false, true)){ project->CloseRigthOpenObjects(editor); };
		ImGui::Dummy(ImVec2(2, 2));
		if (ImGui::MenuItemEx("	Close All###rigth_click_tab_close_all", u8"	", u8"	", false, true)){ project->CloseOpenAllObjects(); };
		ImGui::Dummy(ImVec2(2, 2));
		ImGui::EndPopup();
	}


	ImGui::PopStyleVar(6);
	ImGui::PopStyleColor();

}

