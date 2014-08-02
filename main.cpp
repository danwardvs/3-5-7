#include<allegro.h>
#include<alpng.h>

#define EXIT 0
#define MENU 1
#define GAME 2

#define player 1
#define ai 2

#define none 0
#define three_pile_selected 1
#define five_pile_selected 2
#define seven_pile_selected 3

//Handles the state of the game, menu, game, exiting
int GAME_STATE;


//Declare bitmaps
BITMAP* title_screen;
BITMAP* play_button;
BITMAP* cursor;
BITMAP* game_background;
BITMAP* stone_1;
BITMAP* stone_2;

BITMAP* buffer;

//Declare fonts
FONT* font_48;
FONT* font_24;

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

//Function called when the AI makes a move
void end_ai_turn(){
     turn=player;
     turn_init=0;

}

//Update loop, is ran every frame
void update(){
        //Main menu loop
        if(GAME_STATE==MENU){

            //Draw the main menu and the title text
            stretch_sprite(buffer,title_screen,0,0,SCREEN_W,SCREEN_H);
            textprintf_ex(buffer,font_48,40,20,makecol(0,0,0),-1,"3 5 7");
            textprintf_ex(buffer,font_24,40,90,makecol(0,0,0),-1,"A game of logic and intellect.");
            draw_sprite(buffer,play_button,(SCREEN_W/2)-150,SCREEN_H/2);

            //Play button clicking
            if(mouse_b & 1 && collision(mouse_x,mouse_x,(SCREEN_W/2)-150,(SCREEN_W/2)+150, mouse_y,mouse_y,SCREEN_H/2,(SCREEN_H/2)+100)){
                GAME_STATE=GAME;
                turn_init=0;
                turn=player;
            }
        }

        //Game loop, handles the actual game
        if(GAME_STATE==GAME){
                //Moves the turn to the player, if AI has no move (DEBUG ONLY!!)
                if(key[KEY_ENTER])turn=player;

                //Draw background for game
                draw_sprite(buffer,game_background,0,0);

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


                //Player turn loop, handles the rock clicking
                if(turn==player){

                    //Debug mode text,not needed anymore
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
                    if(three_pile==3){
                      if(five_pile==3){
                        if(seven_pile>0)if(turn==ai){seven_pile=0; end_ai_turn();}
                      }

                     if(five_pile==2){
                        if(seven_pile>1)if(turn==ai){seven_pile=1; end_ai_turn();}
                    }if(five_pile==1){
                        if(seven_pile>2)if(turn==ai){seven_pile=2; end_ai_turn();}
                    }if(five_pile==0){
                        if(seven_pile>3)if(turn==ai){seven_pile=3; end_ai_turn();}
                    }

                  }if(three_pile==2){
                    if(seven_pile==0){
                        if(five_pile==3){if(turn==ai){five_pile=2; end_ai_turn();}}
                        if(five_pile==2){if(turn==ai){five_pile=1; end_ai_turn();}}
                        if(five_pile==1){if(turn==ai){three_pile=0; end_ai_turn();}}
                        if(five_pile==0){if(turn==ai){three_pile=1; end_ai_turn();}}
                    }
                  }if(three_pile==1){
                    if(seven_pile==0){
                        if(five_pile>0)if(turn==ai){five_pile=0; end_ai_turn();}

                    }
                  }if(five_pile==0){
                    if(three_pile==0 && seven_pile>1)if(turn==ai){seven_pile=1; end_ai_turn();}
                    if(three_pile==1 && seven_pile==0)if(turn==ai){three_pile=0; end_ai_turn();}
                    if(three_pile==1 && seven_pile>0)if(turn==ai){seven_pile=0; end_ai_turn();}
                    if(three_pile>1 && seven_pile==1)if(turn==ai){three_pile=0; end_ai_turn();}

                  }
                }


        }

        //Draw cursor and draw buffer to the screen
        draw_sprite(buffer,cursor,mouse_x,mouse_y);
        draw_sprite(screen,buffer,0,0);


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


  destroy_font(f1);
  destroy_font(f2);
  destroy_font(f3);
  destroy_font(f4);
  destroy_font(f5);


}



int main(){


  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT, 1280, 1024, 0, 0);
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");

  buffer=create_bitmap(1280,1024);
  set_window_title("3 5 7");
  setup();

  	while(!key[KEY_ESC] && !GAME_STATE==EXIT){
        update();
  	}

	return 0;
}
END_OF_MAIN()

