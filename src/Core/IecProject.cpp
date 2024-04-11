#include "IecProject.hpp"

IecProject::IecProject()
{
	auto mc = new PlcEditorObject(PlcEditorObject::OBJ_TYPE_MOTION_CONTROLLER, u8"MOTION_CONTROLLER");
	mc->AddChildren(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_MOTION_CONTROLLER_AXIS, u8"AXIS_X"));
	mc->AddChildren(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_MOTION_CONTROLLER_AXIS, u8"AXIS_Y"));
	m_objects.push_back(mc);
	
	m_objects.push_back(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_ETHERCAT_MASTER, u8"ETHERCAT"));
	m_objects.push_back(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_MODBUS_SERVER, u8"MODBUS SERVER"));
	m_objects.push_back(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_MODBUS_CLIENTS, u8"MODBUS CLIENTS"));
	
	auto plc =  new PlcEditorObject(PlcEditorObject::OBJ_TYPE_PLC_PROJECT, u8"PLC");
	plc->AddChildren(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_PLC_POUP_PROGRAM_LADDER, u8"PLC_PRG"));
	plc->AddChildren(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_FOLDER, u8"GLOBAL_VARS"));
	plc->AddChildren(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_FOLDER, u8"STRCUTS"));
	plc->AddChildren(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_FOLDER, u8"ENUMS"));
	plc->AddChildren(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_FOLDER, u8"FUNCTIONS"));
	plc->AddChildren(new PlcEditorObject(PlcEditorObject::OBJ_TYPE_FOLDER, u8"FUNCTIONS_BLOCKS"));

	
	m_objects.push_back(plc);

	m_activeEditor = NULL;
}


IecProject::~IecProject()
{
};
