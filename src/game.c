#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

typedef struct node{
	int prior;
	void *data;
	node *next;
} node;

typedef struct PriorityQueue{
	node *head;
	int len;
	size_t size;
} PriorityQueue;

PriorityQueue *pq_new(size_t elementSize)
{
	new_p *new_p = (PriorityQueue *)malloc(sizeof(PriorityQueue));
	new_p->len = 0;
	new_p->size = elementSize;
	new_p->head = NULL;
}

void* pq_toArray(PriorityQueue *pq)
{
	void* ret = malloc((pq->len) * (pq->size));
	char* ptr = (char*)ret;
	for (node* cur = pq->head; cur->next; cur = cur->next)
	{
		memcpy(ptr, cur->data, (pq->size));
		ptr += (pq->size);
	}
	return ret;
}

node *create_node(void *data, int prior)
{
	node *new_node;
	new_node = (node *)malloc(sizeof(node));
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

void *pq_delete_min(PriorityQueue *pq)
{
	node *ret = pq->head;
	head = head->next;
}

void *pq_delete_max(PriorityQueue *pq) 
{
	node* cur = head;
	node* prev = head;
	while (cur->next)
	{
		if (cur != head)
			prev = prev->next;
		cur = cur->next;
	}
	prev->next = NULL;
	return cur;
}

void pq_insert(PriorotyQueue *pq, void *data, int priority)
{
	node *newN = create_node(data, priority);
	node* cur = head;
	node* prev = head;
	if (!cur) {
		head = newN;
	}
	else{
		while (cur->next && cur->next->prior <= priority) 
		{
			if (cur != head)
				prev = prev->next;
			cur = cur->next;
		}
		if (cur->prior > priority) 
		{
			prev->next = newN;
			newN->next = cur;
		}
		else
		{
			cur->next = newN;
		}
		
	}
}


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    
    /*program initializtion*/
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
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
