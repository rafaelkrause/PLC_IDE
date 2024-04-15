#include "EditorPouLadder.hpp"
#include "LadderRung.hpp"
#include "UI_Style.h"
#include "imgui.h"
#include "log.h"
#include "standartypes.h"
#include "ui_core.hpp"
#include <cstddef>
#include <string>
#include <vector>
#include "EditorPouLadderCommands.hpp"


constexpr const char* 	LADDER_ELEMENT_CLIPBOARD_FORMAT 	= "Leise.LadderElement";
constexpr const char* 	LADDER_RUNG_CLIPBOARD_FORMAT		= "Leise.LadderRung";
constexpr const int 	LADDER_MAXIMUM_UNDO_HISTORY 		= 20;

	
void EditorPouLadder::PushCommand(EditorPouLadderCommand* _cmd)
{
    _cmd->Execute(this);
    m_redo_history.insert(m_redo_history.cbegin(),_cmd);

    if(m_redo_history.size() > LADDER_MAXIMUM_UNDO_HISTORY)
        m_redo_history.erase(m_redo_history.begin());

}

void EditorPouLadder::Undo()
{
    if(m_redo_history_pos >= 0 && m_redo_history_pos < m_redo_history.size())
    {
        auto cmd = m_redo_history[m_redo_history_pos++];
        cmd->Undo(this);
    }

    if(m_redo_history_pos >= m_redo_history.size())
        m_redo_history_pos = m_redo_history.size() -1;

}

void EditorPouLadder::Redo()
{
    
    if(m_redo_history_pos >= 0 && m_redo_history_pos < m_redo_history.size())
    {
        auto cmd = m_redo_history[m_redo_history_pos--];
        cmd->Redo(this);
    }

    if(m_redo_history_pos < 0)
        m_redo_history_pos = 0;

}

void EditorPouLadder::InsertRung(LadderRung _rung, size_t _pos)
{
    if(_pos < m_ladder_rungs.size() -1)
        m_ladder_rungs.insert(m_ladder_rungs.begin() + _pos, _rung);
    else
        m_ladder_rungs.push_back(_rung);
}		

void EditorPouLadder::RemoveRungByPos(size_t _pos)
{
    if(_pos < m_ladder_rungs.size())
        m_ladder_rungs.erase(m_ladder_rungs.begin() + _pos);

}

void EditorPouLadder::RemoveRungByUUID(std::string& _UUID)
{
    for (size_t i = 0; i < m_ladder_rungs.size(); i++) {

        if(m_ladder_rungs[i].GetUUID() == _UUID)
        {
            RemoveRungByPos(i);
            break;
        }
    }
}

LadderRung& EditorPouLadder::GetRungByUUID(std::string& _UUID)
{
    for (size_t i = 0; i < m_ladder_rungs.size(); i++) {
        
        if(m_ladder_rungs[i].GetUUID() == _UUID)
            return m_ladder_rungs[i];
    }

    LOG_FATAL("EditorPouLadder::GetRungByUUID","UUID Not Found. Exception Generated");
    throw std::runtime_error("Ladder Rung not Found. Function EditorPouLadder::GetRungByUUId");
}

int EditorPouLadder::GetRungIdxByUUID(std::string& _UUID)
{
    int result = -1;
    for (size_t i = 0; i < m_ladder_rungs.size(); i++) {
        
        if(m_ladder_rungs[i].GetUUID() == _UUID)
            return i;
    }

    return -1;
}


void EditorPouLadder::DrawWindow(ImVec2 size)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::ColorConvertU32ToFloat4(0xff1e1e1e));
    if(ImGui::BeginChild(this->m_editor_name_id,size,false,ImGuiWindowFlags_HorizontalScrollbar))
    {
        ImDrawList* drawlist = ImGui::GetWindowDrawList();

        //Give a space on Top
        ImGui::Dummy(ImVec2(5,5));

        //Draw Top Menu
        ImGui::BeginGroup();


        if(ImGui::Button("ADD")) 
        {
            this->PushCommand(new CommandAddRung());
        }
        ImGui::SameLine();
        if(ImGui::Button("Remove")) 
        {
            this->PushCommand(new CommandRemoveRung());
        }
        ImGui::SameLine();
        if(ImGui::Button("UNDO")) 
        {
            this->Undo();
        }
        ImGui::SameLine();
        if(ImGui::Button("REDO")) 
        {
            this->Redo();
        }
        ImGui::EndGroup();

        //Draw All Lines
        for (size_t i = 0; i < m_ladder_rungs.size(); i++) 
        {
            m_ladder_rungs[i].Draw(drawlist, i);
        }

        ImGui::Text("History: %ld , pos, %d", m_redo_history.size(), m_redo_history_pos);
        ImGui::EndChild();
    }
    ImGui::PopStyleColor();
};
