#include <cstdio>
#include <time.h>
#include <math.h>
#include "UI_Icons.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "UI_Widgets.h"
#include "UI_Style.h"
#include "standartypes.h"
#include "log.h"


using namespace ImGui;

namespace UI_Widgets
{

	static int_t* NumPad_pValueInt = NULL;
	static real_t* NumPad_pValueFloat = NULL;
	static bool Numpad_isfloat = false;
	static string_t Nunpad_ValueTxt;
	static string_t Numpad_CallerLabel;
	static bool numPadCleared = false;

	static int_t NumPadMaxValue_int = 0;
	static int_t NumPadMinValue_int = 0;

	static real_t NumPadMaxValue_float = 0;
	static real_t NumPadMinValue_float = 0;

	static string_t Keypad_CallerLabel;
	static char *Keypad_Txt = NULL;
	static string_t Keypad_ValueTxt;
	static bool KeyPad_isPassWord;

	//NumPad
	void ShowNumPad(void *v, bool IsFloat)
	{
		if (!v)
			return;

		if (IsFloat)
		{
			NumPad_pValueFloat = (float*)v;
			NumPad_pValueInt = NULL;
			if(*(float*)v == IEC_FLOAT_MAX || *(float*)v == IEC_FLOAT_MIN)
				sprintf(Nunpad_ValueTxt.c, "0");
			else
				sprintf(Nunpad_ValueTxt.c, "%.3f", *(real_t*)v);
		}
		else
		{
			NumPad_pValueFloat = NULL;
			NumPad_pValueInt = (int_t*)v;
			sprintf(Nunpad_ValueTxt.c, "%d", *(int_t*)v);
		}
		
		Numpad_isfloat = IsFloat;

		numPadCleared = false;
		ImGui::OpenPopup("###NumPad");
	}

	void ShowKeyPad(char *v)
	{
		if (v)
		{
			Keypad_Txt = v;
			strcpy(Keypad_ValueTxt.c, v);
			ImGui::OpenPopup("###KeyPad");
		}
	}

	void ClearNumPadBegin()
	{
		if (!numPadCleared)
		{
			sprintf(Nunpad_ValueTxt.c, "");
			numPadCleared = true;
		}
	}

