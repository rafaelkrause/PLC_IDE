#ifndef EDITOR_DUT_LIB_H
#define EDITOR_DUT_LIB_H


#include "EditorBase.hpp"
#include "TextEditor.h"
#include "UI_Style.h"
#include "log.h"

class EditorDut : public EditorBase
{
public:
	EditorDut()
	{
		//Set TextEditor Language
		m_texteditor.SetLanguageDefinition(TextEditor::LanguageDefinition::IEC_ST());

		//Create A Unique name for TextEditor child Window
		sprintf(m_editor_name_id, "###_editor_dut_%ld", this);
	};
	
	~EditorDut()
	{
		;
	}

	bool		HasWindow() override					{ return true; };
	bool		HasSavePending() override				{ return m_textChanged; }
	void		SetSaved() override						{ m_textChanged = false; }
	bool		IsFocused() override					{ return m_texteditor.IsFocused(); }

	const char* GetVarData() override					{ return m_texteditor.GetText().c_str();}
	void		SetVarData(const char* data) override	{ m_texteditor.SetText(data); }
	void		PushSetUserCmdByMenu(EDITOR_COMMANDS cmd) override { ; }
	int			GetActualLineIdx() override				{ return iec_max(0, m_texteditor.GetCursorPosition().mLine + 1); }
	int			GetActualColdx() override				{ return iec_max(0, m_texteditor.GetCursorPosition().mColumn + 1); }
	bool		IsOverwrite() override					{ return m_texteditor.IsOverwrite(); }
	void		SetReadOnly(bool value) override		{ m_texteditor.SetReadOnly(value); }
	bool		IsReadOnly() override					{ return m_texteditor.IsReadOnly(); }



	void DrawWindow(ImVec2 size) override
	{
		UI_Style::SetFont(UI_Style::CONSOLE);
		m_texteditor.SetEditorFontSize(UI_Style::GetAppStyle()->FontConsoleSize);
		m_texteditor.Render(m_editor_name_id,size, false);
		UI_Style::SetFont(UI_Style::DEFAULT);

		if (m_texteditor.IsTextChanged())
			m_textChanged = true;
	};


private:
	TextEditor	m_texteditor;
	bool		m_textChanged;
	char		m_editor_name_id[40];
};


#endif
