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
// =============================================================================
//                                  INCLUDES
// =============================================================================
#include "queue.h"

// =============================================================================
//                            PUBLIC FUNCTIONS
// =============================================================================

void queue_Init(queue_list_t *q){
   q->sizeOfQueue = 0;
   q->head = q->tail = NULL;
}

int queue_Push(queue_list_t *q, const void *data, size_t datasize){
    queue_element_t *newElement = (queue_element_t*)malloc(sizeof(queue_element_t));
	memset(newElement, 0, sizeof(queue_element_t));

    if(newElement == NULL)
    {
        return 0;
    }

	if(datasize != 0){
		newElement->datasize = datasize;
    	newElement->data = malloc(datasize);
	}

    if(newElement->data == NULL)
    {
        free(newElement);
        return 0;
    }

    newElement->next = NULL;

    memcpy(newElement->data, data, newElement->datasize);

    if(q->sizeOfQueue == 0)
    {
        q->head = q->tail = newElement;
    }
    else
    {
        q->tail->next = newElement;
        q->tail = newElement;
    }

    q->sizeOfQueue++;
    return 0;
}

void queue_Pop(queue_list_t *q, void *data){
    if(q->sizeOfQueue > 0)
    {
        queue_element_t *temp = q->head;
        memcpy(data, temp->data, temp->datasize);

        if(q->sizeOfQueue > 1)
        {
            q->head = q->head->next;
        }
        else
        {
            q->head = NULL;
            q->tail = NULL;
        }

        q->sizeOfQueue--;
        free(temp->data);
        free(temp);
    }
}

void queue_Peek(queue_list_t *q, void *data, /*out*/ size_t*datasize){
    if(q->sizeOfQueue > 0)
    {
       queue_element_t *temp = q->head;
	   *datasize = temp->datasize;
       memcpy(data, temp->data, temp->datasize);
    }
}

void queue_Clear(queue_list_t *q){
  queue_element_t *temp;

  while(q->sizeOfQueue > 0)
  {
      temp = q->head;
      q->head = temp->next;
      free(temp->data);
      free(temp);
      q->sizeOfQueue--;
  }

  q->head = q->tail = NULL;
}

int queue_GetSize(queue_list_t *q){
    return q->sizeOfQueue;
}
