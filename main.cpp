#include<allegro.h>



BITMAP* buffer;

int main(){


  allegro_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT, 1280, 1024, 0, 0);
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");

  buffer=create_bitmap(1280,1024);
  set_window_title("3 5 7");
  	while(!key[KEY_ESC]){

        rectfill(buffer,0,0,1280,1024,makecol(255,255,255));

        draw_sprite(screen,buffer,0,0);





	}

	return 0;
}
END_OF_MAIN()

