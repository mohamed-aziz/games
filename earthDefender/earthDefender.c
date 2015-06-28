#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

#define Height 700
#define Width  400
#define FPS    60

struct Ship {
    int x, y;
    int health;
    //ALLEGRO_BITMAP *img;
};

struct Pos {

    int UP, DOWN, RIGHT, LEFT;
};

 // we will be implementing a simple asteroid system no vectors or dynamic arrays :v

typedef struct {
    int x, y; // x, y positio,
    int live ;  // boolean checks if asteroid is live
} asteroid;

void init_all(void) ;

void destruct( void );

void asteroid_init(asteroid *asteroids);

void asteroid_spawn(asteroid *asteroids, int *asteroid_size);

int main( void  ) {
    init_all();   
    struct Pos moves;
    struct Ship myShip;
    int asteroid_number = 10;  
    asteroid *asteroids[asteroid_size];

    moves.UP = moves.DOWN = moves.LEFT = moves.RIGHT = 0;
    myShip.x = 0;
    myShip.y = Width / 2;

    asteroid_init(asteroids);

    //myShip.img = al_load_bitmap("ship.png");
    ALLEGRO_BITMAP *img = al_load_bitmap("ship1.png");
    ALLEGRO_BITMAP *bg  = al_load_bitmap("bg1.png"); 
    ALLEGRO_BITMAP *earth = al_load_bitmap("earth1.png");
    ALLEGRO_DISPLAY *Display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    Display = al_create_display(Height, Width);
    event_queue = al_create_event_queue();    
    timer    = al_create_timer(1.0/FPS);

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(Display));


    int closed = 0; 
    
 
    int redraw = 0;
    al_start_timer(timer);
    while (!closed) {
        ALLEGRO_EVENT ev;//printf("Here"); 
        al_wait_for_event(event_queue, &ev);
        
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            closed = 1;
        }

        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_K : moves.UP = 1;
                                     break;
                case ALLEGRO_KEY_J : moves.DOWN = 1;
                                     break;
                case ALLEGRO_KEY_L : moves.RIGHT = 1;
                                     break;
                case ALLEGRO_KEY_H : moves.LEFT = 1;
                                      
            }
        }
    
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) { 
                case ALLEGRO_KEY_K : moves.UP = 0;
                                     break;
                case ALLEGRO_KEY_J : moves.DOWN = 0;
                                     break;
                case ALLEGRO_KEY_L : moves.RIGHT = 0;
                                     break;
                case ALLEGRO_KEY_H : moves.LEFT = 0;
                                    
            } 
        }

        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (moves.UP && myShip.y>7) {
                myShip.y -= 10;
            }
            else if (moves.DOWN && myShip.y<Width-42) {
                myShip.y += 10;
            } 

            else if (moves.LEFT && myShip.x>0) {
                myShip.x -= 10;
            }

            else if (moves.RIGHT && myShip.x<Height-42) {
                myShip.x += 10;
            }
            // printf("%i  :   %i\n", myShip.x, myShip.y); 
            redraw = 1;

            if (redraw && al_event_queue_is_empty(event_queue)) {
                
                al_draw_bitmap(bg, 0, 0, 0);
                al_draw_bitmap(earth, -300, 0, 0);
                al_draw_bitmap(img, myShip.x, myShip.y, 0); 
                al_flip_display();
                redraw = 0;
            } 
        }
    } 
    al_destroy_display(Display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    return 0;
}

void asteroid_init(asteroid *asteroids, int ast_size) {
   
   for (int i=0; i<ast_size; i++) {
        asteroids[i].x =  asteroids[i].y =  asteroids[i].live = 0;
   
   } 

}


void asteroid_spawn(asteroid *asteroids, int ast_size) {

    for (int i=0; i<ast_size; i++) {
        if (asteroids)
    
    }
}


void asteroid_incspot(asteroid *asteroids, int *ast_size) {

    *ast_size += 1;
    // Duh
    asteroids[*ast_size].x =  asteroids[*ast_size].y =  asteroids[*ast_size].live = 0;

}

void asteroid_draw(asteroid *asteroids, int ast_size) {
    

}

void init_all(void) {

    al_init();
    al_install_keyboard();
    al_init_image_addon();
}

