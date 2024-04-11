/*=============================================================================
 * Copyright (c) 2018 rafaelkrause
 *
 * @Script: UI_Style.h
 * @Author: rafaelkrause
 * @Email: rafaelkrause@gmail.com
 * @Create At: XXXX
 * @Last Modified By: rafaelkrause
 * @Last Modified At:
 * @Description: Use Interface Style and Resources Manager Class
 *============================================================================*/

#ifndef UI_CORE_LIB_H
#define UI_CORE_LIB_H

 // =============================================================================
 //                                  INCLUDES
 // =============================================================================
#include "IecProject.hpp"

namespace UI_Core
{


	// =============================================================================
	//                                  ENUMS
	// =============================================================================


	// =============================================================================
	//                      PUBLIC FUNCTIONS DECLARATIONS
	// =============================================================================

	bool AppInit();
	void AppLoop();


	bool GetIsMaximized();
	void SetIsMaximized(bool state);
	void MinimizeApplication();
	void CloseApplication();
	void MoveWindow(bool isMouseDown);
	void SetStatusMsg(const char* msg);
	const char* GetStatusMsg();
	const char* GetAppTitle();

	float GetProjectTreeSize();
	void  SetProjectTreeSize(float size);
	float GetConsoleSize();
	void SetConsoleSize(float size);

	void NewProject();
	IecProject* GetProject();


}

#endif