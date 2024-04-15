
#ifndef EDITOR_POUP_LADDER_COMMAND_LIB_H
#define EDITOR_POUP_LADDER_COMMAND_LIB_H

#include "EditorPouLadder.hpp"
#include "LadderRung.hpp"
#include <cstddef>
#include <string>
#include <vector>


//Rungs Commands
// Add
// Remove
// Move
// Paste (Same as Add)
// Edited
//      Any kind of Edited (Title, Description, Contacts, Variables etc...)
//      When Edited, save a deep copy of Rung, as is to complique make undo from OR elements, as it can be place any where on rung
//

// Ladder Elements
// Add
// Remove
// Move
// Paste (same as Add)
// Edited




class CommandAddRung : public EditorPouLadder::EditorPouLadderCommand
{

public:

    CommandAddRung() 
    {
        LadderRung  m_rung;
        m_pos = 0;
    };
    ~CommandAddRung() {;};

    void Execute(EditorPouLadder *_editor) override
    {
        if(!_editor)
            return;

        //Get Last Selected Rung IDX
        auto list = _editor->GetSelectedRungsUUIDs();

        if(list.size() > 0)
        {
            auto lastRungUUID = list.back();
            m_pos = _editor->GetRungIdxByUUID(lastRungUUID);
            _editor->InsertRung(m_rung,m_pos);
        }
        else 
        {
            _editor->AddRung(m_rung);
            m_pos = _editor->GetRungIdxByUUID(m_rung.GetUUID());
        }
        //Inset New Rung After Selected
        
        LOG_ERROR("","Execute: Inseted Rung UUID: %s, pos: %d",m_rung.GetUUID().c_str(), m_pos);
        
        m_can_undo = true;
        m_can_redo = false;
    };

    void Undo(EditorPouLadder *_editor) override
    {
        if(!_editor || !m_can_undo)
            return;
        
        m_rung = _editor->GetRungByUUID(m_rung.GetUUID());
        m_pos = _editor->GetRungIdxByUUID(m_rung.GetUUID());
        _editor->RemoveRungByUUID(m_rung.GetUUID());
        LOG_ERROR("","Undo: Remove UUID: %s %d",m_rung.GetUUID().c_str(), m_pos);

        m_can_undo = false;
        m_can_redo = true;
    };

    void Redo(EditorPouLadder *_editor) override
    {
        if(!_editor || m_pos < 0 || !m_can_redo)
            return;

        _editor->InsertRung(m_rung, m_pos);
        LOG_ERROR("","Redo: Inserted UUID: %s at %d",m_rung.GetUUID().c_str(), m_pos);

        m_can_undo = true;
        m_can_redo = false;
    };

private:
    LadderRung  m_rung;
    int         m_pos;
};

class CommandRemoveRung : public EditorPouLadder::EditorPouLadderCommand
{
    public:
    struct UndoData
    {
        int      RungIdx;
        LadderRung  Rung;
    };

    CommandRemoveRung()
    {

    }
    ~CommandRemoveRung()
    {
        
    }
    
    void Execute(EditorPouLadder *_editor) override
    {
        if(_editor)
        {
            auto rungs = _editor->GetSelectedRungsUUIDs();
            for (size_t i; i < rungs.size(); i++) {
                
                std::string r = rungs[i];
                
                //Make a Copy if Rung
                UndoData d;
                d.RungIdx = _editor->GetRungIdxByUUID(r);
                d.Rung    = _editor->GetRungByUUID(r);
                m_removed_rungs.push_back(d);
                _editor->RemoveRungByUUID(r);
            }        
        }
    };

    void Undo(EditorPouLadder *_editor) override
    {
        if(_editor)
        {
            for (size_t i = 0; i < m_removed_rungs.size(); i++) {
                auto d = m_removed_rungs[i];
                _editor->InsertRung(d.Rung, d.RungIdx);
            }
        }

    };

    void Redo(EditorPouLadder *_editor) override
    {
        if(_editor)
        {
            for (size_t i = 0; i < m_removed_rungs.size(); i++) {
                auto d = m_removed_rungs[i];
                _editor->RemoveRungByPos(d.RungIdx);
            }
        }
    }

    private:

    std::vector<UndoData> m_removed_rungs;
};


#endif