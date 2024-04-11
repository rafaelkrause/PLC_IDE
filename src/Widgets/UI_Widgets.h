#ifndef UI_WIDGETS_LIB_H
#define UI_WIDGETS_LIB_H

#include "os_generic.h"
#include <stdarg.h> 
#include "imgui.h"
#include "UI_Style.h"


namespace UI_Widgets{

#define DEGREE_TO_RAD 0.0174533


// Configs
//------------------------------------------------------------------
enum TextAligment {TXT_LEFT,TXT_CENTER,TXT_RIGTH};
inline ImVec2 TxtAlg2Vec2(TextAligment algm){ 
            switch (algm){
                default:
                case TextAligment::TXT_LEFT: return ImVec2(0,0.5f);
                case TextAligment::TXT_CENTER: return ImVec2(0.5f,0.5f);
                case TextAligment::TXT_RIGTH: return ImVec2(1.0f,0.5f);   
            }
}

//	Text Display
//------------------------------------------------------------------
//Show a Text on Screen (Wtih color Option)
void Label(ImVec4 txtcolor , ImVec2 size,TextAligment txtaligm,const char* txt, ...);
//Show a text with Background Color (Colors,size, position aligment are options)
void VarDisplay(ImVec4 bgcolor, ImVec4 txtcolor, ImVec2 size_arg, TextAligment txtaligm, bool border, const char * txt, ... );

void VarDisplayFloatNull(ImVec4 bgcolor, ImVec4 txtcolor, ImVec2 size_arg, TextAligment txtaligm, bool border, const char * txt, real_t *value);
//
// Show Actual Time Use. Use format "%Y-%m-%d.%X" format (For More Information http://en.cppreference.com/w/cpp/chrono/c/strftime)
void ShowDateTime(ImVec4 txtcolor , ImVec2 size,TextAligment txtaligm, const char* format);
//
// Buttons
//------------------------------------------------------------------
bool ButtonCallBack(const char* label, ImVec2 size_arg, bool *onClick, bool *onRealease);

bool ButtonPush(const char* label, ImVec2 size_arg);

void ButtonGroup(const char** labels, int qnt, ImVec2 size_arg, ImVec4 bgselect_color, ImVec4 txtselected_color, int_t* selected, bool isHor);

bool ButtonIcon(const char* label, const char *icon, ImVec2 size_arg, ImVec4 *icon_color);

bool ButtonBigIcon(const char* label, const char *icon, const char* warning_icon, ImVec2 size_arg, UI_Style::FONT_TYPE icon_font, UI_Style::FONT_TYPE warning_font, UI_Style::FONT_TYPE txt_font, ImVec4 *icon_color, ImVec4 *warnign_color, bool_t *is_pressed);

void ToogleButton(const char* label, ImVec4 bgcolorON, ImVec4 txtcolorON, ImVec2 size, bool *value);

void ToogleButton(const char* label, ImVec4 bgcolorON, ImVec4 txtcolorON, ImVec2 size, int *var, int value, int off_value);

void ToogleButtonArgs(const char* name, ImVec4 bgcolor, ImVec4 bgcolorAct, ImVec4 txtcolor, ImVec4 txtcolorAct, ImVec2 size_arg, TextAligment txtaligm, bool border, bool *value, const char * txt, ...);

bool TabButton(const char* label, const char *icon, ImVec2 size_arg, ImVec4 icon_color, bool_t* is_held, bool_t *is_close);

// Input Values
//------------------------------------------------------------------
void InputIntLimits(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags flags, int min, int max);

//Input TOUCH
//-------------------------------------------------------------------
void InputInt16_Touch(const char* label, int_t* v, const char* format, int_t min = 0, int_t max = 0);

void Inputreal_touch(const char* label, real_t* v, const char* format, real_t min = 0.0f, real_t max = 0.0f);

void InputString_Touch(const char* label, char* v, size_t buf_size, ImGuiInputTextFlags flags = 0);

//Spinners and Laoders
//------------------------------------------------------------------
bool Spinner(const char* label, ImVec2 size, float radius, int thickness, const ImVec4 color);


// 
// Separator
//------------------------------------------------------------------
void SeparatorVert();


bool VSliderInt16(const char* label, const ImVec2& size, int_t* v, int_t v_min, int_t v_max, const char* format);

bool VerticalSlider(const char* label, const ImVec2& size, void* v, const real_t v_min, const real_t v_max, ImVec4 frame_Color, ImVec4 grab_color, real_t grab_radius);

//	Graphs
//------------------------------------------------------------------
// Plot a BarDisplay (Like Progres bar)
void BarDisplay(ImVec2 size, float MaxValue, float MinValue,float Value, ImVec4 BgColor, ImVec4 BarColor, ImVec4 TextColor, float round, bool show_minmax);

// Plot a Circle Graph, Proportional to Max/Min With Center Label
void CircleGraph(ImVec2 size,float MaxValue,float MinValue,float Value,ImVec4 BgColor,ImVec4 GraphColor,ImVec4 InnerColor,ImVec4 TextColor,const char* Label,const char* InnerLabel,const char* InnerLabel2);

void ArcGraph(ImVec2 size, float MaxValue, float MinValue, float Value, ImVec4 BgColor, ImVec4 GraphColor, ImVec4 InnerColor, ImVec4 TextColor, const char* InnerLabel);


// Create Time Based Graph
struct PlotLinesTimeProp{
    float   **Values;
	int		NumberOfPens;		
	int		ValuesLen;		
	float	MaxValue;
	float	MinValue;
	int 	GridVLns; 
    int 	GridHLns; 
    ImVec4 	GridColor;
    ImVec4 	LabelsColor;
    ImVec4 	*GraphColor; 
    PlotLinesTimeProp() { PlotLinesTimeProp(1, 250, 100, 0,3,5);}
	PlotLinesTimeProp(int nrPens, int len, float max, float min,int gridVLns,int gridHLns) 
	{ 
		MaxValue = max; 
		MinValue = min; 
		NumberOfPens = (nrPens <=0 ) ? 1 : nrPens;
		ValuesLen = (len <=0 ) ? 250 : len; 
		Values = new float*[NumberOfPens];
		GraphColor = new ImVec4[NumberOfPens];
		for(int i = 0; i < NumberOfPens; i++)
		{
			Values[i] = new float[ValuesLen];
			memset(Values[i], 0, sizeof(float)*ValuesLen); // clean data
			GraphColor[i] = UI_Style::GetContext()->style.ColorPlotLines;
		};
		GridVLns = gridVLns;
		GridHLns = gridHLns;
		GridColor = UI_Style::GetContext()->style.ColorPlotHistogram; // TODO Add new Style For Plot Gid
		LabelsColor = UI_Style::GetContext()->style.ColorText;
	}
	void    Add(int pen, float value) { 
			if( pen <0 || pen>NumberOfPens-1)
			 	return; 
			for(int i = 0; i < ValuesLen-1; i++) 
			{ Values[pen][i] = Values[pen][i+1];} Values[pen][ValuesLen-1] = value;}
	void 	FreeData() { free(Values), MaxValue = MinValue = 0.0f;ValuesLen = 0;NumberOfPens=0;GridVLns = 0;GridHLns=0;free(GraphColor);}
 };


void ContinuosGraphLines(const char *Label,ImVec2 size,PlotLinesTimeProp * data);

//NUMPAD
//------------------------------------------------------------------------------------------------------------------
void NumPadPopUP();

//
// Leds
//-------------------------------------------------------------------------------------------------------------------
void Led(ImVec2 size, ImVec4 border_color, ImVec4 on_color, ImVec4 off_color, bool is_on);


void HelpMarker(const char* desc);


// Axis
//---------------------------------------------------------------------------------------------------------------
void AxisState(int_t axistate, ImVec2 size);


bool Splitter(const char* name, bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_siz= -1.0f);


} //End namespace UI_Widgets


#endif