	void NumPadPopUP()
	{

		static bool_t bShowMsgError = false;


		if (NumPad_pValueInt || NumPad_pValueFloat)
		{

			ImVec2 NumPadSize = ImVec2((UI_Style::GetContext()->BtKeyBoard.x +1)*4+GetStyle().WindowPadding.x*2, 0);
			ImVec2 btsz = UI_Style::GetContext()->BtKeyBoard;

			ImFont* oldFont = ImGui::GetFont();

			ImGui::SetNextWindowSize(NumPadSize);
			if (ImGui::BeginPopupModal("###NumPad", NULL, ImGuiWindowFlags_NoDecoration))
			{
				//Proces input by regular keypad, so can use Keypad and Touch Together
				// Process regular text input (before we check for Return because using some IME will effectively send a Return?)
				// We ignore CTRL inputs, but need to allow ALT+CTRL as some keyboards (e.g. German) use AltGR (which _is_ Alt+Ctrl) to input certain characters.
				ImGuiIO io = ImGui::GetIO();
				const bool ignore_char_inputs = (io.KeyCtrl || io.KeyAlt);

				if (!ignore_char_inputs && !IsKeyPressedMap(ImGuiKey_Tab) && io.InputQueueCharacters.Size > 0)
				{
					for (int n = 0; n < io.InputQueueCharacters.Size; n++)
					{
						char* c = (char*)&io.InputQueueCharacters[n];

						if (n == 0 && *c != '+' && *c != '-')
							ClearNumPadBegin();

						if (*c >= '0' && *c <= '9')
							strncat(Nunpad_ValueTxt.c, c, 1);

						if((*c == '.' || *c == ',') && Numpad_isfloat && strchr(Nunpad_ValueTxt.c, '.') == NULL)
							strcat(Nunpad_ValueTxt.c, ".");

						if (*c == '+')
						{
							if (strlen(Nunpad_ValueTxt.c) >= 1)
							{
								os_str_removeChars(Nunpad_ValueTxt.c, '-');
								os_str_removeChars(Nunpad_ValueTxt.c, '+');
							}
							else
							{
								sprintf(Nunpad_ValueTxt.c, "+");
							}
						}

						if (*c == '-')
						{
							if (strlen(Nunpad_ValueTxt.c) >= 1)
							{
								os_str_removeChars(Nunpad_ValueTxt.c, '-');
								os_str_removeChars(Nunpad_ValueTxt.c, '+');
								os_str_insert(Nunpad_ValueTxt.c, STR_LEN, "-", 0);
							}
							else
							{
								sprintf(Nunpad_ValueTxt.c, "-");
							}
						}
					}
					// Consume characters
					io.InputQueueCharacters.resize(0);
				}
				

				UI_Style::SetFont(UI_Style::BIG_BOLD);
				Label(UI_Style::GetContext()->style.ColorText, ImVec2(NumPadSize.x, UI_Style::GetContext()->style.FontBigSize*2), TXT_CENTER, Nunpad_ValueTxt.c);
				
				if (Numpad_isfloat && !(NumPadMaxValue_float == 0.0 && NumPadMinValue_float == 0.0))
				{
					UI_Style::SetFont(UI_Style::SMALL);
					Label(UI_Style::GetContext()->style.ColorText, ImVec2(NumPadSize.x/2, UI_Style::GetContext()->style.FontSmallSize * 2), TXT_LEFT, "Min: %.3f", NumPadMinValue_float);
					SameLine(); Label(UI_Style::GetContext()->style.ColorText, ImVec2(GetContentRegionAvail().x, UI_Style::GetContext()->style.FontSmallSize * 2), TXT_RIGTH, "Max: %.3f", NumPadMaxValue_float);
				}

				if (!Numpad_isfloat && !(NumPadMaxValue_int == 0 && NumPadMinValue_int == 0))
				{
					UI_Style::SetFont(UI_Style::SMALL);
					Label(UI_Style::GetContext()->style.ColorText, ImVec2(NumPadSize.x / 2, UI_Style::GetContext()->style.FontSmallSize * 2), TXT_LEFT, "Min: %d", NumPadMinValue_int);
					SameLine(); Label(UI_Style::GetContext()->style.ColorText, ImVec2(GetContentRegionAvail().x, UI_Style::GetContext()->style.FontSmallSize * 2), TXT_RIGTH, "Max: %d", NumPadMaxValue_int);
				}

				if (bShowMsgError)
				{
					UI_Style::SetFont(UI_Style::SMALL);
					Label(UI_COLOR_LIGHT_RED, ImVec2(NumPadSize.x, UI_Style::GetContext()->style.FontSmallSize * 2), TXT_CENTER, u8"Valor fora dos limites");
				}

				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0);
				ImGui::PushStyleColor(ImGuiCol_Text, UI_Style::GetContext()->style.ColorText);

				UI_Style::SetFont(UI_Style::MEDIUM_BOLD);
				if (Button("7###NumPad_Nr7", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "7"); }
				SameLine(); if (Button("8###NumPad_Nr8", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "8"); }
				SameLine();	if (Button("9###NumPad_Nr9", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "9"); }
				SameLine();	if ((Button(UI_ICON_MD_KEYBOARD_BACKSPACE"###NumPad_BackSpace", btsz) || IsKeyPressedMap(ImGuiKey_Backspace)) && strlen(Nunpad_ValueTxt.c) >= 1) { numPadCleared = true; Nunpad_ValueTxt.c[strlen(Nunpad_ValueTxt.c) - 1] = '\0'; }
				if (Button("4###NumPad_Nr4", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "4"); }
				SameLine(); if (Button("5###NumPad_Nr5", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "5"); }
				SameLine();	if (Button("6###NumPad_Nr6", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "6"); }
				SameLine();	if ((Button("DEL###NumPad_NrDel", btsz) || IsKeyPressedMap(ImGuiKey_Delete)) && strlen(Nunpad_ValueTxt.c) >= 1) { numPadCleared = true; Nunpad_ValueTxt.c[0] = '\0'; }
				if (Button("1###NumPad_Nr1", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "1"); }
				SameLine(); if (Button("2###NumPad_Nr2", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "2"); }
				SameLine();	if (Button("3###NumPad_Nr3", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "3"); }
				SameLine();	if (Button("+###NumPad_Nr+", btsz)) 
								{ 
									if (strlen(Nunpad_ValueTxt.c) >= 1)
									{
										os_str_removeChars(Nunpad_ValueTxt.c, '-');
										os_str_removeChars(Nunpad_ValueTxt.c, '+');
									}
									else
									{
										sprintf(Nunpad_ValueTxt.c, "+");
									}
				};
				if (Button("0###NumPad_Nr0", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "0"); }
				SameLine(); if (Button("00###NumPad_Nr00", btsz)) { ClearNumPadBegin(); strcat(Nunpad_ValueTxt.c, "00"); }
				SameLine(); if (Button(".###NumPad_Nrdot", btsz) && Numpad_isfloat && strchr(Nunpad_ValueTxt.c, '.') == NULL) { ClearNumPadBegin(); if (strlen(Nunpad_ValueTxt.c) >= 1) strcat(Nunpad_ValueTxt.c, "."); else sprintf(Nunpad_ValueTxt.c, "0.0"); }
				SameLine();	if (Button("- ###NumPad_NrMinus", btsz)) 
								{ 
									if (strlen(Nunpad_ValueTxt.c) >= 1)
									{
										os_str_removeChars(Nunpad_ValueTxt.c, '-');
										os_str_removeChars(Nunpad_ValueTxt.c, '+');
										os_str_insert(Nunpad_ValueTxt.c, STR_LEN, "-", 0);
									}
									else
									{
										sprintf(Nunpad_ValueTxt.c, "-");
									}
				};

				//if (strlen(Nunpad_ValueTxt.c) < 1)
				//	strcpy(Nunpad_ValueTxt.c, "0");
				PopStyleColor();
				ImGui::PushStyleColor(ImGuiCol_Text, UI_Style::GetContext()->style.ColorTextSucess);
				UI_Style::SetFont(UI_Style::DEFAULT_BOLD);
				if (ImGui::Button(UI_ICON_MD_CHECK "  OK###NumPadOK", ImVec2(btsz.x*2 +ImGui::GetStyle().ItemSpacing.x, btsz.y)) || IsKeyPressedMap(ImGuiKey_Enter) || IsKeyPressedMap(ImGuiKey_KeyPadEnter))
				{
					bShowMsgError = false;

					if (Numpad_isfloat)
					{
						if (strlen(Nunpad_ValueTxt.c) > 0 && !(NumPadMaxValue_float == 0.0 && NumPadMinValue_float == 0.0))
						{
							real_t v = atof(Nunpad_ValueTxt.c);
							if (v< NumPadMinValue_float || v >NumPadMaxValue_float)
								bShowMsgError = true;
						}			
						
						if (!bShowMsgError)
						{
							if (strlen(Nunpad_ValueTxt.c) > 0)
								*NumPad_pValueFloat = atof(Nunpad_ValueTxt.c);
							else
								*NumPad_pValueFloat = IEC_FLOAT_MAX;
						}
					}
					else
					{
						if (strlen(Nunpad_ValueTxt.c) > 0 && !(NumPadMaxValue_int == 0 && NumPadMinValue_int == 0))
						{
							int v = atoi(Nunpad_ValueTxt.c);
							if (v < NumPadMinValue_int || v > NumPadMaxValue_int)
								bShowMsgError = true;

						}

						if (!bShowMsgError)
						{
							int temp1 = atoi(Nunpad_ValueTxt.c);
							*NumPad_pValueInt = temp1;
						}
					}
						
					if (!bShowMsgError)
					{
						ImGui::CloseCurrentPopup();
						NumPad_pValueInt = NULL;
						NumPad_pValueFloat = NULL;
						Numpad_CallerLabel.c[0] = '\0';
						numPadCleared = false;
					}
				}
				PopStyleColor();
				ImGui::PushStyleColor(ImGuiCol_Text, UI_Style::GetContext()->style.ColorTextDanger);
				SameLine();
				if (ImGui::Button(UI_ICON_MD_CANCEL "  CANCELAR###NumPadCancel", ImVec2(btsz.x * 2+ImGui::GetStyle().ItemSpacing.x, btsz.y)) || IsKeyPressedMap(ImGuiKey_Escape))
				{
					bShowMsgError = false;

					ImGui::CloseCurrentPopup();
					NumPad_pValueInt = NULL;
					NumPad_pValueFloat = NULL;
					Numpad_CallerLabel.c[0] = '\0';
					numPadCleared = false;
				}
				PopStyleColor();
				PopStyleVar(2);
				End();
			}

			//Restore Original Font
			ImGui::SetCurrentFont(oldFont);
		}
		
	}

	void KeyPadPopUp()
	{

		if (!Keypad_Txt)
			return;

		static bool isCapLock;

		ImVec2 KeyPadSize = ImVec2((UI_Style::GetContext()->BtKeyBoard.x + 1) * 12 + GetStyle().WindowPadding.x * 2, 0);
		ImVec2 btsz = UI_Style::GetContext()->BtKeyBoard;

		ImFont* oldFont = ImGui::GetFont();

		ImGui::SetNextWindowSize(KeyPadSize);
		if (ImGui::BeginPopupModal("###KeyPad", NULL, ImGuiWindowFlags_NoDecoration))
		{
			//Proces input by regular keypad, so can use Keypad and Touch Together
			// Process regular text input (before we check for Return because using some IME will effectively send a Return?)
			// We ignore CTRL inputs, but need to allow ALT+CTRL as some keyboards (e.g. German) use AltGR (which _is_ Alt+Ctrl) to input certain characters.
			ImGuiIO io = ImGui::GetIO();
			const bool ignore_char_inputs = (io.KeyCtrl || io.KeyAlt);

			if (!ignore_char_inputs && !IsKeyPressedMap(ImGuiKey_Tab) && io.InputQueueCharacters.Size > 0)
			{
				for (int n = 0; n < io.InputQueueCharacters.Size; n++)
				{
					char* c = (char*)&io.InputQueueCharacters[n];

					if (*c != '\t' && *c != '\n' && *c != '\r')
						strncat(Keypad_ValueTxt.c, c, 1);
				}
				// Consume characters
				io.InputQueueCharacters.resize(0);
			}


			ImVec2 lbsz = ImVec2(KeyPadSize.x, UI_Style::GetContext()->style.FontBigSize * 2);
			UI_Style::SetFont(UI_Style::BIG);
			if (!KeyPad_isPassWord)
			{
				Label(UI_Style::GetContext()->style.ColorText, lbsz, TXT_CENTER, u8"%s",Keypad_ValueTxt.c);
			}
			else
			{
				string_t tmp = {'\0'};

				for (size_t i = 0; i < strlen(Keypad_ValueTxt.c); i++)
				{
					sprintf(tmp.c, u8"%s*", tmp.c);
				}
				Label(UI_Style::GetContext()->style.ColorText, lbsz, TXT_CENTER, "%s", tmp.c);
			}

			UI_Style::SetFont(UI_Style::MEDIUM);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0);
			ImGui::PushStyleColor(ImGuiCol_Text, UI_Style::GetContext()->style.ColorText);


			if (Button("1###KeyPad_Nr1", btsz)) { strcat(Keypad_ValueTxt.c, "1"); }
			ImGui::SameLine(); if (Button("2###KeyPad_Nr2", btsz)) { strcat(Keypad_ValueTxt.c, "2"); }
			ImGui::SameLine(); if (Button("3###KeyPad_Nr3", btsz)) { strcat(Keypad_ValueTxt.c, "3"); }
			ImGui::SameLine(); if (Button("4###KeyPad_Nr4", btsz)) { strcat(Keypad_ValueTxt.c, "4"); }
			ImGui::SameLine(); if (Button("5###KeyPad_Nr5", btsz)) { strcat(Keypad_ValueTxt.c, "5"); }
			ImGui::SameLine(); if (Button("6###KeyPad_Nr6", btsz)) { strcat(Keypad_ValueTxt.c, "6"); }
			ImGui::SameLine(); if (Button("7###KeyPad_Nr7", btsz)) { strcat(Keypad_ValueTxt.c, "7"); }
			ImGui::SameLine(); if (Button("8###KeyPad_Nr8", btsz)) { strcat(Keypad_ValueTxt.c, "8"); }
			ImGui::SameLine(); if (Button("9###KeyPad_Nr9", btsz)) { strcat(Keypad_ValueTxt.c, "9"); }
			ImGui::SameLine(); if (Button("0###KeyPad_Nr0", btsz)) { strcat(Keypad_ValueTxt.c, "0"); }
			ImGui::SameLine(); if (Button(UI_ICON_MD_KEYBOARD_BACKSPACE"###KeyPad_BackSpace", btsz) || IsKeyPressedMap(ImGuiKey_Backspace)) { if (strlen(Keypad_ValueTxt.c) > 0) { Keypad_ValueTxt.c[strlen(Keypad_ValueTxt.c) - 1] = '\0'; } }
			ImGui::SameLine();	if ((Button("DEL###NumPad_NrDel", btsz) || IsKeyPressedMap(ImGuiKey_Delete)) && strlen(Keypad_ValueTxt.c) >= 1) { *Keypad_ValueTxt.c = '\0'; }


			if (Button(isCapLock ? "Q###KeyPad_Nr_q" :"q###KeyPad_Nr_q", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ?"Q":"q"); }
			ImGui::SameLine(); if (Button(isCapLock ? "W###KeyPad_Nr_w" : "w###KeyPad_Nr_w", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "W" : "w"); }
			ImGui::SameLine(); if (Button(isCapLock ? "E###KeyPad_Nr_e" : "e###KeyPad_Nr_e", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "E" : "e"); }
			ImGui::SameLine(); if (Button(isCapLock ? "R###KeyPad_Nr_r" : "r###KeyPad_Nr_r", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "R" : "r"); }
			ImGui::SameLine(); if (Button(isCapLock ? "T###KeyPad_Nr_t" : "t###KeyPad_Nr_t", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "T" : "t"); }
			ImGui::SameLine(); if (Button(isCapLock ? "Y###KeyPad_Nr_y" : "y###KeyPad_Nr_y", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "Y" : "y"); }
			ImGui::SameLine(); if (Button(isCapLock ? "U###KeyPad_Nr_u" : "u###KeyPad_Nr_u", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "U" : "u"); }
			ImGui::SameLine(); if (Button(isCapLock ? "I###KeyPad_Nr_i" : "i###KeyPad_Nr_i", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "I" : "i"); }
			ImGui::SameLine(); if (Button(isCapLock ? "O###KeyPad_Nr_o" : "o###KeyPad_Nr_o", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "O" : "o"); }
			ImGui::SameLine(); if (Button(isCapLock ? "P###KeyPad_Nr_p" : "p###KeyPad_Nr_p", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "P" : "p"); }
			ImGui::SameLine(); if (Button(isCapLock ? u8"�###KeyPad_Nr__" : u8"�###KeyPad_Nr__", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? u8"�" : u8"�"); }
			ImGui::SameLine(); if (Button(isCapLock ? u8"?###KeyPad_Nr_?" : u8"?###KeyPad_Nr_?", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? u8"?" : u8"?"); }


			if (Button(isCapLock ? "A###KeyPad_Nr_a" : "a###KeyPad_Nr_a", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "A" : "a"); }
			ImGui::SameLine(); if (Button(isCapLock ? "S###KeyPad_Nr_s" : "s###KeyPad_Nr_s", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "S" : "s"); }
			ImGui::SameLine(); if (Button(isCapLock ? "D###KeyPad_Nr_d" : "d###KeyPad_Nr_d", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "D" : "d"); }
			ImGui::SameLine(); if (Button(isCapLock ? "F###KeyPad_Nr_f" : "f###KeyPad_Nr_f", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "F" : "f"); }
			ImGui::SameLine(); if (Button(isCapLock ? "G###KeyPad_Nr_g" : "g###KeyPad_Nr_g", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "G" : "g"); }
			ImGui::SameLine(); if (Button(isCapLock ? "H###KeyPad_Nr_h" : "h###KeyPad_Nr_h", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "H" : "h"); }
			ImGui::SameLine(); if (Button(isCapLock ? "J###KeyPad_Nr_j" : "j###KeyPad_Nr_j", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "J" : "j"); }
			ImGui::SameLine(); if (Button(isCapLock ? "K###KeyPad_Nr_k" : "k###KeyPad_Nr_k", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "K" : "k"); }
			ImGui::SameLine(); if (Button(isCapLock ? "L###KeyPad_Nr_l" : "l###KeyPad_Nr_l", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "L" : "l"); }
			ImGui::SameLine(); if (Button(isCapLock ? u8"�###KeyPad_Nr_�" : u8"�###KeyPad_Nr_�", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? u8"�" : u8"�"); }
			ImGui::SameLine(); if (Button(isCapLock ? "%###KeyPad_Nr_%" : "%###KeyPad_Nr_%", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "%%" : "%%"); }
			ImGui::SameLine(); if (Button(isCapLock ? "*###KeyPad_Nr_*" : "*###KeyPad_Nr_*", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "*" : "*"); }

			if (Button(isCapLock ? "Z###KeyPad_Nr_z" : "z###KeyPad_Nr_z", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "Z" : "z"); }
			ImGui::SameLine(); if (Button(isCapLock ? "X###KeyPad_Nr_x" : "x###KeyPad_Nr_x", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "X" : "x"); }
			ImGui::SameLine(); if (Button(isCapLock ? "C###KeyPad_Nr_c" : "c###KeyPad_Nr_c", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "C" : "c"); }
			ImGui::SameLine(); if (Button(isCapLock ? "V###KeyPad_Nr_v" : "v###KeyPad_Nr_v", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "V" : "v"); }
			ImGui::SameLine(); if (Button(isCapLock ? "B###KeyPad_Nr_b" : "b###KeyPad_Nr_b", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "B" : "b"); }
			ImGui::SameLine(); if (Button(isCapLock ? "N###KeyPad_Nr_n" : "n###KeyPad_Nr_n", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "N" : "n"); }
			ImGui::SameLine(); if (Button(isCapLock ? "M###KeyPad_Nr_m" : "m###KeyPad_Nr_m", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "M" : "m"); }
			ImGui::SameLine(); if (Button(",###KeyPad_Nr_,", btsz)) { strcat(Keypad_ValueTxt.c, ","); }
			ImGui::SameLine(); if (Button(".###KeyPad_Nr_.", btsz)) { strcat(Keypad_ValueTxt.c, "."); }
			ImGui::SameLine(); if (Button(":###KeyPad_Nr_:", btsz)) { strcat(Keypad_ValueTxt.c, ":"); }
			ImGui::SameLine(); if (Button(isCapLock ? "@###KeyPad_Nr_@" : "@###KeyPad_Nr_@", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "@" : "@"); }
			ImGui::SameLine(); if (Button(isCapLock ? "$###KeyPad_Nr_$" : "$###KeyPad_Nr_$", btsz)) { strcat(Keypad_ValueTxt.c, isCapLock ? "$" : "$"); }

			if (Button("CAPS###KeyPad_Caps", ImVec2(btsz.x*2 + ImGui::GetStyle().ItemSpacing.x, btsz.y))) { isCapLock = !isCapLock;}
			ImGui::SameLine(); if (Button("\\##KeyPad_\\", btsz)) { strcat(Keypad_ValueTxt.c, "\\"); }
			ImGui::SameLine(); if (Button(" ###KeyPad_space", ImVec2(btsz.x * 4 + 3*ImGui::GetStyle().ItemSpacing.x, btsz.y))) { strcat(Keypad_ValueTxt.c, " "); }
			ImGui::SameLine(); if (Button("/##KeyPad_/", btsz)) { strcat(Keypad_ValueTxt.c, "/"); }
			PopStyleColor();

			UI_Style::SetFont(UI_Style::DEFAULT_BOLD);
			ImGui::PushStyleColor(ImGuiCol_Text, UI_Style::GetContext()->style.ColorTextSucess);
			ImGui::SameLine();
			if (ImGui::Button(UI_ICON_MD_CHECK "  OK###KeypadOK", ImVec2(btsz.x*2 + ImGui::GetStyle().ItemSpacing.x, btsz.y)) || IsKeyPressedMap(ImGuiKey_Enter) || IsKeyPressedMap(ImGuiKey_KeyPadEnter))
			{
				KeyPad_isPassWord = false;
				strcpy(Keypad_Txt, Keypad_ValueTxt.c);
				Keypad_Txt = NULL;
				Keypad_ValueTxt.c[0] = '\0';
				Keypad_CallerLabel.c[0] = '\0';
				ImGui::CloseCurrentPopup();
			}

			PopStyleColor();
			ImGui::PushStyleColor(ImGuiCol_Text, UI_Style::GetContext()->style.ColorTextDanger);
			ImGui::SameLine();
			if (ImGui::Button(UI_ICON_MD_CANCEL "  CANCELAR###KeypadCancel", ImVec2(btsz.x * 2 + ImGui::GetStyle().ItemSpacing.x, btsz.y)) || IsKeyPressedMap(ImGuiKey_Escape))
			{
				KeyPad_isPassWord = false;
				Keypad_Txt = NULL;
				Keypad_ValueTxt.c[0] = '\0';
				Keypad_CallerLabel.c[0] = '\0';
				ImGui::CloseCurrentPopup();
			}

			PopStyleColor();
			PopStyleVar(2);

			End();
		}

		//Restore Original Font
		ImGui::SetCurrentFont(oldFont);
	}

	//Draw Arcs
	void ArcTo(ImDrawList *drawList, const ImVec2& center, float radius, float a_min, float a_max, int num_segments, bool isCCW)
	{
		if (radius == 0.0f)
		{
			drawList->_Path.push_back(center);
			return;
		}

		// Note that we are adding a point at both a_min and a_max.
		// If you are trying to draw a full closed circle you don't want the overlapping points!
		drawList->_Path.reserve(drawList->_Path.Size + (num_segments + 1));
		
		if (!isCCW)
		{
			for (int i = 0; i <= num_segments; i++)
			{
				const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
				drawList->_Path.push_back(ImVec2(center.x + ImCos(a) * radius, center.y + ImSin(a) * radius));
			}
		}
		else
		{
			for (int i = 0; i <= num_segments; i++)
			{
				const float a = a_max - ((float)i / (float)num_segments) * (a_max - a_min);
				drawList->_Path.push_back(ImVec2(center.x + ImCos(a) * radius, center.y + ImSin(a) * radius));
			}
		}
	}

