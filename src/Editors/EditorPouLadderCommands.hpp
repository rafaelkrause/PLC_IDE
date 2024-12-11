
#ifndef EDITOR_POUP_LADDER_COMMAND_LIB_H
#define EDITOR_POUP_LADDER_COMMAND_LIB_H

#include "EditorPouLadder.hpp"
#include "LadderElement.hpp"
#include "LadderRung.hpp"
#include <cstddef>
#include <string>
#include <vector>


//Rungs Commands
// Add
// Remove
// Move
// Paste 
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
        
    };

    void Undo(EditorPouLadder *_editor) override
    {
        if(!_editor)
            return;
        
        m_rung = _editor->GetRungByUUID(m_rung.GetUUID());
        m_pos = _editor->GetRungIdxByUUID(m_rung.GetUUID());
        _editor->RemoveRungByUUID(m_rung.GetUUID());
        LOG_ERROR("","Undo: Remove UUID: %s %d",m_rung.GetUUID().c_str(), m_pos);
    };

    void Redo(EditorPouLadder *_editor) override
    {
        if(!_editor || m_pos < 0)
            return;

        _editor->InsertRung(m_rung, m_pos);
        LOG_ERROR("","Redo: Inserted UUID: %s at %d",m_rung.GetUUID().c_str(), m_pos);
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

class CommandAddElement : public EditorPouLadder::EditorPouLadderCommand
{

public:

    CommandAddElement() {;};
    CommandAddElement(LadderElement::ElementType _type)
    {
        m_element.SetType(_type);
    }

    ~CommandAddElement() {;};

    void Execute(EditorPouLadder *_editor) override
    {    
        if(_editor)
            {
                auto rungs = _editor->GetSelectedRungsUUIDs();
                if(rungs.size() == 1)
                {
                    auto elements =_editor->GetSelectedElementsUUIDs();

                    //Save Original rung
                    m_rung_original = _editor->GetRungByUUID(rungs[0]);

                    //Save New State
                    m_rung_changed =  _editor->GetRungByUUID(rungs[0]);
                    
                    if(elements.size() > 0 )
                    {
                        auto e = m_rung_changed.GetElementIdxByUUID(elements[0]);
                        m_rung_changed.InsertElement(m_element);
                    }
                    else                 
                    {    
                        m_rung_changed.AddElement(m_element);
                    }
                }       
            }
    };

    void Undo(EditorPouLadder *_editor) override
    {
    };

    void Redo(EditorPouLadder *_editor) override
    {
    };

private:
    LadderElement   m_element;
    LadderRung      m_rung_original;
    LadderRung      m_rung_changed;
};

class CommandRemoveElements: public EditorPouLadder::EditorPouLadderCommand
{
    public:

    CommandRemoveElements() {;}
    ~CommandRemoveElements() {;}
    
    void Execute(EditorPouLadder *_editor) override
    {
        if(_editor)
        {
            auto rungs = _editor->GetSelectedRungsUUIDs();
            if(rungs.size() == 1)
            {
                auto elements =_editor->GetSelectedElementsUUIDs();

                if(elements.size() > 0 )
                {
                    //Save Original rung
                    m_rung_original = _editor->GetRungByUUID(rungs[0]);

                    //Save New State
                    m_rung_changed =  _editor->GetRungByUUID(rungs[0]);

                    //Remove Elements
                    for (size_t i; i < elements.size(); i++)
                        m_rung_changed.RemoveElement(elements[i]);

                    //Update rung on Editor
                    _editor->UpdateRungByUUID(rungs[0], m_rung_changed);

                }
            }       
        }
    };

    void Undo(EditorPouLadder *_editor) override
    {
        if(_editor)
            _editor->UpdateRungByUUID(m_rung_original.GetUUID(), m_rung_original);

    };

    void Redo(EditorPouLadder *_editor) override
    {
        if(_editor)
            _editor->UpdateRungByUUID(m_rung_changed.GetUUID(), m_rung_changed);
    }

    private:

    LadderRung m_rung_original;
    LadderRung m_rung_changed;
};


#endif