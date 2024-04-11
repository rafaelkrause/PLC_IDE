/*=============================================================================
 * Copyright (c) 2018 rafaelkrause
 *
 * @Script: log.h
 * @Author: rafaelkrause
 * @Email: rafaelkrause@gmail.com
 * @Create At: 
 * @Last Modified By: rafaelkrause
 * @Last Modified At: 
 * @Description: Log Implementation (Alowed to use on RT tasks)
 *    Orignal from https://github.com/rxi/log.c Copyright (c) 2017 rxi
 *    Acess the link for Licence information
 * 
 * @Usage: Just add log.h and call log_init anywhere before use it.
 *          At end of application, call log_finish to release resources  
 *============================================================================*/
#ifndef _LOG_H_
#define _LOG_H_

#if __cplusplus
extern "C" {
#endif

	// =============================================================================
	//                                  INCLUDES
	// =============================================================================
	//#include <stdio.h>
#include <stdarg.h>

// =============================================================================
//                              DEFINES/MACROS
// =============================================================================

#define LOG_TRACE( module , ...) log_add(0, module , __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG( module , ...) log_add(1, module , __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO( module , ...)  log_add(2, module , __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARM( module , ...)  log_add(3, module , __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR( module , ...) log_add(4, module , __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL( module , ...) log_add(5, module , __FILE__, __LINE__, __VA_ARGS__)

// =============================================================================
//                            FUNCTIONS DECLARATIONS
// =============================================================================
	int     log_init(const char *filename, int interval_ms);
	void    log_finish();
	void    log_add(int level, const char *module, const char *file, int line, const char *msg, ...);

#if __cplusplus
}
#endif

#endif
