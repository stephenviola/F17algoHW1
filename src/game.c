#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include <stdio.h>
#include "PriorityQueue.h"

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