//
// Draw a text on Screen (With Arguments)
//--------------------------------------------------------------------------------------   
void Label(ImVec4 txtcolor , ImVec2 size,TextAligment txtaligm,const char* txt, ...){

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    
    va_list args;
    va_start(args, txt);
	ImFormatStringV(GetCurrentContext()->TempBuffer.Data, GetCurrentContext()->TempBuffer.Size, txt, args);
    va_end(args);

    const ImGuiStyle style = GetCurrentContext()->Style;
    const ImVec2 label_size = CalcTextSize(&GetCurrentContext()->TempBuffer[0], NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size2 = CalcItemSize(size, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 1.0f);
    const ImRect bb(pos, pos + size2);
    ItemSize(bb, style.FramePadding.y);
    ItemAdd(bb, 0);

     // Render
    PushStyleColor(ImGuiCol_Text,txtcolor);
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, &GetCurrentContext()->TempBuffer[0], NULL, &label_size, TxtAlg2Vec2(txtaligm), &bb);
    PopStyleColor();
}
//
// Draw a Text on Screen with a Background Color
//--------------------------------------------------------------------------------------
void VarDisplay(ImVec4 bgcolor, ImVec4 txtcolor, ImVec2 size_arg, TextAligment txtaligm, bool border, const char * txt, ... ){

		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return;

    
		va_list args;
		va_start(args, txt);
		ImFormatStringV(GetCurrentContext()->TempBuffer.Data, GetCurrentContext()->TempBuffer.Size, txt, args);
		va_end(args);

		const ImGuiStyle style = GetCurrentContext()->Style;
		const ImVec2 label_size = CalcTextSize(&GetCurrentContext()->TempBuffer[0], NULL, true);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
		ImRect bb(pos, pos + size);
		ItemSize(bb, style.FramePadding.y);
	

		const ImGuiID id = window->GetID(GetCurrentContext()->TempBuffer[0]);
		ItemAdd(bb, id);

		 // Render
		RenderFrame(bb.Min, bb.Max, ImGui::ColorConvertFloat4ToU32(bgcolor), true, style.FrameRounding);
		PushStyleColor(ImGuiCol_Text,txtcolor);
		RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, &GetCurrentContext()->TempBuffer[0], NULL, &label_size, TxtAlg2Vec2(txtaligm), &bb);
		PopStyleColor();

}

