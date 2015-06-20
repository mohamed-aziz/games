// Coded by mohamedAziz17 
// medazizknani[at]gmail.com
// Written origanlly in pascal and translated later in c

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>
#include <stdlib.h>

#define FPS            60
#define MAX_POINTS     10
#define LENGTH_PADDLE  100

typedef enum {UP, DOWN, NONE} move;

void init(void);

void destruct(ALLEGRO_DISPLAY *Display, 
               ALLEGRO_EVENT_QUEUE *event_queue,
               ALLEGRO_TIMER *timer);

void gamelogic(int *ballx, int *bally, int *balld, int *balls,
                int *ps1, int *ps2, int *pos1, int *pos2, move p1, move p2);

void draw(int pos1, int  pos2, int ballx, int bally, 
             ALLEGRO_FONT *Font1, int ps1, int ps2);

int randomgen(int max, int min);

int main(void) {
  printf("Coded by mohamedAziz17\n");
  init();
  
  // Setting Display
  ALLEGRO_DISPLAY *Display = NULL;
  Display = al_create_display(700, 500);
  al_set_window_title(Display, "Crazy Pong");
  
  // Setting the event queue and timer
  ALLEGRO_EVENT_QUEUE *event_queue = NULL;
  event_queue = al_create_event_queue();
  ALLEGRO_TIMER *timer = NULL;
  timer  = al_create_timer(1.0/FPS);
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_register_event_source(event_queue, al_get_display_event_source(Display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
      
  // Loading the font 
  ALLEGRO_FONT *Font1 = NULL;
  Font1 = al_load_font("alterebro-pixel-font.ttf", 100, 0);
    
  ALLEGRO_EVENT ev;
  
  // Init the game status
  int closed = 0, redraw=0;
  int scorep1, scorep2;
  scorep1 = scorep2 = 0;
  move p1, p2;
  p1 = p2 = NONE;
  int pos1, pos2;
  pos1 = pos2 = 250;
  int ballx = 350, bally = 250, balls = 10, balld = 45;
  int ps1, ps2;
  ps1 = ps2 = 0;
  
  al_start_timer(timer);
  while (!closed || scorep1 == MAX_POINTS || scorep2 == MAX_POINTS) {
    al_wait_for_event(event_queue, &ev);
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_UP : 
          p2 = UP;
          break;
        case ALLEGRO_KEY_DOWN :
          p2 = DOWN;
          break;
        case ALLEGRO_KEY_A : 
          p1 = UP;
          break;
        case ALLEGRO_KEY_Q :
          p1 = DOWN;
      }
    }
    
    else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
      switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_UP : 
          p2 = NONE;
          break;
        case ALLEGRO_KEY_DOWN :
          p2 = NONE;
          break;
        case ALLEGRO_KEY_A : 
          p1 = NONE;
          break;
        case ALLEGRO_KEY_Q :
          p1 = NONE;
      }      
      
    }
    
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) closed = 1;
    
    else if (ev.type == ALLEGRO_EVENT_TIMER) {
      gamelogic(&ballx, &bally, &balld, &balls, &ps1, &ps2, &pos1, &pos2, p1, p2);
      
      redraw = 1;
      
      // Draw when the time comes ...
      if (redraw && al_is_event_queue_empty(event_queue)) {
        draw(pos1, pos2, ballx, bally, Font1, ps1, ps2);
        redraw = 0;
      }

    } 
  }
 
  printf("Bye :3\n");
  destruct(Display, event_queue, timer);
  return 0;
}

void init(void) {
  al_init();
  al_init_primitives_addon();
  al_init_font_addon();
  al_init_ttf_addon();
  al_install_keyboard();
}

void destruct(ALLEGRO_DISPLAY *Display, 
               ALLEGRO_EVENT_QUEUE *event_queue,
               ALLEGRO_TIMER *timer) {

  al_destroy_display(Display);
  al_destroy_event_queue(event_queue);
  al_destroy_timer(timer);
}

