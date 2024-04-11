#include "Object.hpp"
#include "EditorPouLadder.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "uuid_v4.h"

#include "EditorPou.hpp"
#include "EditorDut.hpp"
#include "EditorTextFile.hpp"
#include "EditorPouLadder.hpp"

PlcEditorObject::PlcEditorObject(ObjType type, const char* name)
{
	string_t stmp;
	//Create a Unique UUID to be Used as Indentifier;
	UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
	m_uuid = uuidGenerator.getUUID().str();

	//Set Object Name
	SetName(name);

	m_type = type;

	switch (type)
	{
	case PlcEditorObject::OBJ_TYPE_APPLICATION:
	case PlcEditorObject::OBJ_TYPE_FOLDER:
		m_Editor = NULL;
		break;
	case PlcEditorObject::OBJ_TYPE_PLC_POUP_PROGRAM:
		m_Editor = new EditorPou();
		sprintf(stmp.c, u8"PROGRAM %s\n VAR \n\n\n\n\n\n\n\n\n\nEND_VAR\n", name);
		m_Editor->SetVarData(stmp.c);
		m_Editor->SetCodeData("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		break;
	case PlcEditorObject::OBJ_TYPE_PLC_POUP_PROGRAM_LADDER:
		m_Editor = new EditorPouLadder();	
		break;
	case PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION:
		m_Editor = new EditorPou();
		sprintf(stmp.c, u8"FUNCTION %s : TYPE \n VAR_INPUT\n\n\nEND_VAR\nVAR\n\n\n\n\n\nEND_VAR\n", name);
		m_Editor->SetVarData(stmp.c);
		m_Editor->SetCodeData("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		break;
	case PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION_BLOCK:
		m_Editor = new EditorPou();
		sprintf(stmp.c, u8"FUNCTION_BLOCK %s\n\nVAR_INPUT\n\nEND_VAR\n\nVAR_OUTPUT\n\nEND_VAR\n\nVAR\n\nEND_VAR\n", name);
		m_Editor->SetVarData(stmp.c);
		m_Editor->SetCodeData("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		break;
	case PlcEditorObject::OBJ_TYPE_PLC_DUT_ENUM:
		m_Editor = new EditorDut();
		sprintf(stmp.c, u8"TYPE %s : \n( \n\n\n\n);\nEND_TYPE\n", name);
		m_Editor->SetVarData(stmp.c);
		break;
	case PlcEditorObject::OBJ_TYPE_PLC_DUT_STRUCT:
		m_Editor = new EditorDut();
		sprintf(stmp.c, u8"TYPE %s : \nSTRUCT \n\n\n\nEND_STRUCT\nEND_TYPE\n", name);
		m_Editor->SetVarData(stmp.c);
		break;
	case PlcEditorObject::OBJ_TYPE_PLC_GLOBAL_VAR_LIST:
		m_Editor = new EditorDut();
		m_Editor->SetVarData(u8"VAR_GLOBAL: \n\n\n\n\n\nEND_VAR\n");
		break;
	case PlcEditorObject::OBJ_TYPE_PLC_GLOBAL_VAR_LIST_PERSIST:
		m_Editor = new EditorDut();
		m_Editor->SetVarData(u8"VAR_GLOBAL PERSISTENT RETAIN: \n\n\n\n\n\nEND_VAR\n");
		break;
	case PlcEditorObject::OBJ_TYPE_TEXT_FILE:
		m_Editor = new EditorTextFile();
		m_Editor->SetVarData(u8"\n\n\n\n\n\n\n\n\n\n");
		break;
	default:
		m_Editor = NULL;
		break;
	}


}

PlcEditorObject::~PlcEditorObject()
{
	//Clean out pointers

	if (m_Editor)
		delete m_Editor;

	for (auto children : m_childrens)
	{
		delete children;
	}

	m_childrens.clear();
}

const char* PlcEditorObject::GetTypeAsString()
{
	switch (m_type)
	{
	case PlcEditorObject::OBJ_TYPE_APPLICATION:			return u8"APPLICATION";
	case PlcEditorObject::OBJ_TYPE_FOLDER:				return u8"FOLDER";
	case PlcEditorObject::OBJ_TYPE_PLC_POUP_PROGRAM:		return u8"PROGRAM";
	case PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION:		return u8"FUNCTION";
	case PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION_BLOCK:	return u8"FUNCTION_BLOCK";
	case PlcEditorObject::OBJ_TYPE_PLC_DUT_ENUM:			return u8"ENUM";
	case PlcEditorObject::OBJ_TYPE_PLC_DUT_STRUCT:			return u8"STRUCT";
	case PlcEditorObject::OBJ_TYPE_PLC_GLOBAL_VAR_LIST:		return u8"VAR_LIST";
	case PlcEditorObject::OBJ_TYPE_PLC_GLOBAL_VAR_LIST_PERSIST:return u8"RETAIN";
	case PlcEditorObject::OBJ_TYPE_TEXT_FILE:			return u8"TEXT_FILE";
	default: return u8"";
	}

	return u8"";
}

void PlcEditorObject::RigthClickMenuItens()
{
	string_t tmp;


	UI_Style::SetFont(UI_Style::DEFAULT);
	ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0, 0.5));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

	ImGui::PushStyleColor(ImGuiCol_Separator, ImGui::GetStyleColorVec4(ImGuiCol_Border));

	sprintf(tmp.c, u8"###rigth_click_menu_%s", this->m_uuid.c_str());
	if (ImGui::BeginPopupContextItem(tmp.c))
	{

		if (ImGui::MenuItemEx(menuItemName(u8"	Cut", tmp.c), u8"	" UI_ICON_MD_CONTENT_CUT, u8"	CTRL+X", false, true))	{ ; };// { m_Editor->PushSetUserCmdByMenu(m_Editor->CMD_CUT); }
		ImGui::Dummy(ImVec2(2, 2));
		if (ImGui::MenuItemEx(menuItemName(u8"	Copy", tmp.c), u8"	" UI_ICON_MD_CONTENT_COPY, u8"	CTRL+C", false, true))	{ ; };//{ m_Editor->PushSetUserCmdByMenu(m_Editor->CMD_COPY); }
		ImGui::Dummy(ImVec2(2, 2));
		if (ImGui::MenuItemEx(menuItemName(u8"	Paste", tmp.c), u8"	" UI_ICON_MD_CONTENT_PASTE, u8"	CTRL+V", false, true))	{ ; };//{ m_Editor->PushSetUserCmdByMenu(m_Editor->CMD_PASTE); }
		ImGui::Dummy(ImVec2(2, 2));
		if (ImGui::MenuItemEx(menuItemName(u8"	Delete", tmp.c), u8"	" UI_ICON_MD_DELETE, u8"	DEL", false, true))		{ ; };//{ m_Editor->PushSetUserCmdByMenu(m_Editor->CMD_DEL); }
		ImGui::Dummy(ImVec2(5, 5));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(5, 5));
		if (ImGui::MenuItemEx(menuItemName(u8"	Undo", tmp.c), u8"	" UI_ICON_VS_DISCARD, u8"	CTRL+Z", false, true))		{ ; };//{   m_Editor->PushSetUserCmdByMenu(m_Editor->CMD_UNDO); }
		ImGui::Dummy(ImVec2(2, 2));
		if (ImGui::MenuItemEx(menuItemName(u8"	Redo", tmp.c), u8"	" UI_ICON_VS_REDO, u8"	CTRL+Y", false, true))			{ ; };//{   m_Editor->PushSetUserCmdByMenu(m_Editor->CMD_REDO); } 
		ImGui::Dummy(ImVec2(5, 5));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(5, 5));
		if (ImGui::MenuItemEx(menuItemName(u8"	Rename", tmp.c), u8"	", u8"	", false, true)) { ; }

		if (m_type == PlcEditorObject::OBJ_TYPE_PLC_POUP_PROGRAM ||
			m_type == PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION_BLOCK ||
			m_type == PlcEditorObject::OBJ_TYPE_FOLDER ||
			m_type == OBJ_TYPE_APPLICATION
			)
		{

			ImGui::Dummy(ImVec2(5, 5));
			ImGui::Separator();
			ImGui::Dummy(ImVec2(5, 5));

			if (m_type == OBJ_TYPE_APPLICATION || m_type == PlcEditorObject::OBJ_TYPE_FOLDER)
			{
				if (ImGui::MenuItemEx(menuItemName(u8"	Function", tmp.c), u8"	" UI_ICON_VS_SYMBOL_METHOD, u8"", false, true))
				{
					PlcEditorObject* newChildren = new PlcEditorObject(OBJ_TYPE_PLC_POUP_FUNCTION, u8"Function");
					AddChildren(newChildren);
				}

				ImGui::Dummy(ImVec2(2, 2));
				if (ImGui::MenuItemEx(menuItemName(u8"	Function Block", tmp.c), u8"	" UI_ICON_VS_SYMBOL_METHOD, u8"", false, true))
					this->AddChildren(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION_BLOCK, u8"FunctionBlock"));

				ImGui::Dummy(ImVec2(2, 2));
				if (ImGui::MenuItemEx(menuItemName(u8"	Enum", tmp.c), u8"	" UI_ICON_VS_SYMBOL_ENUM, u8"", false, true))
					this->AddChildren(new PlcEditorObject(OBJ_TYPE_PLC_DUT_ENUM, u8"ENUM"));

				ImGui::Dummy(ImVec2(2, 2));
				if (ImGui::MenuItemEx(menuItemName(u8"	Struct", tmp.c), u8"	" UI_ICON_VS_SYMBOL_CLASS, u8"", false, true))
					this->AddChildren(new PlcEditorObject(OBJ_TYPE_PLC_DUT_STRUCT, u8"Struct"));

				ImGui::Dummy(ImVec2(2, 2));
				if (ImGui::MenuItemEx(menuItemName(u8"	VarList", tmp.c), u8"	" UI_ICON_VS_GLOBE, u8"", false, true))
					this->AddChildren(new PlcEditorObject(OBJ_TYPE_PLC_GLOBAL_VAR_LIST, u8"GlobalVar"));

				ImGui::Dummy(ImVec2(2, 2));
				if (ImGui::MenuItemEx(menuItemName(u8"	Retain", tmp.c), u8"	"  UI_ICON_VS_GLOBE, u8"", false, true))
					this->AddChildren(new PlcEditorObject(OBJ_TYPE_PLC_GLOBAL_VAR_LIST_PERSIST, u8"GlobalRetain"));

				ImGui::Dummy(ImVec2(2, 2));
				if (ImGui::MenuItemEx(menuItemName(u8"	Text File", tmp.c), u8"	" UI_ICON_VS_SYMBOL_FILE, u8"", false, true))
					this->AddChildren(new PlcEditorObject(OBJ_TYPE_TEXT_FILE, u8"TextFile"));

			}
			else if (m_type == PlcEditorObject::OBJ_TYPE_PLC_POUP_PROGRAM || m_type == PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION_BLOCK)
			{
				ImGui::Dummy(ImVec2(2, 2));
				if (ImGui::MenuItemEx(menuItemName(u8"	Action", tmp.c), u8"	" UI_ICON_VS_GITHUB_ACTION, u8"", false, true))
				{
					;
				}

				ImGui::Dummy(ImVec2(2, 2));
				if (ImGui::MenuItemEx(menuItemName(u8"	Method", tmp.c), u8"	" UI_ICON_VS_SYMBOL_METHOD, u8"", false, true))
				{
					;
				}

			}


		}

		ImGui::Dummy(ImVec2(5, 5));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(5, 5));
		if (ImGui::MenuItemEx(menuItemName(u8"	Folder", tmp.c), u8"	" UI_ICON_MD_FOLDER, u8"", false, true))
		{
			this->AddChildren(new PlcEditorObject(OBJ_TYPE_FOLDER, u8"NewFolder"));
		}
		ImGui::Dummy(ImVec2(5, 5));


		ImGui::EndPopup();
	}

	ImGui::PopStyleVar(6);
	ImGui::PopStyleColor();

}