void VarDisplayFloatNull(ImVec4 bgcolor, ImVec4 txtcolor, ImVec2 size_arg, TextAligment txtaligm, bool border, const char * txt, real_t *value)
{
	if (!value || *value == IEC_FLOAT_MAX || *value == IEC_FLOAT_MIN)
	{
		VarDisplay(bgcolor, txtcolor, size_arg, txtaligm, border, "");
	}
	else
	{
		VarDisplay(bgcolor, txtcolor, size_arg, txtaligm, border, txt, *value);
	}
}

//
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
// Use "%Y-%m-%d.%X" format
//--------------------------------------------------------------------------------------
void ShowDateTime(ImVec4 txtcolor , ImVec2 size,TextAligment txtaligm, const char* format) {
	
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	
    tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), format, &tstruct);
	Label(txtcolor,size,txtaligm,buf);
}
//
// Draw Button 
//--------------------------------------------------------------------------------------
bool ButtonCallBack(const char* label, ImVec2 size_arg, bool *onClick, bool *onRealease){
    
    ImGuiContext& g = *GImGui;
    const ImGuiID id = GetCurrentWindow()->GetID(label);

    //Track Release, before ButtonEx ResetActiveID
    *onRealease = (((g.ActiveId == id) && g.IO.MouseReleased[0]) && onRealease);
    const bool click = ButtonEx(label,size_arg);  
    *onClick = (((g.ActiveId == id) && g.IO.MouseClicked[0]) && onClick);
    return click;
}

