/*=============================================================================
 * Copyright (c) 2018 rafaelkrause
 *
 * @Script: queue.h
 * @Author: rafaelkrause
 * @Email: rafaelkrause@gmail.com
 * @Create At: 2018-08-17 15:19:42
 * @Last Modified By: rafaelkrause
 * @Last Modified At: 
 * @Description: Simple Queue Implementation
 *    Orignal from https://codereview.stackexchange.com/questions/141238/implementing-a-generic-queue-in-c
 *    Changed Names and queuedata (to allow diferents data sizes)
 *============================================================================*/

#ifndef _QUEUE_H_
#define _QUEUE_H_
#if __cplusplus
extern "C" {
#endif


#include "os_generic.h"
	//Queue Element (Or node)
	typedef struct queue_element {
		void 				*data;
		size_t 			datasize;
		struct queue_element 	*next;
	}queue_element_t;

	//Queue List (All Elements Grouped)
	typedef struct queue_list {
		size_t			sizeOfQueue;
		queue_element_t *head;
		queue_element_t *tail;
	}queue_list_t;

	// =============================================================================
	//                            PUBLIC FUNCTIONS
	// =============================================================================

	void    queue_Init(queue_list_t *q);
	int     queue_Push(queue_list_t *q, const void *data, size_t datasize);
	void    queue_Pop(queue_list_t *q, void *data);
	void    queue_Peek(queue_list_t *q, void *data, /*out*/ size_t*datasize);
	void    queue_Clear(queue_list_t *q);
	int     queue_GetSize(queue_list_t *q);

#if __cplusplus
}
#endif

#endif