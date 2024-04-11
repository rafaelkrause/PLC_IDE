#ifndef IEC_PROJECT_PROGRMA_LIB_H
#define IEC_PROJECT_PROGRMA_LIB_H

//#include <pstl/glue_algorithm_defs.h>
#include <vector>
#include <string>
#include <algorithm>
#include "Object.hpp"



class IecProject
{
public:

	bool OpenProject(const char* path)	{ return false; };
	bool SaveProject()					{ return false; };
	bool SaveProject(const char* path)	{ return false; };

	PlcEditorObject* GetChildren(int i) { return m_objects[i]; }
	int	GetNumberOfChildren()			{ return m_objects.size(); }
	const char* GetName()				{ return m_name.c_str(); }

	std::vector<PlcEditorObject*> GetObjectsOpened() { return m_editors_opened; }

	void PushOpenObject(PlcEditorObject* obj) 
	{
		if(!obj || !obj->GetEditor())
			return;

		if (m_editors_opened.empty() || !(std::find(m_editors_opened.begin(), m_editors_opened.end(), obj) != m_editors_opened.end()))
		{
			m_editors_opened.push_back(obj);
			SetActiveEditor(obj);
		}

	}

	void PopOpenObject(PlcEditorObject* obj) 
	{
		if(!obj || !obj->GetEditor())
			return;
 	
		auto found = std::find(m_editors_opened.begin(),m_editors_opened.end(), obj);
    	if (found != m_editors_opened.end()) { m_editors_opened.erase(found); }

		if(m_editors_opened.size() == 0)
		{
			ClearActiveEditor();
		}
		else
		{
			int editorIndex =  found - m_editors_opened.begin();
			editorIndex--;
			if(editorIndex > 0 )
				SetActiveEditor(m_editors_opened.at(editorIndex));
			else
				SetActiveEditor(m_editors_opened.at(0));
			

		 }

	}

	void CloseOpenAllObjects()
	{
		ClearActiveEditor();
		m_editors_opened.clear();
	}

	void CloseRigthOpenObjects(PlcEditorObject* obj)
	{
		if(!obj)
			return;

		SetActiveEditor(obj);

		auto found = std::find(m_editors_opened.begin(),m_editors_opened.end(), obj);

		if(found != m_editors_opened.end())
		{
			m_editors_opened.erase(found + 1,m_editors_opened.end());
		}

	}

	void CloseOthersObjects(PlcEditorObject* obj)
	{
		if(!obj)
			return;

		SetActiveEditor(obj);
		m_editors_opened.erase(std::remove_if(m_editors_opened.begin(), m_editors_opened.end(), [&](PlcEditorObject* p) { return p != obj; }), m_editors_opened.end());
	}

	PlcEditorObject* GetActiveEditor() { return m_activeEditor; }
	
	void SetActiveEditor(PlcEditorObject* obj) 
	{
		if(!obj)
			return;
		if (!m_editors_opened.empty() && std::find(m_editors_opened.begin(), m_editors_opened.end(), obj) != m_editors_opened.end())
		{
			m_activeEditor = obj;
		}
	}
	
	void ClearActiveEditor()
	{
		m_activeEditor = NULL;
	}

	IecProject();
	~IecProject();


private:

	bool			m_IsSaved;
	std::string		m_name;
	std::vector<PlcEditorObject*> m_objects;
	std::vector<PlcEditorObject*> m_editors_opened;
	PlcEditorObject* m_activeEditor;

};

#endif