bool ButtonPush(const char* label, ImVec2 size_arg)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	//if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
	//	flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
	RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

	// Automatically close popups
	//if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
	//    CloseCurrentPopup();

	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
	return held;
}

void ButtonGroup(const char** labels, int qnt, ImVec2 size_arg, ImVec4 bgselect_color, ImVec4 txtselected_color, int_t* selected, bool isHor)
{
	if (!labels)
		return;

	int_t sel = 0;
	if (selected)
		sel = *selected;

	if (!isHor)
	{
		BeginGroup();
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(GetStyle().ItemSpacing.x, 1));
	}
	else
	{
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, GetStyle().ItemSpacing.y));
	}
	for (size_t i = 0; i < qnt; i++)
	{
		if (selected && *selected == i)
		{
			PushStyleColor(ImGuiCol_Button, bgselect_color);
			PushStyleColor(ImGuiCol_ButtonHovered, bgselect_color);
			PushStyleColor(ImGuiCol_ButtonActive, bgselect_color);
			PushStyleColor(ImGuiCol_Text, txtselected_color);
		}

		if (isHor && i > 0)
			SameLine();
		if (Button(labels[i], size_arg))
			sel = i;

		if (selected && *selected == i)
			PopStyleColor(4);
	}
	PopStyleVar(1);
	if (!isHor)
		EndGroup();

	if (selected)
		*selected = sel;
}

