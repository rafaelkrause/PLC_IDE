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
constexpr const int 	LADDER_MAXIMUM_UNDO_HISTORY 		= 50;

	
void EditorPouLadder::PushCommand(EditorPouLadderCommand* _cmd)
{
    _cmd->Execute(this);
    m_undo_history.push_back(_cmd);

    if(m_redo_history.size() > 0)
    {
        for (auto r : m_redo_history) 
            free(r);
        m_redo_history.clear();
    }

    if(m_undo_history.size() > LADDER_MAXIMUM_UNDO_HISTORY)
    {
        delete m_undo_history[0];
        m_undo_history.erase(m_undo_history.begin());
    }
}

void EditorPouLadder::Undo()
{
    if(m_undo_history.size() > 0)
    {
        auto cmd = m_undo_history.at(m_undo_history.size()-1);
        m_undo_history.pop_back();
        cmd->Undo(this);
        m_redo_history.push_back(cmd);
    }
}

void EditorPouLadder::Redo()
{
    if(m_redo_history.size() > 0)
    {
        auto cmd = m_redo_history.at(m_redo_history.size()-1);
        m_redo_history.pop_back();
        cmd->Redo(this);
        m_undo_history.push_back(cmd);
    }
}

void EditorPouLadder::Remove()
{ 
    //Remove Only Rungs
    if(m_selected_rungs.size() > 0 && m_selected_ladder_elements.size() == 0)
        this->PushCommand(new CommandRemoveRung());
    else if(m_selected_ladder_elements.size() > 0)
        this->PushCommand(new CommandRemoveElements());

}

void EditorPouLadder::Insert()
{
    //Remove Only Rungs
    if(m_selected_ladder_elements.size() == 0)
        this->PushCommand(new CommandInsertElements());
    else
        this->PushCommand(new CommandAddRung());
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

void EditorPouLadder::UpdateRungByUUID(std::string& _UUID, LadderRung& _rung)
{
    for (size_t i = 0; i < m_ladder_rungs.size(); i++) {

        if(m_ladder_rungs[i].GetUUID() == _UUID)
        {
            m_ladder_rungs[i] = _rung;
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
            m_ladder_rungs[i].Draw(drawlist, i, true);
        }

        //ImGui::Text("History: %ld , pos, %d", m_redo_history.size(), m_redo_history_pos);
        ImGui::EndChild();
    }
    ImGui::PopStyleColor();
};


void EditorPouLadder::ProcessKeyBoardCommands()
{
    if(!ImGui::IsWindowFocused())
        return;

    //DEL - Delet Selected Rungs or Elements
    if(ImGui::IsKeyPressed(ImGuiKey_Delete,false))
        this->Remove();

    //INSERT - Insert New Rung
    if(ImGui::IsKeyPressed(ImGuiKey_Insert,false))
        this->Insert();

    //CTRL Comands
    if(ImGui::IsKeyPressed(ImGuiKey_LeftCtrl) || ImGui::IsKeyPressed(ImGuiKey_RightCtrl))
    {
        if(ImGui::IsKeyPressed(ImGuiKey_Z, false))
            this->Undo(); 

        if(ImGui::IsKeyPressed(ImGuiKey_Y, false))
            this->Redo(); 

        if(ImGui::IsKeyPressed(ImGuiKey_C, false))
            this->Copy();
        
        if(ImGui::IsKeyPressed(ImGuiKey_V, false))
            this->Paste();
        
        if(ImGui::IsKeyPressed(ImGuiKey_X, false))
            this->Cut();

        if(ImGui::IsKeyPressed(ImGuiKey_Home, false))
            this->MoveTop();
        
        if(ImGui::IsKeyPressed(ImGuiKey_End, false))
            this->MoveBotton();
    
    }


}