const char* PlcEditorObject::GetIcon()
{
	switch (this->GetType())
	{
		case PlcEditorObject::OBJ_TYPE_FOLDER: return UI_ICON_MD_FOLDER;
		case PlcEditorObject::OBJ_TYPE_PLC_POUP_PROGRAM: return UI_ICON_VS_FILE_CODE;
		case PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION: return  UI_ICON_VS_SYMBOL_METHOD;
		case PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION_BLOCK: return UI_ICON_VS_SYMBOL_METHOD;
		case PlcEditorObject::OBJ_TYPE_PLC_DUT_ENUM: return  UI_ICON_VS_SYMBOL_ENUM;
		case PlcEditorObject::OBJ_TYPE_PLC_DUT_STRUCT: return  UI_ICON_VS_SYMBOL_CLASS;
		case PlcEditorObject::OBJ_TYPE_PLC_GLOBAL_VAR_LIST: return UI_ICON_VS_GLOBE;
		case PlcEditorObject::OBJ_TYPE_PLC_GLOBAL_VAR_LIST_PERSIST: return  UI_ICON_VS_GLOBE;
		case PlcEditorObject::OBJ_TYPE_TEXT_FILE: return UI_ICON_VS_SYMBOL_FILE;
		case PlcEditorObject::OBJ_TYPE_APPLICATION: return " ";
		case PlcEditorObject::OBJ_TYPE_PLC_PROJECT: return UI_ICON_MD_SETTINGS_APPLICATIONS;
		case PlcEditorObject::OBJ_TYPE_MOTION_CONTROLLER: return UI_ICON_MD_SETTINGS_APPLICATIONS;
		case PlcEditorObject::OBJ_TYPE_MOTION_CONTROLLER_AXIS: return UI_ICON_MD_TOKEN;
		case PlcEditorObject::OBJ_TYPE_ETHERCAT_MASTER: return UI_ICON_MD_SETTINGS_APPLICATIONS;
		case PlcEditorObject::OBJ_TYPE_ETHERCAT_SALVES:
		case PlcEditorObject::OBJ_TYPE_ETHERCAT_MODULES: return UI_ICON_MD_TOKEN;
		case PlcEditorObject::OBJ_TYPE_MODBUS_SERVER:
		case PlcEditorObject::OBJ_TYPE_MODBUS_CLIENTS: return UI_ICON_MD_SETTINGS_APPLICATIONS;
		case PlcEditorObject::OBJ_TYPE_MODBUS_CLIENTS_NODE: return UI_ICON_MD_TOKEN;
		default:
		return " ";
			break;
	}

	return "";
}

