#include<allegro.h>
#include<alpng.h>
#include<time.h>

#define EXIT 0
#define MENU 1
#define GAME 2
#define SETUP 3
#define OPTIONS 4

#define player 1
#define ai 2

#define player_start 1
#define ai_start 2
#define random_start 3


#define playing 1
#define ai_win  2
#define player_win 3

#define none 0
#define three_pile_selected 1
#define five_pile_selected 2
#define seven_pile_selected 3

//Handles the state of the game, menu, game, exiting
int GAME_STATE;
//Sets the Gui scale of the game
int GUI_SCALE;

//Game ending
int game_ending = playing;
int ai_turn_delay_incrementer = 0;
int ai_turn_delay = 225;
int turn_start = player_start;

bool autoset_gui_scale = true;

//Declare bitmaps
BITMAP* title_screen;
BITMAP* play_button;
BITMAP* cursor;
BITMAP* game_background;
BITMAP* stone_1;
BITMAP* stone_2;
BITMAP* settings;
BITMAP* player_turn;
BITMAP* ai_turn;
BITMAP* slider;
BITMAP* knob;
BITMAP* box;
BITMAP* box_selected;
BITMAP* start_button;
BITMAP* slider_gui_scale;

BITMAP* buffer;

//Declare fonts
FONT* font_48;
FONT* font_34;
FONT* font_24;
FONT* font_14;
FONT* font_10;
FONT* font_20;

//Declare game variables
int turn;
int turn_init;
int pile_selected;


int three_pile_turn=3;
int five_pile_turn=5;
int seven_pile_turn=7;


int three_pile=3;
int five_pile=5;
int seven_pile=7;

int click_incrementer;

//A function that handles error messages
void abort_on_error(const char *message){
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}

//A function for collision, used for mouse clicking
bool collision(int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2)
{
  if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1){
    return true;
  }
  return false;
}

//Random number function
int random(int newLowest, int newHighest)
{
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
}

//Function called when the AI makes a move
void end_ai_turn(){
     if(three_pile+five_pile+seven_pile==0){
        game_ending=player_win;
      }else{
        turn=player;
        turn_init=0;
        ai_turn_delay_incrementer=0;
      }
}

