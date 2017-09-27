#include "simple_logger.h"
#include "PriorityQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*-------------------Priority-------QUEUE---------CLASS-----------------------------------*/

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
void print_status_of_pq(PriorityQueue *pq)
{
	node* cur = pq->head;
	while (cur != NULL)
	{
		printf("%i, ", cur->prior);
		cur = cur->next;
	}
	printf("\n");
}

/**
*@brief Function to take the linked list structure and return a sorted Array from the data || leaves data in container
*@param *pq takes a pointer to a PQ structure to parse into the Array
*@return the pointer to array containing the data
**/
void* pq_toArray(PriorityQueue *pq)
{
	void* ret = malloc((pq->len) * (pq->size));
	char* ptr = (char*)ret;
	for (node* cur = pq->head; cur != NULL; cur = cur->next)
	{
		memcpy(ptr, (cur->data), (pq->size));
		ptr += (pq->size);
	}
	return ret;
}

/**
 *@brief PQ contruction method
 *@param elementSize takes the size of the dereferenced data to MEM ALLOC
 *@return the pointer to a PQ with no elements in the container
**/
PriorityQueue *pq_new(size_t elementSize)
{
	PriorityQueue *new_p = (PriorityQueue *)malloc(sizeof(PriorityQueue));
	new_p->len = 0;
	new_p->size = elementSize;
	new_p->head = NULL;
}

/**
*@brief Method to add a node | calls create node | adds the node by priority
*@param *pq takes the pointer to the PQ structure
*@param *data takes a pointer to the data
*@param priority looks for an integer value as the nodes prioirty
**/
void pq_insert(PriorityQueue *pq, void *data, int priority)
{
	node *newN = create_node(data, priority);
	node* cur = pq->head;
	node* prev = NULL;
	if (!cur)							// Check refernce for Empty Set
	{
		pq->head = newN;
		pq->len++;
		return;
	}
	else if (priority < cur->prior)		// Check for Preappend case
	{
		newN->next = cur;
		pq->head = newN;
		pq->len++;
		return;
	}
	else                               // Find insertion point
	{
		while (cur->next && priority >= cur->next->prior)        //iterate through list || found middle point or hit the trailer
		{
			prev = cur;
			cur = cur->next;
		}
		if (cur->next == NULL)		//stopped because weve hit the trailer  || can go before or after
		{
			if (priority < cur->prior)			//  before current node
			{
				if (prev)
					prev->next = newN;
				pq->len++;
				return;
			}
			else										//  new trailer
			{
				cur->next = newN;
				pq->len++;
				return;
			}
		}
		else                    //stopped between two nodes     ||     triplicate ptr swap
		{

			newN->next = cur->next;
			cur->next = newN;
			pq->len++;
			return;
		}
	}
}

/**
 *@brief Method to remove min || Sorted already will just step the head over
 *@param *pq takes the pointer to the PQ structure
 *@return sends the pointer to the removed node for consumption on the user end
**/
void *pq_delete_min(PriorityQueue *pq)
{
	node *ret = pq->head;
	if (!pq->head)				//check refernence
	{
		return;
	}
	if (pq->head->next)			//multiple item list || decapitate NED S
	{
		pq->head = pq->head->next;
	}
	else						//one item list		||  empty the container
	{
		pq->head = NULL;
	}
	return ret;					//return item for consumption
	
}

/**
 *@brief Method to remove man || Sorted already will just cut the tail
 *@param *pq takes the pointer to the PQ structure
 *@return sends the pointer to the removed node for consumption on the user end
**/
void *pq_delete_max(PriorityQueue *pq) 
{
	if (!pq->head)			//check refernce 
	{
		return;
	}
	node* cur = pq->head;
	node* prev = pq->head;
	while (cur->next)		//iterate to last reference
	{
		if (cur != pq->head)
			prev = prev->next;
		cur = cur->next;
	}
	prev->next = NULL;		//cut the tail
	return cur;				//return former tail for consumption
}

/**
*@brief Method to empty the container || frees the node references data and the node
*@param *pq takes the pointer to the PQ structure
**/
void pq_empty(PriorityQueue *pq)
{
	if (!pq->head)			//check refernce 
	{
		return;
	}
	node* cur = pq->head;
	node* prev = pq->head;
	while (cur->next)		//iterate to last reference || removing the previous as you go
	{
		if (cur != pq->head)
			prev = cur;
		cur = cur->next;
		free_node(prev);
	}
	free_node(cur);
	pq->head = NULL;
}
