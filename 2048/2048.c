#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>


int main(void){
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    ALLEGRO_DISPLAY *Display = NULL;

    Display = al_create_display(500, 500);
    al_set_window_title(Display, "2048");


    return 0;
}