ImVec4 PlcEditorObject::GetIconColor()
{
	switch (this->GetType())
	{
		case PlcEditorObject::OBJ_TYPE_FOLDER: return UI_COLOR_FOLDER;
		case PlcEditorObject::OBJ_TYPE_PLC_POUP_PROGRAM: return UI_COLOR_LIGHT_GREEN;
		case PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION: return UI_COLOR_WHITE;
		case PlcEditorObject::OBJ_TYPE_PLC_POUP_FUNCTION_BLOCK: return UI_COLOR_LIGHT_BLUE;
		case PlcEditorObject::OBJ_TYPE_PLC_DUT_ENUM: return UI_COLOR_STRUCT;
		case PlcEditorObject::OBJ_TYPE_PLC_DUT_STRUCT: return UI_COLOR_STRUCT;
		case PlcEditorObject::OBJ_TYPE_PLC_GLOBAL_VAR_LIST: return UI_COLOR_LIGHT_BLUE;
		case PlcEditorObject::OBJ_TYPE_PLC_GLOBAL_VAR_LIST_PERSIST: return UI_COLOR_LIGHT_BLUE;
		case PlcEditorObject::OBJ_TYPE_TEXT_FILE: return UI_COLOR_WHITE;
		case PlcEditorObject::OBJ_TYPE_APPLICATION: return UI_COLOR_LIGHT_BLUE;
		case PlcEditorObject::OBJ_TYPE_PLC_PROJECT: return  UI_COLOR_LIGHT_GREEN;
		case PlcEditorObject::OBJ_TYPE_MOTION_CONTROLLER: return UI_COLOR_WHITE;
		case PlcEditorObject::OBJ_TYPE_MOTION_CONTROLLER_AXIS: return UI_COLOR_WHITE;
		case PlcEditorObject::OBJ_TYPE_ETHERCAT_MASTER: return UI_COLOR_FOLDER;
		case PlcEditorObject::OBJ_TYPE_ETHERCAT_SALVES:
		case PlcEditorObject::OBJ_TYPE_ETHERCAT_MODULES: return UI_COLOR_FOLDER;
		case PlcEditorObject::OBJ_TYPE_MODBUS_SERVER: return UI_COLOR_LIGHT_BLUE;
		case PlcEditorObject::OBJ_TYPE_MODBUS_CLIENTS: return UI_COLOR_LIGHT_BLUE;
		case PlcEditorObject::OBJ_TYPE_MODBUS_CLIENTS_NODE: return  UI_COLOR_LIGHT_BLUE;
		default: return UI_COLOR_WHITE;
		break;
	}

	return UI_COLOR_WHITE;
}