bool ButtonIcon(const char* label, const char *icon, ImVec2 size_arg, ImVec4 *icon_color)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	//if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
	//	flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
	RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

	//Render Icon
	if (icon_color)
		PushStyleColor(ImGuiCol_Text, *icon_color);

	RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, icon, NULL, NULL, ImVec2(1,0), &bb);

	if (icon_color)
		PopStyleColor();

	return pressed;
}

bool ButtonBigIcon(const char* label, const char *icon,const char* warning_icon, ImVec2 size_arg, UI_Style::FONT_TYPE icon_font, UI_Style::FONT_TYPE warning_font, UI_Style::FONT_TYPE txt_font, ImVec4 *icon_color, ImVec4 *warnign_color, bool_t *is_pressed)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	//if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
	//	flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
	
	ImFont * old_font = GetFont();

	UI_Style::SetFont(txt_font);
	RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, NULL, ImVec2(0.5, 0.9), &bb);

	//Render Icon
	if (icon_color)
		PushStyleColor(ImGuiCol_Text, *icon_color);

	UI_Style::SetFont(icon_font);
	RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, icon, NULL, NULL, ImVec2(0.5, 0.1), &bb);

	if (icon_color)
		PopStyleColor();

	//Render Warning Icon
	if (warning_icon)
	{
		if (icon_color)
			PushStyleColor(ImGuiCol_Text, *warnign_color);

		UI_Style::SetFont(warning_font);
		RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, warning_icon, NULL, NULL, ImVec2(1, 0.1), &bb);
	}

	//Restore brevius font
	ImGui::SetCurrentFont(old_font);
	

	if (is_pressed)
		*is_pressed = (bool_t)held;


	return pressed;
}

bool TabButton(const char* label, const char *icon, ImVec2 size_arg, ImVec4 icon_color, bool_t* is_held, bool_t *is_close)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	string_t tmp;
	sprintf(tmp.c, u8"%s_close_bt", label);
	const ImGuiID id2 = window->GetID(tmp.c);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const ImVec2 icon_size = CalcTextSize(icon, NULL, true);
	const ImVec2 close_buton_size = CalcTextSize(" X ", NULL, true);


	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + icon_size.x + close_buton_size.x + style.FramePadding.x * 4.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	const ImRect bb_icon(pos, pos + icon_size);
	const ImRect bb_label(pos + ImVec2(icon_size.x + pos.x, pos.y), pos + ImVec2(icon_size.x + pos.x, pos.y) + label_size);

	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	//if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
	//	flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	const ImU32 col = GetColorU32(ImGuiCol_Button);
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

	//Render Icon
	window->DrawList->AddText(ImVec2(pos.x + style.FramePadding.x, pos.y + style.FramePadding.y), ImGui::ColorConvertFloat4ToU32(icon_color),icon);

	//Render Label
	window->DrawList->AddText(ImVec2(pos.x + (style.FramePadding.x * 2)+ icon_size.x, pos.y + style.FramePadding.y), ImGui::GetColorU32(ImGuiCol_Text),label);

	//Render Close Button
	ImVec2 closeBtPos = ImVec2(pos.x + (style.FramePadding.x * 3) + icon_size.x + label_size.x, pos.y + style.FramePadding.y);
	const ImRect bb2(closeBtPos, closeBtPos + ImVec2(close_buton_size.x,close_buton_size.y + 2));
	bool hovered2, held2;
	bool pressed2 = ButtonBehavior(bb2, id2, &hovered2, &held2, flags);

    if (IsMouseHoveringRect(bb2.Min, bb2.Max))
	{
		RenderFrame(bb2.Min, bb2.Max, GetColorU32( ImGuiCol_ButtonHovered), true, style.FrameRounding);

	}

	window->DrawList->AddText(closeBtPos,ImGui::GetColorU32(ImGuiCol_Text), " X ");

	//Flag Close Button Pressed
	if(is_close)
		*is_close = (pressed || pressed2) && IsMouseHoveringRect(bb2.Min, bb2.Max);
   
	//Inform tab is Holden
	if(is_held)
		*is_held = held;

	return pressed;
}

//
// Draw a Toogle Button with Arguments Option
//--------------------------------------------------------------------------------------
void ToogleButton(const char* label, ImVec4 bgcolorON, ImVec4 txtcolorON, ImVec2 size, bool *value) {


	bool reset = false;

	if (value && *value)
	{
		PushStyleColor(ImGuiCol_Button, bgcolorON);
		PushStyleColor(ImGuiCol_ButtonHovered, bgcolorON);
		PushStyleColor(ImGuiCol_ButtonActive, bgcolorON);
		PushStyleColor(ImGuiCol_Text, txtcolorON);
		
		reset = true;
	}

	if (ImGui::Button(label, size))
	{
		*value = !*value;
	}


	if (reset)
	{
		PopStyleColor(4);
	}


	
}

