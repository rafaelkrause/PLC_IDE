#ifndef LADDER_ELEMENT_LIB_H
#define LADDER_ELEMENT_LIB_H

#include "standartypes.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "uuid_v4.h"
#include <string>
#include <vector>

class LadderElement
{

public:

    enum ElementType
    {
        CONTACT_OPEN,
        CONTACT_CLOSE,
        CONTACT_RISE_EDGE,
        CONTACT_FALING_EDGE,
        COIL,
        COIL_SET,
        COIL_RESET,
        LABEL,
        GOTO_LABEL,
        RETURN,
        BRANCH_IN,
        BRANCH_OUT,
        BLOCK,
        BLOCK_EN_ENO
    };


    struct BlockPin
    {
        enum PinType
        {
            PIN_CONST_BOOL,
            PIN_CONST_INT,
            PIN_CONST_FLOAT,
            PIN_CONST_STRING,
            PIN_CONST_TIMER,
            PIN_VARIABLE
        };

        std::string Name;
        PinType     Type;
        std::string VariableAdress;     //Used for Variables and Constants
    };


    LadderElement() 
    {
        UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
        m_uuid = uuidGenerator.getUUID().str();
        m_type = ElementType::CONTACT_OPEN;
        
        m_contact_var_name = "???";
        m_block_name = "???";
        m_block_instance = "???";
        
        m_is_selected = false;
        m_is_dragging = false;
        m_is_drop_over = false;

        ImRect m_bbox = ImRect();

    }

    LadderElement(ElementType type)
    {
        UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;
        m_uuid = uuidGenerator.getUUID().str();
        m_type = type;
        
        m_contact_var_name = "???";
        m_block_name = "???";
        m_block_instance = "???";
        
        m_is_selected = false;
        m_is_dragging = false;
        m_is_drop_over = false;

        ImRect m_bbox = ImRect();


    }
    
    ~LadderElement()
    {
        ;
    }

   const char* ToJson();

    std::string GetUUID()                           { return m_uuid;   }

    ElementType GetType()                           { return m_type;   }
    void        SetType(ElementType type)           { m_type = type;   }


    //Contact Data
    std::string GetContactVarName()                { return m_contact_var_name;}
    void SetContactVarName(const char* name)       { if(name)m_contact_var_name.assign(name);else m_contact_var_name.clear();}

    //Block Data
    void SetBlockName(const char* name)             { if(name)m_block_name.assign(name);else m_block_name.clear();}
    std::string GetBlockName()                      { return m_block_name;}

    std::string GetBlockInstace()                   { return m_block_instance;}
    void SetBlockInstance(const char* instance)     { if(instance)m_block_instance.assign(instance);else m_block_instance.clear();}

    
    void AddBlockInputPin(BlockPin in_pin);
    void AddBlockOutputPin(BlockPin out_pint);
    std::vector<BlockPin> GetInputPins();
    std::vector<BlockPin> GetOutPins();

    //Drawing - Return Pos + BBox
    ImVec2 Draw(ImDrawList* drawlist, ImVec2 pos);
    
    //User Interaction
    ImRect GetBoundBox()                            { return m_bbox;  }  
    
    bool IsSelected()                               { return  m_is_selected; }
    void SetAsSelected(bool selected)               {  m_is_selected = selected; }

    bool IsClicked()                                
    { 
        if(ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringRect(m_bbox.Min,m_bbox.Max) && ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
            return true;
        else
            return false;
    }

    bool IsDragging()                               { return  m_is_dragging;  }
    void SetIsDragin(bool is_drag)                  { m_is_dragging = is_drag; }

    bool IsDropOver()                               { return  m_is_drop_over; }
    

private:

    ImVec2 DrawContact(ImDrawList* drawlist, ImVec2 pos);
    ImVec2 DrawCoil(ImDrawList* drawlist, ImVec2 pos);
    
    void RigthClickMenuItens();

	inline const char* menuItemName(const char* label, char* buf)
	{
		sprintf(buf,u8"%s###RightClickMenuItem_%s", label, this->m_uuid.c_str());
		return buf;
	}

    std::string m_uuid;
    ElementType m_type;

    std::string m_contact_var_name;
    
    std::string m_block_name;
    std::string m_block_instance;

    std::vector<BlockPin> m_block_inputs;
    std::vector<BlockPin> m_block_outputs;

    bool    m_is_selected;
    bool    m_is_dragging;
    bool    m_is_drop_over;

    std::string m_json_str;
    ImRect m_bbox;

};



#endif