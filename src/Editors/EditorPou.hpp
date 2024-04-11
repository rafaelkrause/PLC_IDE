#ifndef EDITOR_POU_LIB_H
#define EDITOR_POU_LIB_H

#include "standartypes.h"
#include "UI_Widgets.h"
#include "EditorBase.hpp"
#include "TextEditor.h"
#include "UI_Style.h"

class EditorPou : public EditorBase
{
public:
	EditorPou()
	{
		//Set TextEditor Language
		m_texteditorVars.SetLanguageDefinition(TextEditor::LanguageDefinition::IEC_ST());
		m_texteditorCode.SetLanguageDefinition(TextEditor::LanguageDefinition::IEC_ST());
		//Create A Unique name for TextEditor child Window
		//Create A Unique name for TextEditor child Window
		sprintf(m_editor_name_id_vars, "###_editor_pou_var_%ld", this);
		sprintf(m_editor_name_id_code, "###_editor_pou_code_%ld", this);
		sprintf(m_editor_name_id_spliter, "###_splier_code_%ld", this);
		varsPanelSize = 0.3;
		VarPanelSize_y = 0;
		VarPanelSize_y_aux = 0;

	};
	
	~EditorPou()
	{
		;
	}

	bool		HasWindow()  override						{ return true; };
	bool		HasSavePending() override					{ return m_textChanged; }
	void		SetSaved() override							{ m_textChanged = false; }
	bool		IsFocused() override						{ return m_texteditorVars.IsFocused() || m_texteditorCode.IsFocused(); }

	void		PushSetUserCmdByMenu(EDITOR_COMMANDS cmd) override { ; }
	int			GetActualLineIdx() override					{ return  actLn; }
	int			GetActualColdx() override					{ return  actCol; }
	bool		IsOverwrite() override						{ return isOverWrite; }
	void		SetReadOnly(bool value) override			{ m_texteditorVars.SetReadOnly(value); m_texteditorCode.SetReadOnly(value); }
	bool		IsReadOnly() override						{ return m_texteditorVars.IsReadOnly() && m_texteditorCode.IsReadOnly(); }
	const char* GetVarData() override						{ return m_texteditorVars.GetText().c_str(); }
	const char* GetCodeData() override						{ return m_texteditorCode.GetText().c_str(); }
	void		SetVarData(const char* data) override { m_texteditorVars.SetText(data); }
	void		SetCodeData(const char* data) override { m_texteditorCode.SetText(data); }

	void DrawWindow(ImVec2 size) override
	{
		UI_Style::SetFont(UI_Style::CONSOLE);
		
		VarPanelSize_y = varsPanelSize * size.y;

		UI_Widgets::Splitter(m_editor_name_id_spliter, false, 4.0f, &VarPanelSize_y, &VarPanelSize_y_aux, 10.0f, -(size.y - VarPanelSize_y));

		m_texteditorVars.SetEditorFontSize(UI_Style::GetAppStyle()->FontConsoleSize);
		m_texteditorVars.Render(m_editor_name_id_vars,ImVec2(size.x, VarPanelSize_y), false);

		ImGui::Dummy(ImVec2(4, 4));
		m_texteditorCode.SetEditorFontSize(UI_Style::GetAppStyle()->FontConsoleSize);
		m_texteditorCode.Render(m_editor_name_id_code, ImVec2(size.x, size.y - VarPanelSize_y), false);

		UI_Style::SetFont(UI_Style::DEFAULT);

		if (m_texteditorVars.IsTextChanged() || m_texteditorCode.IsTextChanged())
			m_textChanged = true;

		if(m_texteditorVars.IsFocused())
			actLn = iec_max(0, m_texteditorVars.GetCursorPosition().mLine + 1);
		else
			actLn = iec_max(0, m_texteditorCode.GetCursorPosition().mLine + 1);


		if (m_texteditorVars.IsFocused())
			actCol = iec_max(0, m_texteditorVars.GetCursorPosition().mColumn + 1);
		else
			actCol = iec_max(0, m_texteditorCode.GetCursorPosition().mColumn + 1);
		
		if (m_texteditorVars.IsFocused())
			isOverWrite = m_texteditorVars.IsOverwrite();
		else
			isOverWrite = m_texteditorCode.IsOverwrite();

			
		varsPanelSize = VarPanelSize_y / iec_max(1, size.y);


	};



private:

	TextEditor	m_texteditorVars;
	TextEditor	m_texteditorCode;
	bool		m_textChanged;
	char		m_editor_name_id_vars[50];
	char		m_editor_name_id_code[50];
	char		m_editor_name_id_spliter[50];

	float		VarPanelSize_y;
	float		VarPanelSize_y_aux = 0;

	int			actLn;
	int			actCol;

	bool		isOverWrite;
	float		varsPanelSize;
};


#endif