void gamelogic(int *ballx, int *bally, int *balld, int *balls,
                int *ps1, int *ps2, int *pos1, int *pos2, move p1, move p2) {
  
  if ((p2==UP) && (*pos2-10>=10)) *pos2 -= 10;
  else if( (p2==DOWN) && (*pos2+10+LENGTH_PADDLE<=490)) *pos2 += 10;

  if ((p1==UP) && (*pos1-10>=10)) *pos1 -= 10;
  else if ((p1==DOWN)  && (*pos1+10+LENGTH_PADDLE<=490)) *pos1 += 10;

  *ballx += trunc(cos(ALLEGRO_PI / 180 * *balld)* *balls);
  *bally += trunc(sin((ALLEGRO_PI / 180) * (*balld))* (*balls));
  if ((*bally <= 10) || (*bally >= 470))  *balld += (180 - *balld) *2;
  
  // Cheking Collision With paddles 
  if ((*ballx >= 640 && *ballx<=650) 
     && (*bally >= *pos2 && *bally<= *pos2+LENGTH_PADDLE)) {
       *balld += (90 - *balld - randomgen(0, 4)) *2; // adding some randomness :3
       // if the ball were hit in the sides accelerate the ball 
       if ((*bally >= *pos2 && *bally<= *pos2+40) || (*bally >= *pos2+60 && *bally <= *pos2+LENGTH_PADDLE)) *balls = 18;
       else *balls = 10;
      }
  else if (((*ballx<=40) && (*bally>=30)) 
  && ((*bally >= *pos1) && (*bally<= *pos1+LENGTH_PADDLE))) {
       *balld += (90 - *balld - randomgen(0, 4)) *2; // adding some randomness :3
       // if the ball were hit in the sides accelerate the ball 
       if ((*bally >=  *pos1 &&  *bally <= *pos1+40) || (*bally >= *pos1+60 && *bally <= *pos1+LENGTH_PADDLE)) {*balls = 18;}
       else *balls = 10;
     }
     
  if (*ballx>700) {
    *ballx = 350;
    *bally = 250;
    *balld = randomgen(100, 140);
    *balls = 7;
    *ps1 += 1;
  }
  else if (*ballx<0) {
    *ballx = 350;
    *bally = 250;
    *balld = randomgen(100, 330);
    *balls = 7;
    *ps2 += 1;
  }
}

void draw(int pos1, int pos2, int ballx, int bally, 
           ALLEGRO_FONT *Font1, int ps1, int ps2) {
  // Draw board
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_line(10, 0, 690, 0, al_map_rgb(255, 255, 255), 20);
  al_draw_line(10, 500, 690, 500, al_map_rgb(255, 255, 255), 20);
  
  // Draw Players
  al_draw_line(30, pos1, 30, pos1+100, al_map_rgb(255, 255, 255), 30 ); // draw 1st player
  al_draw_line(670, pos2, 670, pos2+100, al_map_rgb(255, 255, 255), 30 ); // draw 2nd player
  // Draws the ball
  al_draw_filled_rectangle(ballx, bally, ballx+20, bally+20, al_map_rgb(255, 255, 255));
  
  // Draw the middle of the pitch
  for (int i=1; i<=20; i++) { 
    al_draw_line(350, i*24, 350, i*24+10, al_map_rgb(255, 255, 255), 10 ); 
  }
  
  // Draw Scores
  char c1[3];
  char c2[3];
  sprintf(c1, "%d", ps1);
  sprintf(c2, "%d", ps2);

  // printf(c2);
  al_draw_text(Font1, al_map_rgb(255, 255, 255), 280, 30, ALLEGRO_ALIGN_CENTER, c1);
  al_draw_text(Font1, al_map_rgb(255, 255, 255), 420, 30, ALLEGRO_ALIGN_CENTER, c2);
  
  al_flip_display();
}

int randomgen(int max, int min) {
  // as suggested here 
  // http://stackoverflow.com/questions/2509679/how-to-generate-a-random-number-from-within-a-range
  
  int r;
  r = (rand() % (max+1-min))+min;
  return r;
}
