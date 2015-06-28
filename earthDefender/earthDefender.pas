program earthDefender;

uses Allegro5, Al5image, al5primitives, Al5ttf, Al5font,
   Sysutils;
const
  HEIGHT = 700;
  WIDTH  = 400;
  FPS    = 60;

type 
  ship = record 
    {mmm x,y and health and live}
    x, y   : integer;
    health : byte;
    live   : boolean; 
  end;

  asteroid = record 
   {  mmm an x, y position a speed and a live status }
   x, y : integer;
   speed : integer;
   live : boolean;
  end;
  rocket = record 
   {  mmm an x, y position a speed and a live status }
   x, y  : integer;
   live  : boolean;
  end;
  ast = array of asteroid ; { Yees tha's how we declare a dynamic array easy right ??  }
  roc = array [0..10] of rocket;
Var
  Display     : ALLEGRO_DISPLAYptr;
  timer       : ALLEGRO_TIMERptr;
  event_queue : ALLEGRO_EVENT_QUEUEptr;
  ev          : ALLEGRO_EVENT;
  myShip      : ship;
  shipimg, earth,
  bg, asteroid_img, 
  rocketimg: ALLEGRO_BITMAPptr;
  asteroids : ast;  
  asteroids_num : integer = 5; 
  earth_health  : integer = 20;
  asteroid_max_speed : integer = 3;
  rockets : roc;
  score  : integer;
  Font  : ALLEGRO_FONTptr;
procedure draw(); forward;

procedure asteroid_init(var asteroids: ast; asteroids_num : integer); forward;

procedure asteroid_spawn(var asteroids : ast; asteroids_num : integer); forward;
procedure  asteroid_update(var asteroids : ast; asteroids_num : integer); forward;
procedure  asteroid_draw(asteroids : ast; asteroids_num : integer) ; forward;
procedure asteroid_collision(var asteroids : ast ; asteroids_num : integer; var earth_health : integer); forward;

procedure rocket_fire(var rockets : roc; xship, yship : integer); forward; 
procedure rocket_update(var rockets : roc) ; forward;
procedure rocket_draw(rockets : roc) ; forward; 
procedure rocket_init (var rockets : roc) ; forward;
procedure rocket_collision(var rockets : roc ; var asteroids : ast; asteroids_num : integer; var score : integer) ; forward; 
procedure ship_collision(var myShip : ship; var asteroids : ast; asteroids_num : integer); forward;

procedure init_all() ;
begin
  randomize;
  al_init();

  al_install_keyboard();
  al_init_image_addon();
  al_init_primitives_addon();
  al_init_font_addon();
  al_init_ttf_addon();
  Display := al_create_Display(HEIGHT, WIDTH);

  timer   := al_create_timer(1.0/FPS);
  event_queue := al_create_event_queue();

  al_register_event_source(event_queue, al_get_keyboard_event_source()); 
  al_register_event_source(event_queue, al_get_display_event_source(Display)); 
  al_register_event_source(event_queue, al_get_timer_event_source(timer)); 
  
  Setlength(asteroids, asteroids_num);
  

  asteroid_init(asteroids , asteroids_num);  
  earth    := al_load_bitmap('earth1.png'); 
  shipimg  :=  al_load_bitmap('ship1.png');
  bg       := al_load_bitmap('bg1.png');
  asteroid_img := al_load_bitmap('asteroid1.png');
  rocketimg := al_load_bitmap('rocket1.png');
  myship.x := 200;
  myShip.y := WIDTH div 2;
  rocket_init(rockets);
  myShip.live := TRUE;
  Font := al_load_font('sb.ttf', 72, 0)
end;

{ hmm let's work on the earth health  and scoring system }
procedure mainloop();
type 
  moves = (UP, DOWN, LEFT, RIGHT, NONE);
var 
  closed,
  redraw   : boolean;
  Tmoves : moves;
  c : integer;
begin
  Tmoves := NONE;
  al_start_timer(timer);
  closed := FALSE;


  while not(closed) do begin 
    al_wait_for_event(event_queue, ev); 
    
    if (ev._type = ALLEGRO_EVENT_DISPLAY_CLOSE) then closed  := TRUE
    else if (ev._type = ALLEGRO_EVENT_KEY_DOWN) then 
      case ev.keyboard.keycode of 
        ALLEGRO_KEY_H : Tmoves := LEFT;
        ALLEGRO_KEY_J : Tmoves := DOWN;
        ALLEGRO_KEY_K : Tmoves := UP;
        ALLEGRO_KEY_L : Tmoves := RIGHT;
        ALLEGRO_KEY_SPACE : rocket_fire(rockets, myship.x, myship.y);
      end
    else if (ev._type = ALLEGRO_EVENT_KEY_UP) then 
      case ev.keyboard.keycode of 
        ALLEGRO_KEY_H : Tmoves := NONE;
        ALLEGRO_KEY_J : Tmoves := NONE;
        ALLEGRO_KEY_K : Tmoves := NONE;
        ALLEGRO_KEY_L : Tmoves := NONE;

      end
    else if (ev._type = ALLEGRO_EVENT_TIMER) then begin 
      if (Tmoves = LEFT) and (myShip.x>0) then 
        myShip.x -= 10;
      if (Tmoves = DOWN) and (myShip.y<WIDTH-42) then 
        myShip.y += 10;
      if (Tmoves = UP) and (myShip.y>7) then
        myShip.y -= 10; 
      if (Tmoves = RIGHT) and (myShip.x<HEIGHT-42) then 
        myShip.x += 10;
      
      asteroid_spawn(asteroids, asteroids_num);
      
      asteroid_update(asteroids, asteroids_num);
      
      asteroid_collision(asteroids, asteroids_num, earth_health);

      rocket_update(rockets);
      
      rocket_collision(rockets, asteroids, asteroids_num, score);

      ship_collision(myShip, asteroids, asteroids_num);
      { So it Will maybe 1 second 60 frame ok}
  
      if not(myShip.live) then
        c+=1;
      if c=60 then begin
        myShip.live := TRUE;
        c:=0;
        myShip.x := 200;
        myShip.y := WIDTH div 2;
      end;
     
      redraw := TRUE;
    
   
      if ( redraw) and (al_is_event_queue_empty(event_queue)) then begin
        draw();
        redraw := FALSE;
      end;
    end; 
  end;  
end;

procedure destroy(); 
begin
  al_destroy_bitmap(shipimg);
  al_destroy_bitmap(earth);
  al_destroy_bitmap(bg);
  al_destroy_event_queue(event_queue);
  al_destroy_timer(timer);
  al_destroy_display(Display);
end;
 
{ it should have a spawn, update, add asteroid for harder level and draw ,  umm and collision procedure }

procedure draw(); 
begin

  al_draw_bitmap(bg, 0, 0, 0);
  al_draw_bitmap(earth, -300, 0, 0);
  { Weired bug o.O }
  if myShip.live then al_draw_bitmap(shipimg, myShip.x, myShip.y, 0);
  asteroid_draw(asteroids, asteroids_num);
  
  al_draw_filled_rectangle((WIDTH / 2)-10, 340, (WIDTH / 2) + 210, 370, al_map_rgb(255, 201, 175));
  al_draw_filled_rectangle((WIDTH / 2), 340, (WIDTH / 2) + (earth_health * 10) , 370, al_map_rgb(255, 59, 70));
  rocket_draw(rockets);
  
  al_draw_text(Font, al_map_rgb(255, 59, 70), 100, 20, ALLEGRO_ALIGN_CENTER, InttoStr(score));
  al_flip_display();
end;



procedure asteroid_init (var asteroids : ast ; asteroids_num : integer) ;
Var 
  i : integer;
begin   
  for i:=0 to asteroids_num do
   with asteroids[i ] do begin 
     live :=FALSE;
     x    := HEIGHT;
     y    := random(WIDTH) + 1 ;
     speed := asteroid_max_speed - random(3) ; { Well that's anti-physics but who cares about physics at least not me -_- }
      { the asteroids don't have the same speed beacause that will seem ugly }
   end; 
end; 

procedure asteroid_spawn(var asteroids : ast; asteroids_num : integer);
Var 
  i : integer;
begin
   for i:=0 to asteroids_num-1 do 
     if not(asteroids[ i  ].live) then begin
       asteroids[ i ].live := TRUE; 
       { oh yes :3 }
       asteroids[i].x := HEIGHT ;
        asteroids[i].y    := random(WIDTH) + 1 ;
       //break;
      end;
   
end;

procedure  asteroid_update(var asteroids : ast; asteroids_num : integer);
Var 
  i : integer;
begin   
  for i:=0 to   asteroids_num-1 do 
    if asteroids[ i  ].live then 
      asteroids[ i ].x -= asteroids[i].speed;
end; 

procedure asteroid_collision(var asteroids : ast; asteroids_num : integer; var earth_health : integer);
var
   i : integer;
begin 
  for i:=0 to asteroids_num-1 do 
    with asteroids[i] do begin 
    { the asteroid can touch the eaerth or just pass }
      if (x<=100) and ((y >=20) and (y<=380)) then begin 
        earth_health -=1;
        live := FALSE;
      end;
      if (x<=-32) then 
        live := FALSE;
    end;
end;

procedure asteroid_draw(asteroids : ast; asteroids_num : integer);
Var 
  i : integer;
begin 
  for i:=0 to asteroids_num -1 do 
    with asteroids[ i ] do  
       if live then 
         al_draw_bitmap(asteroid_img, x, y , 0);
end; 


procedure rocket_init (var rockets : roc) ;
Var 
  i : integer;
begin   
  for i:=0 to 10 do
   with rockets[i ] do begin 
     live :=FALSE;
     x    := 0;
     y    := 0;
 
   end; 
end; 

procedure rocket_fire(var rockets : roc; xship, yship : integer);
var 
  i : byte;
begin
  for i:=0 to 10 do 
    if not(rockets[i].live) then begin
      rockets[ i ].live := TRUE; 
      rockets[ i ].x    := xship;
      rockets[ i ].y := yship;
      // break; 
    end;
end;

procedure rocket_update(var rockets : roc ) ; 
var 
  i : byte;
begin
  for i:=0 to 10 do
    if rockets[i].live  then 
      rockets[ i].x += 20;
end; 


procedure rocket_draw(rockets : roc) ;  
Var 
  i : integer;
begin 
  for i:=1 to 10 do 
    with rockets[ i ] do  
       if live then 
         al_draw_bitmap(rocketimg, x, y , 0);
end; 

procedure rocket_collision(var rockets : roc; var asteroids : ast; asteroids_num : integer;var  score : integer);
var
   i, j : integer;
begin 
  for i:=0 to 10 do 
    for j:= 0 to asteroids_num-1 do begin 
 
    if (rockets[i].x>=asteroids[j].x) and ((rockets[i].y >=asteroids[j].y-16) and (rockets[i].y<=asteroids[j].y+16)) then begin 
        rockets[i].live := FALSE;
        asteroids[j].live := FALSE;
        {  it won't grow rapidally like this }
        score += (score mod 10) + 1
      end;
      if (rockets[i].x>=HEIGHT+48) then 
        rockets[i].live := FALSE;
    end;

end;

procedure ship_collision(var myship : ship; var asteroids : ast; asteroids_num : integer);
var 
  i : integer;
begin
  for i:=0 to asteroids_num-1 do begin 
    if (myShip.x=asteroids[i].x) and (( myShip.y>=asteroids[i].y-16) and ( myShip.y <= asteroids[i].y +16 )) then begin 
       myShip.live := FALSE;
       asteroids[i].live := FALSE;
    end;
  end;
end;




BEGIN
  init_all() ;
  
  mainloop ();

  destroy();
END.
