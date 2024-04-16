#include "EditorBase.hpp"
#include "UI_Icons.h"
#include "UI_Style.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "LadderElement.hpp"
#include "log.h"
#include "standartypes.h"
#include <cstddef>
#include <cstring>
#include <string>
#include "UI_Widgets.h"
#include "cJSON.h"
#include "ui_core.hpp"


    
    const char* LadderElement::ToJson()
    {

        cJSON* root = NULL;
        root = cJSON_CreateObject();

        if(!root)
            return NULL;

        cJSON_AddStringToObject(root, "Class", "LadderElement");
        cJSON_AddStringToObject(root, "UUID", m_uuid.c_str());

        cJSON_AddNumberToObject(root, "Type", m_type);
        cJSON_AddStringToObject(root, "ContactVarName", m_contact_var_name.c_str());
        cJSON_AddStringToObject(root, "BlockName", m_block_name.c_str());
        cJSON_AddStringToObject(root, "BlockInstace", m_block_instance.c_str());

        if(m_block_inputs.size() > 0)
        {
            cJSON* inputs = cJSON_CreateObject();
            cJSON_AddItemToObject(root, "BlockInputs", inputs);
            
            for (size_t i = 0;  i < m_block_inputs.size(); i++) 
            {
                cJSON *in = cJSON_CreateObject();
		        cJSON_AddItemToArray(inputs, in);

                cJSON_AddStringToObject(in, "Name", m_block_inputs[i].Name.c_str());
                cJSON_AddNumberToObject(in, "Type", m_block_inputs[i].Type);
                cJSON_AddStringToObject(in, "VarAdress", m_block_inputs[i].VariableAdress.c_str());
            }            
        }
        
        if(m_block_outputs.size() > 0)
        {
            cJSON* outputs = cJSON_CreateObject();
            cJSON_AddItemToObject(root, "BlockOutputs", outputs);
            
            for (size_t i = 0;  i < m_block_outputs.size(); i++) 
            {
                cJSON *out = cJSON_CreateObject();
		        cJSON_AddItemToArray(outputs, out);

                cJSON_AddStringToObject(out, "Name", m_block_outputs[i].Name.c_str());
                cJSON_AddNumberToObject(out, "Type", m_block_outputs[i].Type);
                cJSON_AddStringToObject(out, "VarAdress", m_block_outputs[i].VariableAdress.c_str());
            }            
        }

        char* elementJson = cJSON_Print(root);
        std::string result(elementJson);
        
        cJSON_Delete(root);
        //free(elementJson);
        
        return "";
    }

    //Draw element and Return its size
    ImVec2 LadderElement::Draw(ImDrawList* drawlist, ImVec2 pos)
    {
        ImVec2 size(0,0);

        switch (m_type) 
        {
            case CONTACT_OPEN:
            case CONTACT_CLOSE:
            case CONTACT_RISE_EDGE:
            case CONTACT_FALING_EDGE:
                size = DrawContact(drawlist,pos);
                break;
            case COIL:
            case COIL_SET:
            case COIL_RESET:
                size = DrawCoil(drawlist,pos);
                break;
            case LABEL:
            case GOTO_LABEL:
            case RETURN:
            case BRANCH_IN:
            case BRANCH_OUT:
            case BLOCK:
            case BLOCK_EN_ENO:
            default:
            break;
        }

        //Update Internal Bouding Box
        m_bbox.Min = pos;
        m_bbox.Max = ImVec2(pos.x + size.x, pos.y + size.y);

        //Register Item, Used on Drang and Drop operations
        std::string idStr = "###LadderElement_" + m_uuid;
        const ImGuiID id =  ImGui::GetCurrentWindow()->GetID(idStr.c_str());
	
        ImGui::ItemAdd(m_bbox, id);
        ImGui::ButtonBehavior(m_bbox, id, NULL, NULL, 0);   

           
        //Show Right Click Menu
        this->RigthClickMenuItens();

        //Set Drag Drop Source When Draging
	    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	    {
		    // Set payload to carry the index of our item (could be anything)
	        ImGui::SetDragDropPayload("LadderElement", this, sizeof(LadderElement),ImGuiCond_Once); 
            UI_Widgets::Label(UI_COLOR_WHITE, ImVec2(0,ImGui::GetFontSize()*2.5), UI_Widgets::TXT_LEFT, u8"%s", m_contact_var_name.c_str());
            ImGui::EndDragDropSource();
	    }

        return size;

    }

    void LadderElement::RigthClickMenuItens()
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
            //Push command Select line (uuid);
            //Push command Select Element (uuid); 

            if (ImGui::MenuItemEx(menuItemName(u8"	Cut", tmp.c), u8"	" UI_ICON_MD_CONTENT_CUT, u8"	CTRL+X", false, true))	{ ; };// { m_Editor->PushSetUserCmdByMenu(m_Editor->CMD_CUT); }
            ImGui::Dummy(ImVec2(2, 2));
            if (ImGui::MenuItemEx(menuItemName(u8"	Copy", tmp.c), u8"	" UI_ICON_MD_CONTENT_COPY, u8"	CTRL+C", false, true))	{ ; };
            ImGui::Dummy(ImVec2(2, 2));
            if (ImGui::MenuItemEx(menuItemName(u8"	Paste", tmp.c), u8"	" UI_ICON_MD_CONTENT_PASTE, u8"	CTRL+V", false, true))	{ ; };
            ImGui::Dummy(ImVec2(2, 2));
            if (ImGui::MenuItemEx(menuItemName(u8"	Delete", tmp.c), u8"	" UI_ICON_MD_DELETE, u8"	DEL", false, true))		{ ; };
            ImGui::Dummy(ImVec2(5, 5));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(5, 5));
            if (ImGui::MenuItemEx(menuItemName(u8"	Undo", tmp.c), u8"	" UI_ICON_VS_DISCARD, u8"	CTRL+Z", false, true))		{ ; };//{   m_Editor->PushSetUserCmdByMenu(m_Editor->CMD_UNDO); }
            ImGui::Dummy(ImVec2(2, 2));
            if (ImGui::MenuItemEx(menuItemName(u8"	Redo", tmp.c), u8"	" UI_ICON_VS_REDO, u8"	CTRL+Y", false, true))			{ ; };//{   m_Editor->PushSetUserCmdByMenu(m_Editor->CMD_REDO); } 
            ImGui::Dummy(ImVec2(5, 5));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(5, 5));
            if (ImGui::MenuItemEx(menuItemName(u8"	Edit", tmp.c), u8"	", u8"	", false, true)) 
            { 
                ;
            }

            ImGui::Dummy(ImVec2(5, 5));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(5, 5));
            
            if (ImGui::MenuItemEx(menuItemName(u8"	Insert CONTACT", tmp.c), u8"	", u8"	", false, true)) 
            { 
                ;
            }
            if (ImGui::MenuItemEx(menuItemName(u8"	Insert BLOCK", tmp.c), u8"	", u8"	", false, true)) 
            { 
                ;
            }
            if (ImGui::MenuItemEx(menuItemName(u8"	Add COIL", tmp.c), u8"	", u8"	", false, true)) 
            { 
                ;
            }

            if(m_type >= ElementType::CONTACT_OPEN && m_type <= ElementType::COIL_RESET)
            {
                ImGui::Dummy(ImVec2(5, 5));
                ImGui::Separator();
                ImGui::Dummy(ImVec2(5, 5));
            }

            switch (m_type) 
            {
                case ElementType::CONTACT_OPEN:
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to CLOSE", tmp.c), u8" / ", u8"", false, true))
                    {
                        //Set it for All
                        //Push Command CHANGE_ELEMENT TYPE UUID, NEW TYPE;
                        m_type = ElementType::CONTACT_CLOSE;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to RISE EDGE", tmp.c), u8" " UI_ICON_MD_ARROW_UPWARD, u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_RISE_EDGE;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to FALING EDGE", tmp.c), u8" " UI_ICON_MD_ARROW_DOWNWARD, u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_FALING_EDGE;
                    }

                break;

                case ElementType::CONTACT_CLOSE:
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to OPEN", tmp.c), u8" ", u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_OPEN;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to RISE EDGE", tmp.c), u8" " UI_ICON_MD_ARROW_UPWARD, u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_RISE_EDGE;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to FALING EDGE", tmp.c), u8" " UI_ICON_MD_ARROW_DOWNWARD, u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_FALING_EDGE;
                    }
                break;

                case ElementType::CONTACT_RISE_EDGE:
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to OPEN", tmp.c), u8"  ", u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_OPEN;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to CLOSE", tmp.c), u8" / ", u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_CLOSE;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to FALING EDGE", tmp.c), u8" " UI_ICON_MD_ARROW_DOWNWARD, u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_FALING_EDGE;
                    }
                break;

                case ElementType::CONTACT_FALING_EDGE:
                   if (ImGui::MenuItemEx(menuItemName(u8"	Switch to OPEN", tmp.c), u8"  ", u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_OPEN;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to CLOSE", tmp.c), u8" / ", u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_CLOSE;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to RISE EDGE", tmp.c), u8" " UI_ICON_MD_ARROW_UPWARD, u8"", false, true))
                    {
                        m_type = ElementType::CONTACT_RISE_EDGE;
                    }
                break;

                case ElementType::COIL:
                   if (ImGui::MenuItemEx(menuItemName(u8"	Switch to TO SET", tmp.c), u8" S ", u8"", false, true))
                    {
                        m_type = ElementType::COIL_SET;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to RESET", tmp.c), u8" R ", u8"", false, true))
                    {
                        m_type = ElementType::COIL_RESET;
                    }
                break;
                case ElementType::COIL_SET:
                   if (ImGui::MenuItemEx(menuItemName(u8"	Switch to COIL", tmp.c), u8"  ", u8"", false, true))
                    {
                        m_type = ElementType::COIL;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to RESET", tmp.c), u8" R ", u8"", false, true))
                    {
                        m_type = ElementType::COIL_RESET;
                    }
                break;
               case ElementType::COIL_RESET:
                   if (ImGui::MenuItemEx(menuItemName(u8"	Switch to COIL", tmp.c), u8"  ", u8"", false, true))
                    {
                        m_type = ElementType::COIL;
                    }
                    if (ImGui::MenuItemEx(menuItemName(u8"	Switch to SET", tmp.c), u8" S ", u8"", false, true))
                    {
                        m_type = ElementType::COIL_SET;
                    }
                break;


                default:
                break;
            }

            ImGui::EndPopup();
        }

        ImGui::PopStyleVar(6);
        ImGui::PopStyleColor();

    }


    ImVec2 LadderElement::DrawContact(ImDrawList* drawlist, ImVec2 pos)
    {
        if(!drawlist)
            return ImVec2(0,0);

        float lineThickness = 2.0;
        ImU32 txtColor = ImGui::GetColorU32(ImGuiCol_Text);
        ImU32 lineColor = ImGui::ColorConvertFloat4ToU32(UI_COLOR_WHITE);

        if(m_is_selected)
        {
            lineColor = ImGui::ColorConvertFloat4ToU32(UI_COLOR_LIGHT_BLUE);
            lineThickness += 2;
        }
        //Calc Text Size;
        ImVec2 textSize = ImGui::CalcTextSize(this->m_contact_var_name.c_str());  
        textSize.y = ImGui::GetFontSize() * 1.2;

        //Calc ContactSize
        ImVec2 BBox = ImVec2(80,(textSize.y * 3));
        if(BBox.x < textSize.x)
            BBox.x = textSize.x;
        //Add Margings
        BBox.x +=20;

        //First Draw Text
        ImVec2 txtPos = ImVec2(pos.x + ((BBox.x - textSize.x)/2), pos.y + textSize.y);
        drawlist->AddText(txtPos,txtColor, this->m_contact_var_name.c_str());

        if(m_is_selected)
        {
            if(ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringRect(m_bbox.Min,m_bbox.Max) && ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
            {
                
            }
        }


        //Draw Open Contact
        ImVec2 vertBarStart = ImVec2(pos.x + (BBox.x / 2 - textSize.y/2), pos.y + textSize.y * 2);
        ImVec2 vertBarEnd   = ImVec2(vertBarStart.x, vertBarStart.y + textSize.y);

        ImVec2 horBarStart  = ImVec2(pos.x, pos.y + textSize.y * 2.5);
        ImVec2 horBarEnd    = ImVec2(vertBarStart.x, horBarStart.y);

        drawlist->AddLine(horBarStart, horBarEnd , lineColor, lineThickness/2); // Draw First PArt of Contact -|
        drawlist->AddLine(vertBarStart, vertBarEnd, lineColor, lineThickness); 

        vertBarStart.x  += textSize.y;
        vertBarEnd.x    += textSize.y;        
        
        horBarStart.x = vertBarStart.x;
        horBarEnd.x =  pos.x + BBox.x;

        drawlist->AddLine(horBarStart, horBarEnd , lineColor,lineThickness/2); // Draw Second part of Contact |-
        drawlist->AddLine(vertBarStart, vertBarEnd, lineColor,lineThickness); 

        //Draw Special contacts
        ImVec2 specialContactPos = ImVec2(horBarStart.x - textSize.y,vertBarStart.y);
        switch (this->m_type) 
        {
            case LadderElement::CONTACT_CLOSE:
                //Draw Angle Bar
                vertBarStart.x  -= (textSize.y -2);
                vertBarEnd.x    -= 2;
                drawlist->AddLine(vertBarStart, vertBarEnd, lineColor,lineThickness/2);
                break;
            case LadderElement::CONTACT_RISE_EDGE:
                //Draw Up Arrow
                drawlist->AddText(specialContactPos,lineColor, UI_ICON_MD_ARROW_UPWARD);
                break;
            case LadderElement::CONTACT_FALING_EDGE:
                //Draw Down Arrow
                drawlist->AddText(specialContactPos,lineColor, UI_ICON_MD_ARROW_DOWNWARD);
                
                break;
            default:
            break;
        }

       return BBox;
    }

    ImVec2 LadderElement::DrawCoil(ImDrawList* drawlist, ImVec2 pos)
    {
        if(!drawlist)
            return ImVec2(0,0);

        float lineThickness = 2.0;
        ImU32 txtColor = ImGui::GetColorU32(ImGuiCol_Text);
        ImU32 lineColor = ImGui::ColorConvertFloat4ToU32(UI_COLOR_WHITE);

        if(m_is_selected)
        {
            lineColor = ImGui::ColorConvertFloat4ToU32(UI_COLOR_LIGHT_BLUE);
            lineThickness += 2;
        }


        //Calc Text Size;
        ImVec2 textSize = ImGui::CalcTextSize(this->m_contact_var_name.c_str());
        textSize.y = ImGui::GetFontSize() * 1.2;

        //Calc ContactSize
        ImVec2 BBox = ImVec2(80,(textSize.y * 3));
        if(BBox.x < textSize.x)
            BBox.x = textSize.x;
        //Add Margings
        BBox.x +=20;

        //First Draw Text
        ImVec2 txtPos = ImVec2(pos.x + (BBox.x- textSize.x)/2, pos.y + textSize.y);
        drawlist->AddText(txtPos,txtColor, this->m_contact_var_name.c_str());

        //Draw Open Contact
        ImVec2 vertBarStart = ImVec2(pos.x + (BBox.x / 2 - textSize.y/2), pos.y + textSize.y * 2);
        ImVec2 vertBarEnd   = ImVec2(vertBarStart.x, vertBarStart.y + textSize.y);

        ImVec2 horBarStart  = ImVec2(pos.x, pos.y + textSize.y * 2.5);
        ImVec2 horBarEnd    = ImVec2(vertBarStart.x - lineThickness, horBarStart.y);

        ImVec2 horBarCenter  = ImVec2(horBarEnd.x + textSize.y, horBarEnd.y + 2);

        drawlist->AddLine(horBarStart, horBarEnd , lineColor,lineThickness/2); // Draw First PArt of Contact -(
        
        drawlist->PathArcTo(horBarCenter, textSize.y, 2.60, 3.82,textSize.y); // 220º to 330 º
        drawlist->PathStroke(lineColor,0,lineThickness);

        vertBarStart.x  += textSize.y;
        vertBarEnd.x    += textSize.y;        
        
        horBarStart.x = vertBarStart.x;
        horBarEnd.x =  pos.x + BBox.x;

        horBarCenter.x -= textSize.y;

        drawlist->AddLine(horBarStart, horBarEnd , lineColor,lineThickness/2); // Draw Second part of Contact )-
        drawlist->PathArcTo(horBarCenter, textSize.y, 0.55,-0.65,textSize.y); // 30º to 150 º
        drawlist->PathStroke(lineColor,0,lineThickness);


        //Draw Special contacts
        ImVec2 specialContactPos = ImVec2(horBarStart.x - textSize.y + 2, vertBarStart.y);
        switch (this->m_type) 
        {
            case LadderElement::COIL_SET:
                //Draw Up Arrow
                drawlist->AddText(specialContactPos,lineColor, "S");
                break;
            case LadderElement::COIL_RESET:
                //Draw Down Arrow
                drawlist->AddText(specialContactPos,lineColor, "R");
                break;
            default:
            break;
        }

       return BBox;
    }