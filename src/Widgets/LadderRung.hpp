#ifndef LADDER_RUNG_LIB_H
#define LADDER_RUNG_LIB_H

#include "imgui.h"
#include "imgui_internal.h"
#include "standartypes.h"
#include "UI_Widgets.h"
#include "UI_Style.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iterator>
#include <string>
#include <system_error>
#include <vector>
#include "LadderElement.hpp"
#include "log.h"
#include "ui_core.hpp"
#include "cJSON.h"

class LadderRung
{


public:
	LadderRung()
    {
        UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
	    m_uuid = uuidGenerator.getUUID().str();

        m_tile.c[0] = '\0';
        m_description.c[0] = '\0';
        m_editing_description = false;
        m_editing_tile = false;

        m_elements.push_back(LadderElement(LadderElement::ElementType::CONTACT_OPEN));
        m_elements.push_back(LadderElement(LadderElement::ElementType::CONTACT_CLOSE));
        m_elements.push_back(LadderElement(LadderElement::ElementType::CONTACT_RISE_EDGE));
        m_elements.push_back(LadderElement(LadderElement::ElementType::CONTACT_FALING_EDGE));
        m_elements.push_back(LadderElement(LadderElement::ElementType::COIL));
        m_elements.push_back(LadderElement(LadderElement::ElementType::COIL_SET));
        m_elements.push_back(LadderElement(LadderElement::ElementType::COIL_RESET));
    }

    LadderRung(std::string& _UUID)
    {
        m_uuid = _UUID;
        m_tile.c[0] = '\0';
        m_description.c[0] = '\0';
        m_editing_description = false;
        m_editing_tile = false;

        m_elements.push_back(LadderElement(LadderElement::ElementType::CONTACT_OPEN));
        m_elements.push_back(LadderElement(LadderElement::ElementType::CONTACT_CLOSE));
        m_elements.push_back(LadderElement(LadderElement::ElementType::CONTACT_RISE_EDGE));
        m_elements.push_back(LadderElement(LadderElement::ElementType::CONTACT_FALING_EDGE));
        m_elements.push_back(LadderElement(LadderElement::ElementType::COIL));
        m_elements.push_back(LadderElement(LadderElement::ElementType::COIL_SET));
        m_elements.push_back(LadderElement(LadderElement::ElementType::COIL_RESET));
    }

    ~LadderRung()
    {
        ;
    }

    void AddElement(int col_pos, int level);
    void RemoveElement(std::string& uuid)
    {
        for (size_t i; i < m_elements.size(); i++) 
        {
            if(m_elements[i].GetUUID() == uuid)
            {
                m_elements.erase(m_elements.begin() + i);
                break;
            }
        }
    }
    std::string& GetUUID()      { return m_uuid;}
    ImRect& GetBBox()           { return m_bbox;}