void ToogleButton(const char* label, ImVec4 bgcolorON, ImVec4 txtcolorON, ImVec2 size, int *var, int value, int off_value) 
{


	bool reset = false;

	if (var && *var == value)
	{
		PushStyleColor(ImGuiCol_Button, bgcolorON);
		PushStyleColor(ImGuiCol_ButtonHovered, bgcolorON);
		PushStyleColor(ImGuiCol_ButtonActive, bgcolorON);
		PushStyleColor(ImGuiCol_Text, txtcolorON);

		reset = true;
	}

	if (ImGui::Button(label, size) && var)
	{
		if (*var == value)
			*var = off_value;
		else
			*var = value;
	}

	if (reset)
	{
		PopStyleColor(4);
	}

}

void ToogleButtonArgs(const char* name, ImVec4 bgcolor, ImVec4 bgcolorAct, ImVec4 txtcolor, ImVec4 txtcolorAct, ImVec2 size_arg, TextAligment txtaligm, bool border, bool *value, const char * txt, ...) {

	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiID id = window->GetID(name);

	va_list args;
	va_start(args, txt);
	ImFormatStringV(GetCurrentContext()->TempBuffer.Data, GetCurrentContext()->TempBuffer.Size, txt, args);
	va_end(args);


	const ImGuiStyle style = GetCurrentContext()->Style;
	const ImVec2 label_size = CalcTextSize(&GetCurrentContext()->TempBuffer[0], NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
	ImRect bb(pos, pos + size);
	ItemSize(bb, style.FramePadding.y);

	if (!ItemAdd(bb, id))
		return;

	// Render
	ImVec4 _bgColor;
	ImVec4 _txtColor;
	if (value && *value)
	{
		_bgColor	= bgcolorAct;
		_txtColor	= txtcolorAct;
	}
	else
	{
		_bgColor = bgcolor;
		_txtColor = txtcolor;
	}
	

	RenderFrame(bb.Min, bb.Max, ImGui::ColorConvertFloat4ToU32(_bgColor), true, style.FrameRounding);
	PushStyleColor(ImGuiCol_Text, _txtColor);
	RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, &GetCurrentContext()->TempBuffer[0], NULL, &label_size, TxtAlg2Vec2(txtaligm), &bb);
	PopStyleColor();

	IsItemClicked();

	if (value && IsItemClicked())
		*value = !*value;


}

//
// Input Int with Limtis
//--------------------------------------------------------------------------------------
void InputIntLimits(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags flags, int min, int max)
{
	ImGui::InputInt(label, v, step, step_fast, flags);

	//Check Limits
	if (*v < min)
		*v = min;
	if (*v > max)
		*v = max;

}

//InputTouch
void InputInt16_Touch(const char* label, int_t* v, const char* format, int_t min, int_t max)
{
	bool changed = InputScalar(label, ImGuiDataType_S16, v, NULL, NULL, format, 0);
	

	if (ImGui::IsItemActivated() && UI_Style::GetAppStyle()->IsTouchEnable)
	{
		NumPad_pValueFloat = NULL;
		NumPad_pValueInt = v;
		sprintf(Nunpad_ValueTxt.c, "%d", *v);
		Numpad_isfloat = false;

		NumPadMaxValue_int = max;
		NumPadMinValue_int = min;

		ImGui::OpenPopup("###NumPad");
		strcpy(Numpad_CallerLabel.c, label);
	}

	if(strcmp(Numpad_CallerLabel.c,label) == 0)
		NumPadPopUP();

	if (changed && min != 0 && max != 0)
	{
		//Check Limits
		if (*v < min)
			*v = min;
		if (*v > max)
			*v = max;
	}
}

void Inputreal_touch(const char* label, real_t* v, const char* format, real_t min, real_t max)
{

	bool changed;

	if (!v || *v == IEC_FLOAT_MAX || *v == IEC_FLOAT_MIN)
		changed = InputScalar(label, ImGuiDataType_Float, v, NULL, NULL, " ", 0);
	else
		changed = InputScalar(label, ImGuiDataType_Float, v, NULL, NULL, format, 0);

	if (ImGui::IsItemActivated() && v && UI_Style::GetAppStyle()->IsTouchEnable)
	{
		NumPad_pValueFloat = v;	
		NumPad_pValueInt = NULL;
	
		NumPadMaxValue_float = max;
		NumPadMinValue_float = min;

		if (*(float*)v == IEC_FLOAT_MAX || *(float*)v == IEC_FLOAT_MIN)
			sprintf(Nunpad_ValueTxt.c, "");
		else
			sprintf(Nunpad_ValueTxt.c, "%.3f", *(real_t*)v);

		Numpad_isfloat = true;

		ImGui::OpenPopup("###NumPad");
		strcpy(Numpad_CallerLabel.c, label);
	}

	if (strcmp(Numpad_CallerLabel.c, label) == 0)
		NumPadPopUP();

	if (changed && min != 0 && max != 0)
	{
		//Check Limits
		if (*v < min)
			*v = min;
		if (*v > max)
			*v = max;
	}

}

void InputString_Touch(const char* label, char* v, size_t buf_size, ImGuiInputTextFlags flags)
{
	bool result = ImGui::InputText(label, v, buf_size, flags, 0, 0);

	if (ImGui::IsItemActivated() && UI_Style::GetAppStyle()->IsTouchEnable)
	{
		if (v)
		{
			KeyPad_isPassWord = (flags & ImGuiInputTextFlags_Password);
			Keypad_Txt = v;
			strcpy(Keypad_ValueTxt.c, v);
			strcpy(Keypad_CallerLabel.c, label);
			ImGui::OpenPopup("###KeyPad");
		}
	}

	if (strcmp(Keypad_CallerLabel.c, label) == 0)
		KeyPadPopUp();
}

