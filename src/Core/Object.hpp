#ifndef PROJECT_OBJECT_LIB_H
#define PROJECT_OBJECT_LIB_H

#include <vector>
#include <string>
#include "EditorBase.hpp"


class PlcEditorObject
{
public:

	enum ObjType
	{
		OBJ_TYPE_APPLICATION,
		OBJ_TYPE_FOLDER,
		OBJ_TYPE_TEXT_FILE,
		OBJ_TYPE_PLC_PROJECT,
		OBJ_TYPE_PLC_POUP_PROGRAM,
		OBJ_TYPE_PLC_POUP_PROGRAM_LADDER,
		OBJ_TYPE_PLC_POUP_FUNCTION,
		OBJ_TYPE_PLC_POUP_FUNCTION_BLOCK,
		OBJ_TYPE_PLC_DUT_ENUM,
		OBJ_TYPE_PLC_DUT_STRUCT,
		OBJ_TYPE_PLC_GLOBAL_VAR_LIST,
		OBJ_TYPE_PLC_GLOBAL_VAR_LIST_PERSIST,
		OBJ_TYPE_MOTION_CONTROLLER,
		OBJ_TYPE_MOTION_CONTROLLER_AXIS,
		OBJ_TYPE_ETHERCAT_MASTER,
		OBJ_TYPE_ETHERCAT_SALVES,
		OBJ_TYPE_ETHERCAT_MODULES,
		OBJ_TYPE_MODBUS_SERVER,
		OBJ_TYPE_MODBUS_CLIENTS,
		OBJ_TYPE_MODBUS_CLIENTS_NODE,

	};


	const char*		GetUUID()							{ return m_uuid.c_str();}
	const char*		GetName()							{ return m_name.c_str(); }
	void			SetName(const char* name)			{ m_name = name; }
	ObjType			GetType()							{ return m_type; }
	const char*		GetTypeAsString();
	void			RigthClickMenuItens();
	PlcEditorObject* GetChildren(int i)					{ return m_childrens[i]; }
	int				GetNumberOfChilderns()				{ return m_childrens.size(); }
	bool			RemoveChildren(int idx)				{ m_childrens.erase(m_childrens.begin() + idx); return true;}
	void			AddChildren(PlcEditorObject* child)	{ m_childrens.push_back(child); }
	EditorBase*		GetEditor()							{ return m_Editor; }
	char*			ToJson();
	void			FromJson(const char* json);
	const char*		GetIcon();
	ImVec4			GetIconColor();

	PlcEditorObject(ObjType type, const char* name);
	PlcEditorObject(const char* json_string);
	~PlcEditorObject();

private:
	std::vector<PlcEditorObject*> m_childrens;
	ObjType						 m_type;
	EditorBase*					 m_Editor;
	std::string					 m_name;
	std::string					 m_uuid;

	inline const char* menuItemName(const char* label, char* buf)
	{
		sprintf(buf,u8"%s###RichClickMenuItem_%s", label, this->m_uuid.c_str());
		return buf;
	}

};



#endif