    void Draw(ImDrawList* _drawlist, int _line_index, bool _isSelected)
    {     

        if(!_drawlist)
            return ;

        //Save Current cursor position
        ImVec2 originalCursorPos = ImGui::GetCursorScreenPos();
        
        //Give some Space for Line Adorners 
        ImGui::Dummy(ImVec2(50,10));

        //Rung Title and Description
        ImGui::SameLine();
        ImGui::BeginGroup();
        UI_Style::SetFont(UI_Style::DEFAULT);
        float textHeight = ImGui::GetFontSize() * 1.8;
        ImVec2 headerSize = ImVec2(ImGui::GetContentRegionAvail().x, textHeight);


        //Rung Title
        //-------------------------------------
        UI_Style::SetFont(UI_Style::DEFAULT_BOLD);
        if(!m_editing_tile)
        {
            UI_Widgets::VarDisplay(UI_COLOR_BG_LEVEL_5, UI_COLOR_WHITE, headerSize, UI_Widgets::TXT_LEFT, true, u8"%s",m_tile.c); 
        }
        else
        {
            string_t name;
            sprintf(name.c,"###rung_%s_title_editing",m_uuid.c_str());
            if(ImGui::InputText(name.c, m_tile.c, sizeof(m_tile)-1,ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
                m_editing_tile = false;

        }

        if(ImGui::IsItemClicked())
            m_editing_tile = true;

        if(!ImGui::IsItemClicked() && ImGui::IsMouseClicked(0))
            m_editing_tile = false;

        //Rung Description
        //-------------------------------------
        UI_Style::SetFont(UI_Style::DEFAULT);
        if(!m_editing_description )
        {
            ImGui::Text(u8"%s", strlen(m_description.c) > 0 ? m_description.c : "Description:");
        }
        else
        {
            string_t name;
            sprintf(name.c,"###rung_%s_title_editing",m_uuid.c_str());
            if(ImGui::InputTextMultiline(name.c, m_description.c, sizeof(m_description.c)-1,headerSize,ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CtrlEnterForNewLine))
                m_editing_description = false;
        }

        if(ImGui::IsItemClicked())
            m_editing_description = true;

        if(!ImGui::IsItemClicked() && ImGui::IsMouseClicked(0))
            m_editing_description = false;

        //Draw Rung Elements (Ladder)
        //-------------------------------------
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 startPos = pos;

        float rungMaxY = 0;
        
        float fontHeigt = ImGui::GetFontSize() * 1.2;

        int elementIdx = 0;
        for (auto& r : m_elements) 
        {
            ImVec2 bb = r.Draw(_drawlist,pos);              
            
            //Clear Elements Selection if Line not Selected
            if(!_isSelected)
                 r.SetAsSelected(false);
            
            //check Element Selection
            if(r.IsClicked() && _isSelected)
            {
                if(!ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl) && !ImGui::IsKeyDown(ImGuiKey::ImGuiKey_RightCtrl))
                    ClearSelectedElements();
                
                r.SetAsSelected(true);
            }   
           
            ImGuiPayload& payload = ImGui::GetCurrentContext()->DragDropPayload;
            if (payload.IsDataType("LadderElement"))
            {
                //Calc Drop Points
                ImVec2 leftRectP1 = ImVec2(pos.x-5,pos.y+fontHeigt*2.5 - 5);
                ImVec2 leftRectP2 = ImVec2(leftRectP1.x + 10,leftRectP1.y + 10);
                ImRect leftRect(leftRectP1,leftRectP1);


                ImVec2 rigthRectP1 = ImVec2(pos.x + bb.x - 5,pos.y+fontHeigt * 2.5 - 5);
                ImVec2 rigthRectP2 = ImVec2(rigthRectP1.x + 10,rigthRectP1.y + 10);
                ImRect rigthRect(rigthRectP1,rigthRectP1);
                
                //Draw Drop Points;
                if(ImGui::IsMouseHoveringRect(leftRectP1, leftRectP2))
                    _drawlist->AddRectFilled(leftRectP1, leftRectP2, ImGui::ColorConvertFloat4ToU32(UI_COLOR_LIGHT_YELLOW));                
                else
                    _drawlist->AddRectFilled(leftRectP1, leftRectP2, ImGui::ColorConvertFloat4ToU32(UI_COLOR_LIGHT_BLUE));                
                
                _drawlist->AddRect(leftRectP1, leftRectP2, ImGui::ColorConvertFloat4ToU32(UI_COLOR_LIGHT_GRAY));  

                //Drop Released
                if (ImGui::IsMouseHoveringRect(leftRectP1, leftRectP2) && !ImGui::IsMouseDown(0) && payload.Data && payload.DataSize == sizeof(LadderElement))
                {
                    //Copy Ladder Element
                    LadderElement leCopy = LadderElement(LadderElement::ElementType::CONTACT_OPEN);
                    leCopy = *(LadderElement*)payload.Data;

                    leCopy.SetAsSelected(false);

                    //Insert into Rung
                    std::vector<LadderElement>::iterator it = m_elements.begin();
                    std::advance(it, elementIdx);
                    m_elements.insert(it,leCopy);
                    
                    //Mark Element as Hide (delete)
                    //((LadderElement*)payload.Data)->
                    //Clear Payload;
                    payload.Clear();

                    // Add Core comman
                    // app.cmd(Copy_delete, editor, paramters)

                }
            }

            //Update Draw Position
            pos.x += bb.x;
            rungMaxY = std::max(rungMaxY,bb.y);

            elementIdx++;
        }

        //If no Elements, draw Empty line
        if(m_elements.size() <= 0)
        {
            _drawlist->AddLine(ImVec2(pos.x, pos.y + fontHeigt * 2.5), ImVec2(pos.x + 40, pos.y + fontHeigt * 2.5), ImGui::ColorConvertFloat4ToU32(UI_COLOR_WHITE),2.0);
        }

        ImGui::EndGroup();
       

        //Draw Line Divisor
        _drawlist->AddLine(ImVec2(originalCursorPos.x + 45, originalCursorPos.y), ImVec2(originalCursorPos.x + 45, pos.y + rungMaxY+fontHeigt*2),ImGui::GetColorU32(ImGuiCol_TextDisabled),1.0);

        //Draw Line Number
        char lineNumber[20];
        sprintf(lineNumber, u8"%d", _line_index + 1);
        ImVec2 lnNumberSize = ImGui::CalcTextSize(lineNumber);
        _drawlist->AddText(ImVec2(originalCursorPos.x + 40 - lnNumberSize.x,originalCursorPos.y+5), -_isSelected ? ImGui::GetColorU32(ImGuiCol_Text) : ImGui::GetColorU32(ImGuiCol_TextDisabled),lineNumber);
       
         //Horizontal Line
        //drawlist->AddLine(ImVec2(originalCursorPos.x + 25, originalCursorPos.y), ImVec2(originalCursorPos.x +40, originalCursorPos.y + pos.y+ rungMaxY + fontHeigt), ImGui::ColorConvertFloat4ToU32(UI_COLOR_WHITE),2);

        //Add Rug Start
        _drawlist->AddLine(ImVec2(startPos.x, startPos.y + fontHeigt), ImVec2(startPos.x, startPos.y + rungMaxY + fontHeigt), ImGui::ColorConvertFloat4ToU32(UI_COLOR_WHITE),2);


        //Clear Elements if Clicked on Line Identification Area
        if(_isSelected && ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringRect(originalCursorPos, ImVec2(originalCursorPos.x + 45, pos.y + rungMaxY+fontHeigt*2)))
            ClearSelectedElements();

        //Update Cursor position
        ImGui::SetCursorScreenPos(ImVec2(originalCursorPos.x,startPos.y + rungMaxY + fontHeigt*2));


        m_bbox = ImRect(originalCursorPos, ImVec2(pos.x, pos.y + rungMaxY + fontHeigt*2));
  
    }

    const char* ToJson()
    {
        std::string result = "{ ";
        result + "\"UUID\": \"" + m_uuid + "\", \n"; 
        result + "\"Title\": \"" + m_tile.c + "\", \n"; 
        result + "\"Description\": \"" + m_description.c + "\", \n"; 
        result + "\"Elements\": \n [ ";
        for (auto& e : m_elements) 
            result + "{ " + e.ToJson() + " }, \n";

        result + " ]} ";

        LOG_ERROR("","Rung to Json: %s", result.c_str());
        return result.c_str();
    }


private:

    void ClearSelectedElements()
    {
        for (auto& r : m_elements) 
            r.SetAsSelected(false);
    }


    std::string m_uuid;
    string_t    m_tile;
    string_t    m_description;
    
    bool        m_editing_description;
    bool        m_editing_tile;

    ImRect      m_bbox;

    std::vector<LadderElement> m_elements;
};


#endif