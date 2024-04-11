#ifndef EDITOR_BASE_LIB_H
#define EDITOR_BASE_LIB_H

#include "imgui.h"

class EditorBase
{
public:

	enum EDITOR_COMMANDS
	{
		CMD_UNDO,
		CMD_REDO,
		CMD_COPY,
		CMD_CUT,
		CMD_PASTE,
		CMD_DEL,
		CMD_SELECT_ALL,
		CMD_FIND,
		CMD_FIND_REPLACE,
		CMD_GOTO_LINE,

	};

	EditorBase() { ; };
	~EditorBase() { ; };


	virtual bool	HasWindow()						{ return false; };
	virtual	bool	HasSavePending()				{ return false; }
	virtual	void	SetSaved()						{; }
	virtual	bool	IsFocused()						{ return false; }
	virtual void	DrawWindow(ImVec2 size)			{ ; }
	virtual const char*   GetVarData()				{ return ""; }
	virtual const char*   GetCodeData()				{ return ""; }
	virtual void	SetVarData(const char* data)	{ ; }
	virtual void	SetCodeData(const char* data)	{ ; }
	virtual void	PushSetUserCmdByMenu(EDITOR_COMMANDS cmd) { ; }
	virtual int		GetActualLineIdx()				{ return 0; }
	virtual int		GetActualColdx()				{ return 0; }
	virtual bool	IsOverwrite()					{ return false; }
	virtual void	SetReadOnly(bool value)			{ ; }
	virtual bool	IsReadOnly()					{ return false; }

private:

};

#endif

