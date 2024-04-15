#ifndef EDITOR_POUP_LADDER_LIB_H
#define EDITOR_POUP_LADDER_LIB_H


#include "EditorBase.hpp"
#include "UI_Style.h"
#include "LadderRung.hpp"
#include "imgui.h"
#include "log.h"
#include "standartypes.h"
#include "ui_core.hpp"
#include <cstddef>
#include <string>
#include <vector>

class EditorPouLadder : public EditorBase
{
public:

	class EditorPouLadderCommand
	{
		public:
		EditorPouLadderCommand() { m_can_undo = false; m_can_redo = false;};
		~EditorPouLadderCommand() {;};

		virtual void Execute(EditorPouLadder *_editor) {;};
		virtual void Undo(EditorPouLadder *_editor) {LOG_ERROR("", "UNDO");};
		virtual void Redo(EditorPouLadder *_editor) {LOG_ERROR("", "REDO");;};
		bool m_can_undo;
		bool m_can_redo;
	};
	
	EditorPouLadder()
	{
		//Create A Unique name for TextEditor child Window
		UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
	    m_uuid = uuidGenerator.getUUID().str();
		sprintf(m_editor_name_id, "###_editor_pou_ladder_%s", m_uuid.c_str());
        //m_ladder_rungs.push_back(LadderRung());
		//m_ladder_rungs.push_back(LadderRung());
		//m_ladder_rungs.push_back(LadderRung());
		//m_ladder_rungs.push_back(LadderRung());
		//m_ladder_rungs.push_back(LadderRung());
		//m_ladder_rungs.push_back(LadderRung());
		//m_ladder_rungs.push_back(LadderRung());
		//m_ladder_rungs.push_back(LadderRung());
		//m_ladder_rungs.push_back(LadderRung());
		m_redo_history_pos = 0;
	};
	
	~EditorPouLadder()
	{
		;
	}

	bool		HasWindow()  override				{ return true; };
	bool		HasSavePending() override			{ return m_editorChanged; }
	void		SetSaved() override					{ m_editorChanged = false; }
	bool		IsFocused() override				{ return false; }

	const char* GetVarData() override			    { return NULL;}
	void		SetVarData(const char* data) override  { ; }
	int			GetActualLineIdx() override			{ return 0;}
	int			GetActualColdx() override			{ return 0;}
	bool		IsOverwrite() override				{ return false; }
	void		SetReadOnly(bool value) override	{ ; }
	bool		IsReadOnly() override				{ return false; }
	
	void		PushCommand(EditorPouLadderCommand* _cmd);
	void		Undo();
	void		Redo();
	
	void		PushSelectedRung(std::string& rung_uuid)				{ m_selected_rungs.push_back(rung_uuid); }
	void 		ClearSelectedRungs()									{ m_selected_rungs.clear();	}
	const std::vector<std::string> &GetSelectedRungsUUIDs() const			{ return  m_selected_rungs ;}
	
	void 		PushSelectedLadderElement(std::string& element_uuid)	{ m_selected_ladder_elements.push_back(element_uuid);}
	void 		ClearSelectedLadderElements()							{ m_selected_ladder_elements.clear();}

	void		AddRung(LadderRung _rung)								{ m_ladder_rungs.push_back(_rung);}
	void 		InsertRung(LadderRung _rung, size_t _pos);	

	void 		RemoveRungByPos(size_t _pos);
	void 		RemoveRungByUUID(std::string& _UUID);

	LadderRung& GetRungByUUID(std::string& _UUID);
	int			GetRungIdxByUUID(std::string& _UUID);

	void DrawWindow(ImVec2 size) override;

private:

	std::string m_uuid;	
    std::vector<LadderRung> m_ladder_rungs;
	std::vector<std::string> m_selected_rungs;
	std::vector<std::string> m_selected_ladder_elements;
	std::vector<EditorPouLadderCommand*> m_redo_history;

	int			m_redo_history_pos;
    bool		m_editorChanged;
	char		m_editor_name_id[40];
	bool		m_call_goto_popup;
	bool		m_call_find_popup;

