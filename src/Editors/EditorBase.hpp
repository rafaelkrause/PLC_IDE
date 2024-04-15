#ifndef EDITOR_BASE_LIB_H
#define EDITOR_BASE_LIB_H

#include "imgui.h"
#include <cstddef>

class EditorBase
{
public:


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
	virtual int		GetActualLineIdx()				{ return 0; }
	virtual int		GetActualColdx()				{ return 0; }
	virtual bool	IsOverwrite()					{ return false; }
	virtual void	SetReadOnly(bool value)			{ ; }
	virtual bool	IsReadOnly()					{ return false; }

private:

};

#endif