//Update loop, is ran every frame
void update(){

        //Debug code, keep scrolling
        if(key[KEY_1])GUI_SCALE=1;
        if(key[KEY_2])GUI_SCALE=2;
        if(key[KEY_3])GUI_SCALE=3;
        if(key[KEY_4])GUI_SCALE=4;
        if(key[KEY_5])GUI_SCALE=5;
        if(key[KEY_6])GUI_SCALE=6;
        if(key[KEY_7])GUI_SCALE=7;
        if(key[KEY_8])GUI_SCALE=8;
        if(key[KEY_9])GUI_SCALE=9;
        if(key[KEY_0])GUI_SCALE=10;

        //Options game loop
        if(GAME_STATE==OPTIONS){
            stretch_sprite(buffer,title_screen,0,0,SCREEN_W,SCREEN_H);

            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,20,20,makecol(0,0,0),-1,"Options");
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,20,20,makecol(0,0,0),-1,"Options");
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,20,20,makecol(0,0,0),-1,"Options");
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,20,20,makecol(0,0,0),-1,"Options");
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,20,20,makecol(0,0,0),-1,"Options");


            if(autoset_gui_scale)stretch_sprite(buffer,box_selected,30,(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),300/GUI_SCALE,300/GUI_SCALE);
            if(!autoset_gui_scale)stretch_sprite(buffer,box,30,(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),300/GUI_SCALE,300/GUI_SCALE);

            if(autoset_gui_scale && click_incrementer>10 && mouse_b & 1 && collision(mouse_x,mouse_x,30,30+(300/GUI_SCALE), mouse_y,mouse_y,(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),(SCREEN_H-(350/GUI_SCALE))+((300/GUI_SCALE)-(1200/GUI_SCALE)))){
                    autoset_gui_scale=false;
                    click_incrementer=0;
            }
            if(!autoset_gui_scale && click_incrementer>10 && mouse_b & 1 && collision(mouse_x,mouse_x,30,30+(300/GUI_SCALE), mouse_y,mouse_y,(SCREEN_H-(700/GUI_SCALE))-(1200/GUI_SCALE),(SCREEN_H-(700/GUI_SCALE))+((300/GUI_SCALE)-(1200/GUI_SCALE)))){
                    autoset_gui_scale=true;
                    click_incrementer=0;
            }

            textprintf_ex(buffer,font_14,20,SCREEN_H-150,makecol(0,0,0),-1,"GUI Scale:%i",GUI_SCALE);
            draw_sprite(buffer,slider_gui_scale,25,SCREEN_H-50);
            draw_sprite(buffer,knob,(GUI_SCALE*50)-25,SCREEN_H-65);

            if(mouse_b & 1 && collision(mouse_x,mouse_x,(GUI_SCALE*50)-100,(GUI_SCALE*50)+100, mouse_y,mouse_y,SCREEN_H-100,SCREEN_H)){
                if(mouse_x>49 && mouse_x<500)GUI_SCALE=mouse_x/50;
                if(mouse_x<50)GUI_SCALE=1;
                if(mouse_x>499)GUI_SCALE=9;
            }


        }

        //Game setup loop
        if(GAME_STATE==SETUP){
            stretch_sprite(buffer,title_screen,0,0,SCREEN_W,SCREEN_H);
            draw_sprite(buffer,slider,25,SCREEN_H-50);
            draw_sprite(buffer,knob,ai_turn_delay-25,SCREEN_H-65);

            stretch_sprite(buffer,start_button,(SCREEN_W-(1440/GUI_SCALE))-10,10,(1440/GUI_SCALE)-10,(428/GUI_SCALE)+10);

            if(mouse_b & 1 && collision(mouse_x,mouse_x,(SCREEN_W-(1440/GUI_SCALE))-10,(((SCREEN_W-(1440/GUI_SCALE))-10)+((1440/GUI_SCALE)-10)), mouse_y,mouse_y,10,(428/GUI_SCALE)+10)){
                GAME_STATE=GAME;
                if(turn_start==player_start){turn=player; turn_init=0;}
                if(turn_start==ai_start)turn=ai;
                if(turn_start==random_start){
                    if(random(1,2)==1)turn=player;
                    else turn=ai;
                }


            }

            if(turn_start==player_start)stretch_sprite(buffer,box_selected,30,(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),300/GUI_SCALE,300/GUI_SCALE);
            if(turn_start!=player_start)stretch_sprite(buffer,box,30,(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),300/GUI_SCALE,300/GUI_SCALE);
            if(turn_start==ai_start)stretch_sprite(buffer,box_selected,30,(SCREEN_H-(700/GUI_SCALE))-(1200/GUI_SCALE),300/GUI_SCALE,300/GUI_SCALE);
            if(turn_start!=ai_start)stretch_sprite(buffer,box,30,(SCREEN_H-(700/GUI_SCALE))-(1200/GUI_SCALE),300/GUI_SCALE,300/GUI_SCALE);
            if(turn_start==random_start)stretch_sprite(buffer,box_selected,30,(SCREEN_H-(1050/GUI_SCALE))-(1200/GUI_SCALE),300/GUI_SCALE,300/GUI_SCALE);
            if(turn_start!=random_start)stretch_sprite(buffer,box,30,(SCREEN_H-(1050/GUI_SCALE))-(1200/GUI_SCALE),300/GUI_SCALE,300/GUI_SCALE);
            if(mouse_b & 1 && collision(mouse_x,mouse_x,30,30+(300/GUI_SCALE), mouse_y,mouse_y,(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),(SCREEN_H-(350/GUI_SCALE))+((300/GUI_SCALE)-(1200/GUI_SCALE))))turn_start=player_start;
            if(mouse_b & 1 && collision(mouse_x,mouse_x,30,30+(300/GUI_SCALE), mouse_y,mouse_y,(SCREEN_H-(700/GUI_SCALE))-(1200/GUI_SCALE),(SCREEN_H-(700/GUI_SCALE))+((300/GUI_SCALE)-(1200/GUI_SCALE))))turn_start=ai_start;
            if(mouse_b & 1 && collision(mouse_x,mouse_x,30,30+(300/GUI_SCALE), mouse_y,mouse_y,(SCREEN_H-(1050/GUI_SCALE))-(1200/GUI_SCALE),(SCREEN_H-(1050/GUI_SCALE))+((300/GUI_SCALE)-(1200/GUI_SCALE))))turn_start=random_start;

            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,35+(300/GUI_SCALE),(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"Player Start");
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,35+(300/GUI_SCALE),(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"Player Start");
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,35+(300/GUI_SCALE),(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"Player Start");
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,35+(300/GUI_SCALE),(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"Player Start");
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,35+(300/GUI_SCALE),(SCREEN_H-(350/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"Player Start");

            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,35+(300/GUI_SCALE),(SCREEN_H-(700/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"AI Start");
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,35+(300/GUI_SCALE),(SCREEN_H-(700/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"AI Start");
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,35+(300/GUI_SCALE),(SCREEN_H-(700/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"AI Start");
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,35+(300/GUI_SCALE),(SCREEN_H-(700/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"AI Start");
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,35+(300/GUI_SCALE),(SCREEN_H-(700/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"AI Start");

            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,35+(300/GUI_SCALE),(SCREEN_H-(1050/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"Random Start");
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,35+(300/GUI_SCALE),(SCREEN_H-(1050/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"Random Start");
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,35+(300/GUI_SCALE),(SCREEN_H-(1050/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"Random Start");
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,35+(300/GUI_SCALE),(SCREEN_H-(1050/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"Random Start");
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,35+(300/GUI_SCALE),(SCREEN_H-(1050/GUI_SCALE))-(1200/GUI_SCALE),makecol(0,0,0),-1,"Random Start");



            if(mouse_b & 1 && collision(mouse_x,mouse_x,ai_turn_delay-50,ai_turn_delay+50, mouse_y,mouse_y,SCREEN_H-100,SCREEN_H)){
                if(mouse_x>49 && mouse_x<500)ai_turn_delay=mouse_x;
                if(mouse_x<50)ai_turn_delay=50;
                if(mouse_x>499)ai_turn_delay=500;
            }
            int ai_turn_delay_real_value=ai_turn_delay-50;
            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,20,SCREEN_H-(800/GUI_SCALE),makecol(0,0,0),-1,"AI Thinking Time:%i",ai_turn_delay_real_value);
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,20,SCREEN_H-(800/GUI_SCALE),makecol(0,0,0),-1,"AI Thinking Time:%i",ai_turn_delay_real_value);
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,20,SCREEN_H-(800/GUI_SCALE),makecol(0,0,0),-1,"AI Thinking Time:%i",ai_turn_delay_real_value);
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,20,SCREEN_H-(800/GUI_SCALE),makecol(0,0,0),-1,"AI Thinking Time:%i",ai_turn_delay_real_value);
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,20,SCREEN_H-(800/GUI_SCALE),makecol(0,0,0),-1,"AI Thinking Time:%i",ai_turn_delay_real_value);

            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,90/GUI_SCALE,180/GUI_SCALE,makecol(0,0,0),-1,"Left click: Remove stone.");
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,90/GUI_SCALE,180/GUI_SCALE,makecol(0,0,0),-1,"Left click: Remove stone.");
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,90/GUI_SCALE,180/GUI_SCALE,makecol(0,0,0),-1,"Left click: Remove stone.");
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,90/GUI_SCALE,180/GUI_SCALE,makecol(0,0,0),-1,"Left click: Remove stone.");
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,90/GUI_SCALE,180/GUI_SCALE,makecol(0,0,0),-1,"Left click: Remove stone.");

            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,90/GUI_SCALE,(180/GUI_SCALE)*3,makecol(0,0,0),-1,"Right click: Reset turn.");
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,90/GUI_SCALE,(180/GUI_SCALE)*3,makecol(0,0,0),-1,"Right click: Reset turn.");
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,90/GUI_SCALE,(180/GUI_SCALE)*3,makecol(0,0,0),-1,"Right click: Reset turn.");
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,90/GUI_SCALE,(180/GUI_SCALE)*3,makecol(0,0,0),-1,"Right click: Reset turn.");
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,90/GUI_SCALE,(180/GUI_SCALE)*3,makecol(0,0,0),-1,"Right click: Reset turn.");

            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,90/GUI_SCALE,(180/GUI_SCALE)*5,makecol(0,0,0),-1,"Space: Finish turn.");
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,90/GUI_SCALE,(180/GUI_SCALE)*5,makecol(0,0,0),-1,"Space: Finish turn.");
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,90/GUI_SCALE,(180/GUI_SCALE)*5,makecol(0,0,0),-1,"Space: Finish turn.");
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,90/GUI_SCALE,(180/GUI_SCALE)*5,makecol(0,0,0),-1,"Space: Finish turn.");
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,90/GUI_SCALE,(180/GUI_SCALE)*5,makecol(0,0,0),-1,"Space: Finish turn.");

            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,90/GUI_SCALE,(180/GUI_SCALE)*7,makecol(0,0,0),-1,"Enter: Reset Game.");
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,90/GUI_SCALE,(180/GUI_SCALE)*7,makecol(0,0,0),-1,"Enter: Reset Game.");
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,90/GUI_SCALE,(180/GUI_SCALE)*7,makecol(0,0,0),-1,"Enter: Reset Game.");
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,90/GUI_SCALE,(180/GUI_SCALE)*7,makecol(0,0,0),-1,"Enter: Reset Game.");
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,90/GUI_SCALE,(180/GUI_SCALE)*7,makecol(0,0,0),-1,"Enter: Reset Game.");

            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,90/GUI_SCALE,(180/GUI_SCALE)*9,makecol(0,0,0),-1,"ESC: Pause/Exit Game.");
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,90/GUI_SCALE,(180/GUI_SCALE)*9,makecol(0,0,0),-1,"ESC: Pause/Exit Game.");
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,90/GUI_SCALE,(180/GUI_SCALE)*9,makecol(0,0,0),-1,"ESC: Pause/Exit Game.");
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,90/GUI_SCALE,(180/GUI_SCALE)*9,makecol(0,0,0),-1,"ESC: Pause/Exit Game.");
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,90/GUI_SCALE,(180/GUI_SCALE)*9,makecol(0,0,0),-1,"ESC: Pause/Exit Game.");
        }
        //Main menu loop
        if(GAME_STATE==MENU){

            //Draw the main menu and the title text
            stretch_sprite(buffer,title_screen,0,0,SCREEN_W,SCREEN_H);
            //textprintf_ex(buffer,font_48,40,20,makecol(0,0,0),-1,"3 5 7");
            //textprintf_ex(buffer,font_24,40,90,makecol(0,0,0),-1,"A game of logic and intellect.");
            if(GUI_SCALE==9)textprintf_ex(buffer,font_14,90/GUI_SCALE,180/GUI_SCALE,makecol(0,0,0),-1,"A game of logic and intellect.");
            if(GUI_SCALE==8)textprintf_ex(buffer,font_20,90/GUI_SCALE,180/GUI_SCALE,makecol(0,0,0),-1,"A game of logic and intellect.");
            if(GUI_SCALE==7)textprintf_ex(buffer,font_24,90/GUI_SCALE,180/GUI_SCALE,makecol(0,0,0),-1,"A game of logic and intellect.");
            if(GUI_SCALE==6)textprintf_ex(buffer,font_34,90/GUI_SCALE,180/GUI_SCALE,makecol(0,0,0),-1,"A game of logic and intellect.");
            if(GUI_SCALE==5)textprintf_ex(buffer,font_48,90/GUI_SCALE,180/GUI_SCALE,makecol(0,0,0),-1,"A game of logic and intellect.");


            draw_sprite(buffer,play_button,(SCREEN_W/2)-150,SCREEN_H/2);
            stretch_sprite(buffer,settings,SCREEN_W-(800/GUI_SCALE),SCREEN_H-(800/GUI_SCALE),800/GUI_SCALE,800/GUI_SCALE);
            textprintf_ex(buffer,font_48,(SCREEN_W/2)-125,(SCREEN_H/2)+10,makecol(0,0,0),-1,"PLAY");

          if(mouse_b & 1 && collision(mouse_x,mouse_x,SCREEN_W-(800/GUI_SCALE),SCREEN_W, mouse_y,mouse_y,SCREEN_H-(800/GUI_SCALE),SCREEN_H))
             GAME_STATE=OPTIONS;

            //Play button clicking

          if(mouse_b & 1 && collision(mouse_x,mouse_x,(SCREEN_W/2)-150,(SCREEN_W/2)+150, mouse_y,mouse_y,SCREEN_H/2,(SCREEN_H/2)+100)){
                GAME_STATE=SETUP;
           }
        }

        //Game loop, handles the actual game
        if(GAME_STATE==GAME){
                //Moves the turn to the player, if AI has no move (DEBUG ONLY!!)

                //Debug code, keep scrolling
                //if(key[KEY_ENTER])turn=player;

                if(key[KEY_ENTER]){
                    three_pile=3;
                    five_pile=5;
                    seven_pile=7;
                    three_pile_turn=3;
                    five_pile_turn=5;
                    seven_pile_turn=7;
                    game_ending=playing;
                    if(turn_start==player_start){turn=player; turn_init=0;}
                    if(turn_start==ai_start)turn=ai;
                    if(turn_start==random_start){
                        if(random(1,2)==1)turn=player;
                        else turn=ai;
                    }
                }
                while(key[KEY_ENTER]){}

                //Draw background for game
                draw_sprite(buffer,game_background,0,0);

                if(game_ending==player_win)textprintf_ex(buffer,font_48,400,20,makecol(0,0,0),-1,"Player wins!");
                if(game_ending==ai_win)textprintf_ex(buffer,font_48,400,20,makecol(0,0,0),-1,"AI wins!");



                //Draw two semi transparent layers

                //This layer doesn't change until the turn ends
                if(three_pile>0)draw_sprite(buffer,stone_1,0,0);
                if(three_pile>1)draw_sprite(buffer,stone_1,100,0);
                if(three_pile>2)draw_sprite(buffer,stone_1,200,0);

                if(five_pile>0)draw_sprite(buffer,stone_1,0,200);
                if(five_pile>1)draw_sprite(buffer,stone_1,100,200);
                if(five_pile>2)draw_sprite(buffer,stone_1,200,200);
                if(five_pile>3)draw_sprite(buffer,stone_1,300,200);
                if(five_pile>4)draw_sprite(buffer,stone_1,400,200);

                if(seven_pile>0)draw_sprite(buffer,stone_1,0,400);
                if(seven_pile>1)draw_sprite(buffer,stone_1,100,400);
                if(seven_pile>2)draw_sprite(buffer,stone_1,200,400);
                if(seven_pile>3)draw_sprite(buffer,stone_1,300,400);
                if(seven_pile>4)draw_sprite(buffer,stone_1,400,400);
                if(seven_pile>5)draw_sprite(buffer,stone_1,500,400);
                if(seven_pile>6)draw_sprite(buffer,stone_1,600,400);

                //This layer changes when you are still working on a turn
                //It doesn't render if the game has ended
                 if(game_ending==playing){
                    if(three_pile_turn>0)draw_sprite(buffer,stone_2,0,0);
                    if(three_pile_turn>1)draw_sprite(buffer,stone_2,100,0);
                    if(three_pile_turn>2)draw_sprite(buffer,stone_2,200,0);

                    if(five_pile_turn>0)draw_sprite(buffer,stone_2,0,200);
                    if(five_pile_turn>1)draw_sprite(buffer,stone_2,100,200);
                    if(five_pile_turn>2)draw_sprite(buffer,stone_2,200,200);
                    if(five_pile_turn>3)draw_sprite(buffer,stone_2,300,200);
                    if(five_pile_turn>4)draw_sprite(buffer,stone_2,400,200);

                    if(seven_pile_turn>0)draw_sprite(buffer,stone_2,0,400);
                    if(seven_pile_turn>1)draw_sprite(buffer,stone_2,100,400);
                    if(seven_pile_turn>2)draw_sprite(buffer,stone_2,200,400);
                    if(seven_pile_turn>3)draw_sprite(buffer,stone_2,300,400);
                    if(seven_pile_turn>4)draw_sprite(buffer,stone_2,400,400);
                    if(seven_pile_turn>5)draw_sprite(buffer,stone_2,500,400);
                    if(seven_pile_turn>6)draw_sprite(buffer,stone_2,600,400);
                 }

                //Player turn loop, handles the rock clicking
                if(turn==player){

                    if(game_ending==playing)stretch_sprite(buffer,player_turn,SCREEN_W-(600/GUI_SCALE)-20,20,600/GUI_SCALE,600/GUI_SCALE);


                    //Debug mode text, keep scrolling
                    //textprintf_ex(buffer,font_48,400,20,makecol(0,0,0),-1,"%i,%i,%i",three_pile_turn,five_pile_turn,seven_pile_turn);

                     //Initializes the player's turn,
                     if(turn_init==0){
                        seven_pile_turn=seven_pile;
                        five_pile_turn=five_pile;
                        three_pile_turn=three_pile;
                        turn_init++;

                     }
                    //Clicking on the three pile
                    if(mouse_b & 1 && collision(mouse_x,mouse_x,0,300, mouse_y,mouse_y,0,100)){
                        if(pile_selected==none || pile_selected==three_pile_selected)
                            if(three_pile_turn>0)three_pile_turn--;
                    }
                    //Clicking on the five pile
                    if(mouse_b & 1 && collision(mouse_x,mouse_x,0,500, mouse_y,mouse_y,200,300)){
                        if(pile_selected==none || pile_selected==five_pile_selected)

                            if(five_pile_turn>0)five_pile_turn--;
                    }
                    //Clicking on the seven pile
                    if(mouse_b & 1 && collision(mouse_x,mouse_x,0,700, mouse_y,mouse_y,400,500)){
                        if(pile_selected==none || pile_selected==seven_pile_selected)
                            if(seven_pile_turn>0)seven_pile_turn--;
                    }

                    //Wait until the mouse is let up to continue game
                    while(mouse_b & 1){}

                    //Right click to reset piles
                    if(mouse_b & 2){
                        seven_pile_turn=seven_pile;
                        five_pile_turn=five_pile;
                        three_pile_turn=three_pile;
                    }
                    //Ends turn if there is a rock taken
                    if(key[KEY_SPACE]){
                        if(three_pile!=three_pile_turn || five_pile!=five_pile_turn || seven_pile!=seven_pile_turn){
                            three_pile=three_pile_turn;
                            five_pile=five_pile_turn;
                            seven_pile=seven_pile_turn;
                            turn=ai;
                            if(three_pile+five_pile+seven_pile==0 && game_ending==playing){
                                game_ending=ai_win;
                            }
                        }
                    }
                    //Sets the pile_selected variable if there is a rock taken from that pile
                    if(three_pile!=three_pile_turn)pile_selected=three_pile_selected;
                    if(five_pile!=five_pile_turn)pile_selected=five_pile_selected;
                    if(seven_pile!=seven_pile_turn)pile_selected=seven_pile_selected;
                    if(three_pile==three_pile_turn && five_pile==five_pile_turn && seven_pile==seven_pile_turn)pile_selected=none;




                }
                //AI turn loop
                if(turn==ai){

                if(game_ending==playing)stretch_sprite(buffer,ai_turn,SCREEN_W-(600/GUI_SCALE)-20,20,600/GUI_SCALE,600/GUI_SCALE);

                ai_turn_delay_incrementer++;
                //Sets the delay of the AI making its move
                if(ai_turn_delay_incrementer>=ai_turn_delay-50){
                  if(three_pile==3){
                    if(five_pile==5){
                        if(seven_pile==7)if(turn==ai){three_pile=2; end_ai_turn();}
                        if(seven_pile==6)if(turn==ai){three_pile=2; end_ai_turn();}
                        if(seven_pile==5)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==4)if(turn==ai){three_pile=1; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=1; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){five_pile=3; end_ai_turn();}

                    }if(five_pile==4){
                        if(seven_pile>4)if(turn==ai){three_pile=1; end_ai_turn();}
                        if(seven_pile==4)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=1; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){five_pile=3; end_ai_turn();}
                    }if(five_pile==3){
                        if(seven_pile>0)if(turn==ai){seven_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){three_pile=2; end_ai_turn();}
                    }if(five_pile==2){
                        if(seven_pile>1)if(turn==ai){seven_pile=1; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){seven_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){three_pile=2; end_ai_turn();}
                    }if(five_pile==1){
                        if(seven_pile>2)if(turn==ai){seven_pile=2; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){seven_pile=1; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){three_pile=1; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){three_pile=0; end_ai_turn();}
                    }if(five_pile==0){
                        if(seven_pile>3)if(turn==ai){seven_pile=3; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){seven_pile=2; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){three_pile=2; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){three_pile=1; end_ai_turn();}
                    }

                  }if(three_pile==2){
                    if(five_pile==5){
                        if(seven_pile==0)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=3; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){five_pile=1; end_ai_turn();}
                        if(seven_pile==4)if(turn==ai){three_pile=1; end_ai_turn();}
                        if(seven_pile==5)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==6)if(turn==ai){five_pile=4; end_ai_turn();}
                        if(seven_pile==7)if(turn==ai){seven_pile=6; end_ai_turn();}
                    }if(five_pile==4){
                        if(seven_pile==0)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=3; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){five_pile=1; end_ai_turn();}
                        if(seven_pile==4)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==5)if(turn==ai){three_pile=1; end_ai_turn();}
                        if(seven_pile==6)if(turn==ai){seven_pile=5; end_ai_turn();}
                        if(seven_pile==7)if(turn==ai){seven_pile=6; end_ai_turn();}

                    }if(five_pile==3){
                        if(seven_pile==0)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=1; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==4)if(turn==ai){seven_pile=2; end_ai_turn();}
                        if(seven_pile==5)if(turn==ai){three_pile=1; end_ai_turn();}
                        if(seven_pile==6)if(turn==ai){seven_pile=5; end_ai_turn();}
                        if(seven_pile==7)if(turn==ai){seven_pile=6; end_ai_turn();}
                    }if(five_pile==2){
                        if(seven_pile>0)if(turn==ai){seven_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){three_pile=1; end_ai_turn();}

                    }if(five_pile==2){
                        if(seven_pile>0)if(turn==ai){seven_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){three_pile=1; end_ai_turn();}
                    }if(five_pile==1){
                        if(seven_pile>3)if(turn==ai){seven_pile=3; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){seven_pile=2; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){three_pile=0; end_ai_turn();}
                    }if(five_pile==0){
                        if(seven_pile>2)if(turn==ai){seven_pile=2; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){seven_pile=1; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){three_pile=1; end_ai_turn();}
                    }
                  }if(three_pile==1){

                    if(five_pile==5){
                        if(seven_pile==6)if(turn==ai){seven_pile=4; end_ai_turn();}
                        if(seven_pile==7)if(turn==ai){seven_pile=4; end_ai_turn();}
                        if(seven_pile==4)if(turn==ai){five_pile=4; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==5)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=3; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=1; end_ai_turn();}

                    }if(five_pile==4){
                        if(seven_pile==7)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==6)if(turn==ai){seven_pile=5; end_ai_turn();}
                        if(seven_pile==5)if(turn==ai){seven_pile=4; end_ai_turn();}
                        if(seven_pile==4)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=3; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=1; end_ai_turn();}

                    }if(five_pile==3){
                        if(seven_pile>2)if(turn==ai){seven_pile=2; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){seven_pile=1; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=1; end_ai_turn();}

                    }if(five_pile==2){
                        if(seven_pile>3)if(turn==ai){seven_pile=3; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){five_pile=1; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=1; end_ai_turn();}
                    }if(five_pile==1){
                        if(seven_pile>1)if(turn==ai){seven_pile=1; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){seven_pile=0; end_ai_turn();}
                    }if(five_pile==0){
                        if(seven_pile>0)if(turn==ai){seven_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){three_pile=0; end_ai_turn();}

                    }if(five_pile==0){
                        if(three_pile==0 && seven_pile>1)if(turn==ai){seven_pile=1; end_ai_turn();}
                        if(three_pile==1 && seven_pile==0)if(turn==ai){three_pile=0; end_ai_turn();}
                        if(three_pile==1 && seven_pile>0)if(turn==ai){seven_pile=0; end_ai_turn();}
                        if(three_pile>1 && seven_pile==1)if(turn==ai){three_pile=0; end_ai_turn();}

                    }
                }if(three_pile==0){
                    if(five_pile==5){
                        if(seven_pile>5)if(turn==ai){seven_pile=5; end_ai_turn();}
                        if(seven_pile==5)if(turn==ai){seven_pile=4; end_ai_turn();}
                        if(seven_pile==4)if(turn==ai){five_pile=4; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){five_pile=3; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){five_pile=1; end_ai_turn();}
                    }if(five_pile==4){
                        if(seven_pile>4)if(turn==ai){seven_pile=4; end_ai_turn();}
                        if(seven_pile==4)if(turn==ai){five_pile=3; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){five_pile=3; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){five_pile=1; end_ai_turn();}

                    }if(five_pile==3){
                        if(seven_pile>3)if(turn==ai){seven_pile=3; end_ai_turn();}
                        if(seven_pile==3)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=2; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){five_pile=1; end_ai_turn();}



                    }if(five_pile==2){
                        if(seven_pile>2)if(turn==ai){seven_pile=2; end_ai_turn();}
                        if(seven_pile==2)if(turn==ai){five_pile=1; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){five_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){five_pile=1; end_ai_turn();}

                    }if(five_pile==1){
                        if(seven_pile>0)if(turn==ai){seven_pile=0; end_ai_turn();}
                        if(seven_pile==0)if(turn==ai){five_pile=0; end_ai_turn();}
                    }if(five_pile==0){
                        if(seven_pile>1)if(turn==ai){seven_pile=1; end_ai_turn();}
                        if(seven_pile==1)if(turn==ai){seven_pile=0; end_ai_turn();}
                    }


                }
              }


            }
        }
        if(key[KEY_F9])set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
        if(key[KEY_F8])set_gfx_mode(GFX_AUTODETECT,800 , 600, 0, 0);
        if(key[KEY_F7])set_gfx_mode(GFX_AUTODETECT, 1024, 768, 0, 0);
        if(key[KEY_F6])set_gfx_mode(GFX_AUTODETECT,1280 ,1024 , 0, 0);
        if(key[KEY_F5])set_gfx_mode(GFX_AUTODETECT,1366 ,768 , 0, 0);
        if(key[KEY_F4])set_gfx_mode(GFX_AUTODETECT,1440 ,900 , 0, 0);
        if(key[KEY_F])set_gfx_mode(GFX_AUTODETECT,1600 ,900 , 0, 0);
        if(key[KEY_F])set_gfx_mode(GFX_AUTODETECT,1920 ,1080 , 0, 0);
        if(key[KEY_F])set_gfx_mode(GFX_AUTODETECT,3840 ,2160 , 0, 0);

        //State independent reset key
        if(key[KEY_ESC]){
           int alert_value;
           alert_value=alert3(NULL, "Paused",NULL,"&Main Menu", "&Resume","&Exit", 'm','r','e' );
           if(alert_value==1){GAME_STATE=MENU;
                    three_pile=3;
                    five_pile=5;
                    seven_pile=7;
                    three_pile_turn=3;
                    five_pile_turn=5;
                    seven_pile_turn=7;
                    game_ending=playing;
           }
           if(alert_value==3)GAME_STATE=EXIT;

      }

        click_incrementer++;
        //Draw cursor and draw buffer to the screen
        draw_sprite(buffer,cursor,mouse_x,mouse_y);
        draw_sprite(screen,buffer,0,0);

        //Sets the speed of the entire game, DO NOT TOUCH, YOU CAN DIE!
        rest(1);

}