	// //Copy a Rung on Element (According to selection) to Clipboard
	// void CommandCopy()
	// {
	// 	// //Copy one Element
	// 	// if(m_selected_ladder_elements.size() == 1 && m_selected_rungs.size() == 1)
	// 	// {
	// 	// 	for (auto& r : m_ladder_rungs) 
	// 	// 	{
	// 	// 		if(r.GetUUID() == m_selected_rungs[0])
	// 	// 		{
	// 	// 			for (auto& e : r.GetElements())
	// 	// 			{
	// 	// 				if(e.GetUUID() == m_selected_ladder_elements[0])
	// 	// 				{
	// 	// 					//Convert data to JSON
	// 	// 					const char* jsonData = e.ToJson();
	// 	// 					//Copy to clipboard
	// 	// 					UI_Core::ClipBoardCopy(LADDER_ELEMENT_CLIPBOARD_FORMAT , (const char*)jsonData, strlen(jsonData)+1);    
	// 	// 				}
	// 	// 			break;
	// 	// 			}

	// 	// 			break;
	// 	// 		}
	// 	// 	}
	// 	// }
	// 	// //copy Multiple elements
	// 	// else if(m_selected_ladder_elements.size() > 1 && m_selected_rungs.size() == 1)
	// 	// {

	// 	// }
	// 	// //Copy on line	
	// 	// else if(m_selected_rungs.size() == 1)
	// 	// {
	// 	// 	for (auto& r : m_ladder_rungs) 
	// 	// 	{
	// 	// 		if(r.GetUUID() == m_selected_rungs[0])
	// 	// 		{
	// 	// 			const char* jsonData = r.ToJson();
    //     //         	UI_Core::ClipBoardCopy(LADDER_RUNG_CLIPBOARD_FORMAT , (const char*)jsonData, strlen(jsonData)+1);    
	// 	// 		}
	// 	// 	}
	// 	// }
	// 	// //Copy Multiple Lines
	// 	// else if(m_selected_rungs.size() > 1)
	// 	// {

	// 	// }
	// }

	// void CommandPaste()
	// {
	// 	//Paste Rungs
	// 	if(UI_Core::ClipBoardHasData(LADDER_RUNG_CLIPBOARD_FORMAT))
	// 	{
	// 		char* data = UI_Core::ClipBoardGetData(LADDER_RUNG_CLIPBOARD_FORMAT, NULL);
	// 		if(data)
	// 		{
	// 			//Push Command Add
	// 			//LadderRung newRung;
	// 			//newRung.FromJson(data);
	// 			//m_ladder_rungs.insert();
	// 			//free(data);
	// 		}
	// 	}
	// 	//Paste Elements
	// 	else if(UI_Core::ClipBoardHasData(LADDER_ELEMENT_CLIPBOARD_FORMAT))
	// 	{
	// 		char* data = UI_Core::ClipBoardGetData(LADDER_ELEMENT_CLIPBOARD_FORMAT, NULL);
	// 		if(data)
	// 		{
	// 			//Push Command Inset Elemet

	// 			//LadderRung newRung;
	// 			//newRung.FromJson(data);
	// 			//m_ladder_rungs.insert();
	// 			//free(data);
	// 		}
	// 	}
	// }

	// void CommandDelete()
	// {


	// 	// //Search for Elements
	// 	// if(m_selected_ladder_elements.size() > 0 && m_selected_rungs.size() > 0)
	// 	// {
	// 	// 	for (size_t i; m_selected_rungs.size(); i++) 
	// 	// 	{
	// 	// 		for (size_t j; j < m_ladder_rungs.size();j++) 
	// 	// 		{
	// 	// 			if(m_ladder_rungs[j].GetUUID() == m_selected_rungs[i])
	// 	// 			{
	// 	// 				for (size_t w; w < m_selected_ladder_elements.size(); w++) 
	// 	// 				{
	// 	// 					for (size_t h; h < m_ladder_rungs[j].GetElements().size(); w++) 
	// 	// 					{
	// 	// 						if(m_ladder_rungs[j].GetElements()[h].GetUUID() == m_selected_ladder_elements[w])
	// 	// 						{
	// 	// 							m_ladder_rungs[j].RemoveElement(m_selected_ladder_elements[w]);
	// 	// 						}
	// 	// 					}
	// 	// 				}
	// 	// 			}
	// 	// 		}
	// 	// 	}
	// 	// }

		
	// }



};


#endif
