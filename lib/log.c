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
 * @Usage: Just add log.h and call log_init anywhere before use it.
 *          At end of application, call log_finish to release resources  
 *============================================================================*/
// =============================================================================
//                                  INCLUDES
// =============================================================================
#include "os_generic.h"
#include "ringbuffer.h"
#include "log.h"


// =============================================================================
//                              DEFINES/MACROS
// =============================================================================
#define LOG_MIN_SAVE_TIME   100
#define LOG_MAX_SAVE_TIME   3000
#define	LOG_BUFFER_SIZE		1000 

// =============================================================================
//                               GLOBAL VARIABLES
// =============================================================================
ringBuffer_typedef(string_t, msg_buffer_t);

char*			    g_LogFileName;
msg_buffer_t	g_data_buffer;
os_spinlock_t	g_LogLock;
int					  g_isInitialized;
int					  g_Kill;
os_thread_ptr_t		g_hThread;
int					  g_saveinterval;


static const char *level_names[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

// =============================================================================
//                            FUNCTIONS DECLARATIONS
// =============================================================================
//
// Save Queue Data to File, Cyclic
//------------------------------------------------------------------------------
int log_dumptoFile(void* arg)
{
	FILE* fp;
	string_t data;

	while (!g_Kill)
	{
		//If queue has data, save it to file
		if (!isBufferEmpty(&g_data_buffer) && g_isInitialized)
		{
			fp = fopen(g_LogFileName, "a");

			if (fp)
			{
				//Save until queue is empty
				while (!isBufferEmpty(&g_data_buffer))
				{
					//Copy Data
					os_spinlock_lock(&g_LogLock);
					bufferRead(&g_data_buffer, data);
					os_spinlock_unlock(&g_LogLock);

					//Write it to File
					fprintf(fp, u8"%s\n", data.c);
				}
			}
			fclose(fp);
		}
		os_sleep(g_saveinterval);
	}

return 1;
}
//
// Init Log Module (Alloc Memory and Create Work Thread)
//------------------------------------------------------------------------------
int log_init(const char *filename, int interval_ms)
{
	if (!filename)
		return -1;

   //Init Locks
  os_spinlock_init(&g_LogLock);

  //Init Buffer
  bufferInit(g_data_buffer, LOG_BUFFER_SIZE, string_t);
   

  //Save File Name
  g_LogFileName = malloc(strlen(filename) + 1);
  strcpy(g_LogFileName, filename);
  
  //Create Save Timer
  if(interval_ms < LOG_MIN_SAVE_TIME)
    interval_ms = LOG_MIN_SAVE_TIME;
  
  if(interval_ms > LOG_MAX_SAVE_TIME)
    interval_ms = LOG_MAX_SAVE_TIME;

  g_saveinterval = interval_ms;

  //Create Dump File Thread
  g_hThread = os_thread_create(log_dumptoFile, NULL, 0);

  g_isInitialized = 1;
  return 1;
}
//
// Finish Log Module (Kill Thread, Free Resources)
//------------------------------------------------------------------------------
void log_finish()
{

  if(g_isInitialized)
  {
    g_Kill = 1;
	os_thread_join(g_hThread);
    //queue_Clear(&g_data_queue);
  }
  
  g_isInitialized = 0;

}
//
// Add new Log Line to queue
//-------------------------------------------------------------------------------
void log_add(int level, const char *module, const char *file, int line, const char *msg, ...){
  
	if(!g_isInitialized)
      return;

  va_list args;
  string_t buf;
  string_t bufTime;
  string_t bufMsg;

  //Get current time
  time_t t = time(NULL);
  struct tm *lt = localtime(&t);
  bufTime.c[strftime(bufTime.c, STR_LEN, u8"%Y-%m-%d %H:%M:%S", lt)] = '\0';

  //Get Mensage
  va_start(args, msg);
  vsnprintf(bufMsg.c, STR_LEN-2 ,msg,args);
  va_end(args);

  //Build Log Mensage
  int size;
  if (level >= 5)
	  size = sprintf(buf.c, u8"%s [%5s] [%5s] %s at ln %d on %s", bufTime.c, level_names[level], module, bufMsg.c, line, file);
  else
	  size = sprintf(buf.c, u8"%s [%5s] [%5s] %s", bufTime.c , level_names[level], module, bufMsg.c);
     
  //Add Msg to Queue
  os_spinlock_lock(&g_LogLock);
	bufferWrite(&g_data_buffer, buf);
  os_spinlock_unlock(&g_LogLock);
  
}
