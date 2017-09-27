#ifndef __pq__
#define __pq__

#include <stdlib.h>
#include <stdio.h>
#include "node.h"

/*-------------------Priority-------QUEUE---------HEADER--------------------------------*/

//Basic PQ structure, holds the head, the length, and the size of a given element
typedef struct PriorityQueue{
	node *head;
	int len;
	size_t size;
} PriorityQueue;

/**
*@brief Printf the nodes | Used for debugging
*@param *pq takes a pointer to a PQ structure to print to STD OUT
**/
void print_status_of_pq(PriorityQueue *pq);

/**
*@brief Function to take the linked list structure and return a sorted Array from the data || leaves data in container
*@param *pq takes a pointer to a PQ structure to parse into the Array
*@return the pointer to array containing the data
**/
void* pq_toArray(PriorityQueue *pq);

/**
 *@brief PQ contruction method
 *@param elementSize takes the size of the dereferenced data to MEM ALLOC
 *@return the pointer to a PQ with no elements in the container
**/
PriorityQueue *pq_new(size_t elementSize);

/**
*@brief Method to add a node | calls create node | adds the node by priority
*@param *pq takes the pointer to the PQ structure
*@param *data takes a pointer to the data
*@param priority looks for an integer value as the nodes prioirty
**/
void pq_insert(PriorityQueue *pq, void *data, int priority);

/**
 *@brief Method to remove min || Sorted already will just step the head over
 *@param *pq takes the pointer to the PQ structure
 *@return sends the pointer to the removed node for consumption on the user end
**/
void *pq_delete_min(PriorityQueue *pq);

/**
 *@brief Method to remove man || Sorted already will just cut the tail
 *@param *pq takes the pointer to the PQ structure
 *@return sends the pointer to the removed node for consumption on the user end
**/
void *pq_delete_max(PriorityQueue *pq) ;

/**
*@brief Method to empty the container || frees the node references data and the node
*@param *pq takes the pointer to the PQ structure
**/
void pq_empty(PriorityQueue *pq);

#endif
