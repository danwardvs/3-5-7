#include<allegro.h>
#include<alpng.h>


BITMAP* title_screen;
BITMAP* buffer;

FONT* font_48;

void abort_on_error(const char *message){
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}


void setup(){

      if(!(title_screen = load_bitmap("images/title_screen.png",NULL))){
    abort_on_error( "Cannot find images/title_screen.png.\n Please check your files and try again.");
  }

  FONT *f1, *f2, *f3, *f4, *f5;

  if(!(f1 = load_font("fonts/font_48.pcx", NULL, NULL))){
    abort_on_error( "Cannot find fonts/font_48.pcx \n Please check your files and try again");
  }
  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge temporary fonts to create "pixelart"
  font_48 = merge_fonts(f4, f5 = merge_fonts(f2, f3));

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

  	while(!key[KEY_ESC]){

        //rectfill(buffer,0,0,1280,1024,makecol(255,255,255));


        stretch_sprite(buffer,title_screen,0,0,SCREEN_W,SCREEN_H);
        textprintf_ex(buffer,font_48,20,20,makecol(0,0,0),-1,"3 5 7");



        draw_sprite(screen,buffer,0,0);





	}

	return 0;
}
END_OF_MAIN()

