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

/*---------------------------GAME---MAIN-------------------------------------------------*/

//basic structure of a brick to be used in a PQ implementation
typedef struct
{
    int width;
}Brick;

void draw_stack(Sprite *brick,Vector2D start,Brick *bricklist,unsigned int count)
{
    unsigned int i,j;
    int brickheight = 32;
    int brickwidth = 32;
    Vector2D drawPosition;
	//check pointers
    if (!brick)return;
    if (!bricklist)return;
    for (i = 0; i < count; i++)  //vertical control
    {
        drawPosition.x = start.x - ((bricklist[i].width * brickwidth)/2);
        drawPosition.y = start.y - ((i + 1) * brickheight);
        for (j = 0;j < bricklist[i].width;j++)  //horizontal control
        {
            drawPosition.x += brickwidth;
            gf2d_sprite_draw(
                brick,
                drawPosition,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                0);
		}
    }
}

int main(int argc, char * argv[])
{
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite,*brick;
    static Brick bricklist[] = 
    {
        {2},  
        {7},  
        {1},  
        {5},  
        {14},  
        {9},  
        {13},  
        {24},  
        {16},  
        {22}
    };

	//iterator for looping
	int i = 0;
	//pointer to array of ints
	int *stack;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {0,0,255,200};

	//Initalize a pq
	PriorityQueue *pq = pq_new(sizeof(Brick));
	//populate the priority queue from bricklist
	while (i<10)
	{
		//insert values
		pq_insert(pq, &bricklist[i], bricklist[i].width);
/*used for value checking on STD OUT*/		//	if(i==9) print_status_of_pq(pq);
		i++;
	}


    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(17);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/

    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    brick = gf2d_sprite_load_all("images/brick.png",32,32,16);
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

	//retrieve array from PQ ETL wrapper
	stack = pq_toArray(pq);
	//empty the sorting pq container
	pq_empty(pq);
	//free the conainter
	free(pq);

    /*main game loop*/

    while(!done)
    {
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)		mf = 0;
        
        gf2d_graphics_clear_screen();
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            draw_stack(brick,vector2d(600,700),stack,10);
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();
        if (keys[SDL_SCANCODE_ESCAPE])	done = 1; // exit condition

/*Non self cleaning logs :(*/		//		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
	
	slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
