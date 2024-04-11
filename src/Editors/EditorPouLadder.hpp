#ifndef EDITOR_POUP_LADDER_LIB_H
#define EDITOR_POUP_LADDER_LIB_H


#include "EditorBase.hpp"
#include "UI_Style.h"
#include "LadderRung.hpp"
#include "imgui.h"
#include "standartypes.h"
#include <cstddef>
#include <vector>


class EditorPouLadder : public EditorBase
{
public:
	
	struct ComandData
	{
		EDITOR_COMMANDS Cmd;
		std::string 	DataSource;
	};


	EditorPouLadder()
	{
		//Create A Unique name for TextEditor child Window
		sprintf(m_editor_name_id, "###_editor_pou_ladder_%ld", this);
        m_ladder_rungs.push_back(LadderRung());
		m_ladder_rungs.push_back(LadderRung());
		m_ladder_rungs.push_back(LadderRung());
		m_ladder_rungs.push_back(LadderRung());
		m_ladder_rungs.push_back(LadderRung());
		m_ladder_rungs.push_back(LadderRung());
		m_ladder_rungs.push_back(LadderRung());
		m_ladder_rungs.push_back(LadderRung());
		m_ladder_rungs.push_back(LadderRung());


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
	void		PushSetUserCmdByMenu(EDITOR_COMMANDS cmd) override { ; }
	int			GetActualLineIdx() override			{ return 0;}
	int			GetActualColdx() override			{ return 0;}
	bool		IsOverwrite() override				{ return false; }
	void		SetReadOnly(bool value) override	{ ; }
	bool		IsReadOnly() override				{ return false; }

    
	void DrawWindow(ImVec2 size) override
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::ColorConvertU32ToFloat4(0xff1e1e1e));
        if(ImGui::BeginChild(this->m_editor_name_id,size,false,ImGuiWindowFlags_HorizontalScrollbar))
        {
            ImDrawList* drawlist = ImGui::GetWindowDrawList();

			//Give a space on Top
			ImGui::Dummy(ImVec2(5,5));

			//Draw Top Menu



			//Draw All Lines
			for (size_t i = 0; i < m_ladder_rungs.size(); i++) 
			{
				m_ladder_rungs[i].Draw(drawlist, i);
			}

            ImGui::EndChild();
        }
		ImGui::PopStyleColor();
	};

private:
	
    std::vector<LadderRung> m_ladder_rungs;
    bool		m_editorChanged;
	char		m_editor_name_id[40];
};


#endif
