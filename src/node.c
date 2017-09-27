#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include <stdio.h>

/*----------------------------------NODE------CLASS----------------------------------------------*/

//Basic Node structure with one link, an integer priority, and a void pointer to the data
typedef struct node{
	int prior;
	void *data;
	struct node *next;
} node;

/**
*@brief Method to create a node
*@param *data takes the pointer to the data
*@param prior looks for an integer arguement to describe a given nodes priority
*@return sends the pointer to the newly created node
*/
node *create_node(void *data, int prior)
{
	node *new_node;
	new_node = (node *)malloc(sizeof(node));
	new_node->data = data;
	new_node->prior = prior;
	new_node->next = NULL;
	return new_node;
}

/**
 *@param pointer to the node to be deleted
**/
void free_node(node *bye)
{
	free(bye);
}
