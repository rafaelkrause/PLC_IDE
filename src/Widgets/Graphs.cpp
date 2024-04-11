#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "UI_Widgets.h"
#include "UI_Style.h"

using namespace ImGui; //To avoid Write ImGui::, less work

namespace UI_Widgets
{

//
// Show A circular GRaph, Proportional to Max/Min Value. Inside GRaph Shows Label
//-----------------------------------------------------------------------------------
void BarDisplay(ImVec2 size, float MaxValue, float MinValue,float Value, ImVec4 BgColor, ImVec4 BarColor, ImVec4 TextColor, float round, bool show_minmax){
    
    ImGuiWindow* window = GetCurrentWindow();

    if (window->SkipItems)
        return;
    
    const ImGuiStyle style = GetCurrentContext()->Style;
    ImVec2 pos = window->DC.CursorPos;
    const ImRect bb(pos, pos + size);
    ItemSize(bb, style.FramePadding.y);
    ImRect bar_bb = bb;

    //Draw Background
    window->DrawList->AddRectFilled(bb.Min,bb.Max,ColorConvertFloat4ToU32(BgColor),round);
    //DrawBar
    if(MaxValue == MinValue || MaxValue <= MinValue)
        return;
    float xscale = (size.x/(MaxValue - MinValue));
    
	//Check mas and min values
	if (Value < MinValue) { Value = MinValue; }
	if (Value > MaxValue) { Value = MaxValue; }

	if (Value > MinValue)
	{
		bar_bb.Max.x = bar_bb.Min.x + Value * xscale;
		window->DrawList->AddRectFilled(bar_bb.Min, bar_bb.Max, ColorConvertFloat4ToU32(BarColor), round);
	}
    
}

//
// Show A circular GRaph, Proportional to Max/Min Value. Inside GRaph Shows Label
//-----------------------------------------------------------------------------------
void CircleGraph(ImVec2 size,         
                float MaxValue,     // Equivalente 0º 0%
                float MinValue,     // Equivalente 360º 100%
                float Value,
                ImVec4 BgColor,
                ImVec4 GraphColor,
                ImVec4 InnerColor,
                ImVec4 TextColor,
                const char* Label,
                const char* InnerLabel,
                const char* InnerLabel2){

    ImGuiWindow* window = GetCurrentWindow();

    if (window->SkipItems)
        return;

    //Register G
    const ImGuiStyle style = GetCurrentContext()->Style;
    ImVec2 pos = window->DC.CursorPos;
    const ImRect bb(pos, pos + size);
    ItemSize(bb, style.FramePadding.y);    
 
    //Cacl Graph REctangle
    


    //Calc End Angle
    float endA;
    if(Value < MinValue){ endA = 0.0f;}
    else if( Value > MaxValue){endA = 1.0f;}
    else{endA = Value/(MaxValue - MinValue); }

    endA = endA*2*IM_PI;

    const int segments = 60;
	float radius = 0;
	ImVec2 center = ImVec2(0,0);
	
	if (strlen(Label) > 0)
	{
		radius = iec_min(size.x / 2, size.y / 2 - style.FramePadding.y);
		center = ImVec2(pos.x + size.x / 2, pos.y + (size.y - CalcTextSize("X").y - style.FramePadding.y) / 2);
	}
	else
	{
		radius = iec_min(size.x / 2, size.y / 2 - style.FramePadding.y);
		center = ImVec2(pos.x + size.x / 2, pos.y + (size.y - style.FramePadding.y) / 2);
	}

    // Draw Background
    window->DrawList->AddCircleFilled(center,radius,ColorConvertFloat4ToU32(BgColor),segments);
    // Draw Graph
    // start: end of the outer arc.  end: start of the inner arc
    const ImVec2 vec1 = ImVec2( cosf(endA)*radius + center.x, sinf(endA)*radius + center.y);
    // start: end of the inner arc.  end: start of the outer arc
    const ImVec2 vec2 = ImVec2( cosf(0)*radius + center.x, sinf(0)*radius + center.y);
    // draw the pie segment
    // window->DrawList->AddLine(vec_line_start, vec_line_end, ImGui::ColorConvertFloat4ToU32(ImVec4(255.0f, 255.0f, 255.0f, 255.0f)));
    window->DrawList->PathLineTo(center);
    window->DrawList->PathArcTo(center, radius, 0, (endA-0.001f), segments);  //000.1 to remove little gap betwen arc and line
    window->DrawList->PathLineTo(vec1);  // start outer to inner arc line   
    window->DrawList->PathFillConvex(ColorConvertFloat4ToU32(GraphColor)); 
    // Draw Inner
    window->DrawList->AddCircleFilled(center,radius*0.9f,ColorConvertFloat4ToU32(InnerColor),segments);
    // Draw Inner Label
    sprintf(&GetCurrentContext()->TempBuffer[0],InnerLabel, Value);
    ImVec2 txtsz = ImGui::CalcTextSize(&GetCurrentContext()->TempBuffer[0]);
    txtsz.x = center.x - (txtsz.x/2);
    txtsz.y = center.y - (txtsz.y/2);
    window->DrawList->AddText(txtsz,ImGui::ColorConvertFloat4ToU32(TextColor),&GetCurrentContext()->TempBuffer[0]);
    // Draw Inner Label 2
    txtsz = ImGui::CalcTextSize(InnerLabel2);
    txtsz.x = center.x - (txtsz.x/2);
    txtsz.y = center.y + (txtsz.y/2)*2 + 1;
    window->DrawList->AddText(txtsz,ImGui::ColorConvertFloat4ToU32(TextColor),InnerLabel2);

    //Out Label
	if (strlen(Label) > 0)
	{
		txtsz = CalcTextSize(Label);
		txtsz.x = center.x - (txtsz.x / 2);
		txtsz.x = center.y - (txtsz.y / 2);
		//txtsz.y = bb.Max.y - (txtsz.y)-style.FramePadding.y;
		window->DrawList->AddText(txtsz, ColorConvertFloat4ToU32(TextColor), Label);
	}

}


//
// Show A circular GRaph, Proportional to Max/Min Value. Inside GRaph Shows Label
//-----------------------------------------------------------------------------------
void ArcGraph(ImVec2 size,
	float MaxValue,     // Equivalente 0º 0%
	float MinValue,     // Equivalente 360º 100%
	float Value,
	ImVec4 BgColor,
	ImVec4 GraphColor,
	ImVec4 InnerColor,
	ImVec4 TextColor,
	const char* InnerLabel)
{

	ImGuiWindow* window = GetCurrentWindow();

	if (window->SkipItems)
		return;

	//Register G
	const ImGuiStyle style = GetCurrentContext()->Style;
	ImVec2 pos = window->DC.CursorPos;
	const ImRect bb(pos, pos + size);
	ItemSize(bb, style.FramePadding.y);

	//Cacl Graph REctangle



	//Calc End Angle
	float endA;
	if (Value < MinValue) { endA = 0.0f; }
	else if (Value > MaxValue) { endA = 1.0f; }
	else { endA = Value / (MaxValue - MinValue); }

	endA = (endA * IM_PI) + IM_PI;

	const int segments = 60;
	float radius = 0;
	ImVec2 center = ImVec2(0, 0);

	radius = iec_min(size.x - style.FramePadding.x*2, size.y - style.FramePadding.y*2);
	center = ImVec2(pos.x + (size.x/2), pos.y + size.y);

	// Draw Background
	// draw the pie segment
	window->DrawList->PathLineTo(center);
	window->DrawList->PathArcTo(center, radius, IM_PI, 2*IM_PI, segments);
	window->DrawList->PathFillConvex(ColorConvertFloat4ToU32(BgColor));

	// Draw Graph
	// start: end of the outer arc.  end: start of the inner arc
	const ImVec2 vec1 = ImVec2(cosf(endA)*radius + center.x, sinf(endA)*radius + center.y);
	// start: end of the inner arc.  end: start of the outer arc
	const ImVec2 vec2 = ImVec2(cosf(0)*radius + center.x, sinf(0)*radius + center.y);
	// draw the pie segment
	window->DrawList->PathLineTo(center);
	window->DrawList->PathArcTo(center, radius, IM_PI, (endA - 0.001f), segments);  //000.1 to remove little gap betwen arc and line
	window->DrawList->PathLineTo(vec1);  // start outer to inner arc line   
	window->DrawList->PathFillConvex(ColorConvertFloat4ToU32(GraphColor));
	
	// Draw Inner
	window->DrawList->PathLineTo(center);
	window->DrawList->PathArcTo(center, radius*0.8f, IM_PI, 2 * IM_PI, segments);
	window->DrawList->PathFillConvex(ColorConvertFloat4ToU32(InnerColor));
	
	//Round Corner
	//const ImVec2 vec3 = ImVec2(cosf(endA)*radius*0.90f + center.x, sinf(endA)*radius*0.95f + center.y);
	//window->DrawList->AddCircleFilled(vec3, radius*0.1f, ColorConvertFloat4ToU32(GraphColor), 30);

	// Draw Inner Label
	sprintf(&GetCurrentContext()->TempBuffer[0], InnerLabel, Value);
	ImVec2 txtsz = ImGui::CalcTextSize(&GetCurrentContext()->TempBuffer[0]);
	txtsz.x = center.x - (txtsz.x / 2);
	txtsz.y = pos.y + radius - (txtsz.y *1.2 );
	
	window->DrawList->AddText(txtsz, ImGui::ColorConvertFloat4ToU32(TextColor), &GetCurrentContext()->TempBuffer[0]);

}



//
// Continuos Graph, The Buffer size determine the history time
//-----------------------------------------------------------------------------------
void ContinuosGraphLines(const char *Label,ImVec2 size, PlotLinesTimeProp * data){

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    const ImGuiStyle& style = GetStyle();

    const ImVec2 label_size = CalcTextSize(Label, NULL, true);

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size.x, size.y));
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
    const ImRect graph_bb(inner_bb.Min + ImVec2(0,label_size.y + style.FramePadding.y),inner_bb.Max);

    ItemSize(frame_bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, 0))
        return;

    //const bool hovered = ItemHoverable(graph_bb, 0);

    //Render Back Ground
    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, 0.0f);

    if(!data)
        return;
   
    //Render Labels
    ImVec2 labelpos;
    //Graphs Label
    labelpos.x = inner_bb.Min.x + (inner_bb.GetSize().x/2)-(label_size.x/2);
    labelpos.y = inner_bb.Min.y;
    window->DrawList->AddText(labelpos,GetColorU32(data->LabelsColor),Label);
    //Max Value
    sprintf(&GetCurrentContext()->TempBuffer[0],"%.0f",data->MaxValue);
    window->DrawList->AddText(inner_bb.Min,GetColorU32(data->LabelsColor),&GetCurrentContext()->TempBuffer[0]);
    //MinValue
    labelpos.x = inner_bb.Min.x;
    labelpos.y = inner_bb.Max.y - label_size.y;
    sprintf(&GetCurrentContext()->TempBuffer[0],"%.0f",data->MinValue);
    window->DrawList->AddText(labelpos,GetColorU32(data->LabelsColor),&GetCurrentContext()->TempBuffer[0]);   

    //Render Grid
    if(data->GridHLns >0){
        const float griHRate = graph_bb.GetHeight()/data->GridHLns;
        ImVec2 hlnStart,hlnend;
        for(int i = 0; i <= data->GridHLns; i++)
        {
            hlnStart.x = graph_bb.Min.x;
            hlnStart.y = graph_bb.Min.y + (i*griHRate);
            hlnend.x = graph_bb.Max.x;
            hlnend.y = hlnStart.y;
            window->DrawList->AddLine(hlnStart,hlnend,ColorConvertFloat4ToU32(data->GridColor),1.0f);
        }
    }
    if(data->GridVLns >0){
        const float griVRate = graph_bb.GetWidth()/data->GridVLns;
        ImVec2 vlnStart,vlnend;
        for(int i = 0; i <= data->GridVLns; i++)
        {
            vlnStart.x = graph_bb.Min.x+(i*griVRate);
            vlnStart.y = graph_bb.Min.y;
            vlnend.x = vlnStart.x;
            vlnend.y = graph_bb.Max.y;
            window->DrawList->AddLine(vlnStart,vlnend,ColorConvertFloat4ToU32(data->GridColor),1.0f);
        }
    }

    //Plot Graph
    if(data->ValuesLen>0 && data->NumberOfPens>0){
    
        if(data->MaxValue == data->MinValue)
            return;

        float yScale = graph_bb.GetHeight()/(data->MaxValue - data->MinValue);
        float xStep = graph_bb.GetWidth()/data->ValuesLen;

        
        for(int i = 0; i < data->NumberOfPens; i++)
        {     
            ImVec2 p0 = ImVec2(graph_bb.Min.x,graph_bb.Max.y-((data->Values[i][0] - data->MinValue)*yScale));
            ImVec2 p1;
            const ImU32 lncolor = ColorConvertFloat4ToU32(data->GraphColor[i]);

            for(int j = 1; j < data->ValuesLen; j++){           
                p1.x = graph_bb.Min.x+(xStep*j);
                p1.y = graph_bb.Max.y-((data->Values[i][j] - data->MinValue)*yScale);

                if(p0.y >= graph_bb.Min.y && p1.y >= graph_bb.Min.y && p0.y <= graph_bb.Max.y && p1.y <= graph_bb.Max.y )
                    window->DrawList->AddLine(p0,p1,lncolor);  
                p0 = p1;
            }     
        }
    }
};


}// End namespace UI_Widgets