//
// Loader Spinner
//--------------------------------------------------------------------------------------
bool Spinner(const char* label, ImVec2 size, float radius, int thickness, const ImVec4 color)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	// Render
	//window->DrawList->PathClear();

	int num_segments = 30;
	int start = (int)ImSin(g.Time*1.8f)*(num_segments - 5);

	const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
	const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

	const ImVec2 centre = ImVec2(pos.x + size.x/2, pos.y + size.y/2);

	for (int i = 0; i < num_segments; i++) 
	{
		const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
		window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius, centre.y + ImSin(a + g.Time * 8) * radius));
	}

	window->DrawList->PathStroke(ColorConvertFloat4ToU32(color), false, thickness);

		
	return true;
}

//
// Vertical Separator
//-------------------------
void SeparatorVert()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	if (window->SkipItems)
		return;

	// Those flags should eventually be overridable by the user
	ImGuiSeparatorFlags flags = ImGuiSeparatorFlags_Vertical;
	flags |= ImGuiSeparatorFlags_SpanAllColumns;
	SeparatorEx(flags);
}

bool VSliderInt16(const char* label, const ImVec2& size, int_t* v, int_t v_min, int_t v_max, const char* format)
{
	return VSliderScalar(label, size, ImGuiDataType_S16, v, &v_min, &v_max, format);
}

bool VerticalSlider(const char* label, const ImVec2& size, void* v, const real_t v_min, const real_t v_max, ImVec4 frame_Color, ImVec4 grab_color, real_t grab_radius)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);

	const float center_x = frame_bb.GetWidth()*0.5 + frame_bb.Min.x;
	const ImRect bb(ImVec2(center_x - 2 * grab_radius, frame_bb.Min.y - grab_radius), ImVec2(center_x + 2 * grab_radius, frame_bb.Max.y + grab_radius));

	ItemSize(frame_bb, style.FramePadding.y);
	if (!ItemAdd(frame_bb, id))
		return false;
	
	const bool hovered = ItemHoverable(bb, id);
	if ((hovered && g.IO.MouseClicked[0]) || g.NavActivateId == id || g.NavActivateInputId == id)
	{
		SetActiveID(id, window);
		SetFocusID(id, window);
		FocusWindow(window);
	}


	//Calc Percent
	real_t value_range = iec_max(1, v_max - v_min);

	// Process interacting with the slider
	bool value_changed = false;

	//Use Scroll
	if (hovered && ImGui::GetIO().MouseWheel != 0.0)
	{
		*(float*)v += ImGui::GetIO().MouseWheel;

		*(float*)v = iec_min(v_max, *(float*)v);
		*(float*)v = iec_max(v_min, *(float*)v);

		value_changed = true;

	}

	//Mouse Position
	if (g.ActiveId == id)
	{
		if (!g.IO.MouseDown[0])
		{
			ClearActiveID();
		}
		else
		{
			//Calc new Value
			float slider_mouse_pos = bb.Max.y - g.IO.MousePos.y;
			const float slider_usable_sz = (bb.Max.y - bb.Min.y);
			const float slider_usable_pos_min = bb.Min.y;
			const float slider_usable_pos_max = bb.Max.y;


			const float mouse_abs_pos = g.IO.MousePos.y;
			float clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
			clicked_t = 1.0f - clicked_t;


			float new_value = clicked_t * value_range;

			// Apply result
			if (*(float*)v != new_value)
			{
				*(float*)v = new_value;
				value_changed = true;
			}
		}
	}

	real_t perCent = *(real_t*)v / value_range;
	const float line_Radius = 3;

	// Draw Middle Line
	window->DrawList->AddRectFilled(ImVec2(center_x - line_Radius, frame_bb.Min.y), ImVec2(center_x + line_Radius, frame_bb.Max.y), ColorConvertFloat4ToU32(frame_Color));
	window->DrawList->AddCircleFilled(ImVec2(center_x, frame_bb.Min.y), line_Radius, ColorConvertFloat4ToU32(frame_Color)); // Round Edge
	window->DrawList->AddCircleFilled(ImVec2(center_x, frame_bb.Max.y), line_Radius, ColorConvertFloat4ToU32(frame_Color)); // Round Edge

	//Draw Percent Fill
	ImVec2 grabPos = ImVec2(center_x, frame_bb.Max.y - ((frame_bb.Max.y - frame_bb.Min.y)*perCent));
	window->DrawList->AddRectFilled(ImVec2(center_x - line_Radius, frame_bb.Max.y), ImVec2(grabPos.x + line_Radius,grabPos.y), ColorConvertFloat4ToU32(grab_color));
	window->DrawList->AddCircleFilled(ImVec2(center_x, frame_bb.Max.y), line_Radius, ColorConvertFloat4ToU32(grab_color));  // Round Edge


	//Draw Grab Hover
	if(hovered)
		window->DrawList->AddCircleFilled(grabPos, grab_radius*1.5, ColorConvertFloat4ToU32(ImVec4(grab_color.x, grab_color.y, grab_color.z, grab_color.w*0.5)),30);

	//Draw Grab
	window->DrawList->AddCircleFilled(grabPos, grab_radius*1.3, ColorConvertFloat4ToU32(ImVec4(grab_color.x, grab_color.y, grab_color.z, grab_color.w*0.2)), 30);
	window->DrawList->AddCircleFilled(grabPos, grab_radius, ColorConvertFloat4ToU32(grab_color),30);

	return value_changed;
}

//
// Leds
//
void Led(ImVec2 size, ImVec4 border_color, ImVec4 on_color, ImVec4 off_color, bool is_on)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	ImVec2 pos = window->DC.CursorPos;

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, 0))
		return;

	// Render
	window->DrawList->PathClear();

	float radius = ImMin(size.x,size.y)/2;
	window->DrawList->AddCircleFilled(bb.GetCenter(), radius*0.8, ColorConvertFloat4ToU32(is_on? on_color : off_color), 30);
	window->DrawList->AddCircle(bb.GetCenter(), radius, ColorConvertFloat4ToU32(border_color), 30);

}

//
// Help Marks
//
// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see misc/fonts/README.txt)
void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

bool Splitter(const char* name, bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID(name);
	ImRect bb;
	bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
	bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	return SplitterBehavior(bb,id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 10.0f);
}

} // End UI_Widgets


