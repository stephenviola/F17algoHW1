#ifndef __node__
#define __node__

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
node *create_node(void *data, int prior);

/**
 *@param pointer to the node to be deleted
**/
void free_node(node *bye);


#endif