void setup(){

    //The game starts in the main menu
    GAME_STATE=MENU;

    //Load in the bitmaps, and abort on error if they don't exist
  if(!(title_screen = load_bitmap("images/title_screen.png",NULL))){
    abort_on_error( "Cannot find images/title_screen.png.\n Please check your files and try again.");
  }

  if(!(play_button = load_bitmap("images/play_button.png",NULL))){
    abort_on_error( "Cannot find images/play_button.png.\n Please check your files and try again.");
  }
  if(!(cursor = load_bitmap("images/cursor.png",NULL))){
    abort_on_error( "Cannot find images/cursor.png.\n Please check your files and try again.");
  }

  if(!(game_background = load_bitmap("images/game_background.png",NULL))){
    abort_on_error( "Cannot find images/game_background.png.\n Please check your files and try again.");
  }
  if(!(stone_1 = load_bitmap("images/stone_1.bmp",NULL))){
    abort_on_error( "Cannot find images/stone_1.png.\n Please check your files and try again.");
  }
  if(!(stone_2 = load_bitmap("images/stone_2.bmp",NULL))){
    abort_on_error( "Cannot find images/stone_2.png.\n Please check your files and try again.");
  }
  if(!(settings = load_bitmap("images/settings.png",NULL))){
    abort_on_error( "Cannot find images/settings.png.\n Please check your files and try again.");
  }
  if(!(player_turn = load_bitmap("images/player_turn.png",NULL))){
    abort_on_error( "Cannot find images/player_turn.png.\n Please check your files and try again.");
  }
  if(!(ai_turn = load_bitmap("images/ai_turn.png",NULL))){
    abort_on_error( "Cannot find images/ai_turn.png.\n Please check your files and try again.");
  }
  if(!(slider = load_bitmap("images/slider.png",NULL))){
    abort_on_error( "Cannot find images/slider.png.\n Please check your files and try again.");
  }
  if(!(knob = load_bitmap("images/knob.png",NULL))){
    abort_on_error( "Cannot find images/knob.png.\n Please check your files and try again.");
  }
  if(!(box = load_bitmap("images/box.png",NULL))){
    abort_on_error( "Cannot find images/box.png.\n Please check your files and try again.");
  }
  if(!(box_selected = load_bitmap("images/box_selected.png",NULL))){
    abort_on_error( "Cannot find images/box_selected.png.\n Please check your files and try again.");
  }
  if(!(start_button = load_bitmap("images/start_button.png",NULL))){
    abort_on_error( "Cannot find images/start_button.png.\n Please check your files and try again.");
  }
  if(!(slider_gui_scale = load_bitmap("images/slider_gui_scale.png",NULL))){
    abort_on_error( "Cannot find images/slider_gui_scale.png.\n Please check your files and try again.");
  }



  //Load .pcx and convert them into a font
  FONT *f1, *f2, *f3, *f4, *f5;

  if(!(f1 = load_font("fonts/font_48.pcx", NULL, NULL))){
    abort_on_error( "Cannot find fonts/font_48.pcx \n Please check your files and try again");
  }
  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge temporary fonts to create "pixelart"
  font_48 = merge_fonts(f4, f5 = merge_fonts(f2, f3));

  if(!(f1 = load_font("fonts/font_24.pcx", NULL, NULL))){
    abort_on_error( "Cannot find fonts/font_24.pcx \n Please check your files and try again");
  }
  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge temporary fonts to create "pixelart"
  font_24 = merge_fonts(f4, f5 = merge_fonts(f2, f3));

   if(!(f1 = load_font("fonts/font_10.pcx", NULL, NULL))){
    abort_on_error( "Cannot find fonts/font_10.pcx \n Please check your files and try again");
  }
  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge temporary fonts to create "pixelart"
  font_10 = merge_fonts(f4, f5 = merge_fonts(f2, f3));

  if(!(f1 = load_font("fonts/font_14.pcx", NULL, NULL))){
    abort_on_error( "Cannot find fonts/font_14.pcx \n Please check your files and try again");
  }
  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge temporary fonts to create "pixelart"
  font_14 = merge_fonts(f4, f5 = merge_fonts(f2, f3));

    if(!(f1 = load_font("fonts/font_34.pcx", NULL, NULL))){
    abort_on_error( "Cannot find fonts/font_34.pcx \n Please check your files and try again");
  }
  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge temporary fonts to create "pixelart"
  font_34 = merge_fonts(f4, f5 = merge_fonts(f2, f3));



  if(!(f1 = load_font("fonts/font_20.pcx", NULL, NULL))){
    abort_on_error( "Cannot find fonts/font_20.pcx \n Please check your files and try again");
  }
  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge temporary fonts to create "pixelart"
  font_20 = merge_fonts(f4, f5 = merge_fonts(f2, f3));


  destroy_font(f1);
  destroy_font(f2);
  destroy_font(f3);
  destroy_font(f4);
  destroy_font(f5);


}



int main(){

  srand(time(NULL));
  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT, 1280, 1024, 0, 0);
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");
  if(SCREEN_W<3840)GUI_SCALE=1;
  if(SCREEN_W<1921)GUI_SCALE=2;
  if(SCREEN_W<1601)GUI_SCALE=3;
  if(SCREEN_W<1441)GUI_SCALE=4;
  if(SCREEN_W<1367)GUI_SCALE=5;
  if(SCREEN_W<1281)GUI_SCALE=6;
  if(SCREEN_W<1025)GUI_SCALE=7;
  if(SCREEN_W<801)GUI_SCALE=8;
  if(SCREEN_W<641)GUI_SCALE=9;

  buffer=create_bitmap(1280,1024);
  set_window_title("3 5 7");
  setup();

  	while(!GAME_STATE==EXIT){
        update();
  	}

	return 0;
}
END_OF_MAIN()

