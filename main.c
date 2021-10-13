#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#pragma intrinsic( cos, sin )
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

#include "leepra.h"
#include "tyfuus.h"
#include "effects.h"
#include "file.h"
#include "kancr.h"
#include "timer.h"

#define WIDTH 512
#define HEIGHT 384


#define SNOW 0x00FFFFFF
#define BACKGROUND 0xc0c0c0c0

void ye_old_snowroutine( tyfuus_bitmap *bitmap ){
	int count = bitmap->width*(bitmap->height-1);
	unsigned int *screenp = bitmap->data;
	int w = bitmap->width;
	int h = bitmap->height;
	int x;

	while(count--){
		if(screenp[count]==SNOW){
			//gjeldene pixel er snø.
			int friction = (rand()%3)-1;
			if(screenp[count+w+friction]==BACKGROUND){
				screenp[count+w+friction] = SNOW;
				screenp[count] = BACKGROUND;
			}
		}
	}
	//spawn snow
	x = rand()%w;
	bitmap->data[x] = SNOW;

}

void blit( tyfuus_bitmap *target, tyfuus_bitmap *source, int scroll ){
	int y;
	int w = source->width*4;
	unsigned int *dst = target->data+256;
	unsigned int *src = source->data+(scroll*source->width);

	for(y=0;y<target->height;y++){
		memcpy( dst, src, w );
		dst += target->width;
		src += source->width;
	}
}

void Error(char* string){
	leepra_close();
	MessageBox( NULL, string, NULL, MB_OK );
	exit(1);
}

object *load_object();

#include "minifmod.h"
unsigned int fileopen(char *name){
	return (unsigned int)fopen("data/muzak.xm", "rb");
}

void fileclose(unsigned int handle){
	fclose((FILE *)handle);
}

int fileread(void *buffer, int size, unsigned int handle){
	return fread(buffer, 1, size, (FILE *)handle);
}

void fileseek(unsigned int handle, int pos, signed char mode){
	fseek((FILE *)handle, pos, mode);
}

int filetell(unsigned int handle){
	return ftell((FILE *)handle);
}


char *scroller[]={
	"Hello there!",
	"",
	"This is Kusma of Excess on the keys backed up",
	"by Irvin of Flimmer and we would like to say",
	"\"MERRY CHRISTMAS\" to you all!",
	"",
	"",
	"",
	"Oh, and by the way...",
	"you might have noticed the little logo on the side.",
	"You see, this is not only a christmas greeting",
	"but also an advertisement for the worlds best",
	"Norwegian scene-portal.",
	"",
	"Yes, we are talking about www.demoscene.no",
	"",
	"",
	"This site has now been re-opened, as a",
	"christmaspresent from me(Kusma) to all the ",
	"norwegian-speaking sceners (and",
	"wannabe-sceners) in the world.",
	"",
	"",
	"Irvin would also like to point out that he is",
	"really ashamed of the horrible music, but due",
	"to threats of rape from kusma he decided to",
	"make something afterall.",
	"",
	"Ofcourse, this is a pure lie. He has been working",
	"on the tune for about a year, and he nagged me",
	"all the fucking time, so I decided to finally give",
	"him a chance.",
	"",
	"Ohwell, everybody can't be as sexy, smart and",
	"talented as me.",
	"",
	"",
	"Okay, lets get to the interresting stuff",
	"",
	"The 3d-object contains over one billion",
	"polygons, and is lit with 1024 dynamicly moving",
	"lightsources using tamphongshading.",
	"",
	"The snowroutine is rendered using the patented",
	"Snow-o'tronic 5000 technique. This involves",
	"raytracing reflections and calculating natural",
	"motions for each induvidual snowflake.",
	"Unfortuanly the snow is white, so the",
	"reflections become invisible",
	"",
	"All this, including music, fits in less than",
	"one byte.",
	"What can I say... We rule.",
	"",
	"",
	"",
	"",
	"We would like to greet: (in order of penis-size)",
	"",
	"Shish Cracking Crew (I managed to remember",
	"you guys this time!)",
	"Haujobb",
	"Aardbei (mostly ile, he is sexy)",
	"Spacebalds (Mmmm... bald)",
	"Unique",
	"Kvasigen",
	"Anakata",
	"Ephidrena",
	"Damage",
	"The Silents",
	"Index",
	"Aurum",
	"Outracks",
	"Phrenetics",
	"Therapii",
	"Nocturnal",
	"Purple",
	"DXM (stefans penis isn't THAT small)",
	"INF",
	"Portal Project",
	"The Lost Souls",
	"Yaphan (those womans-throusers are sexy)",
	"Deft Overflow",
	"Moondreamers",
	"Bypass",
	"Fairlight",
	"Razor 1911",
	"Squirrelz",
	"",
	"",
	"Didn't see your name up here?",
	"",
	"Feeling left out?",
	"",
	"well, then you are proabebly a nolife looser",
	"and you smell bad."
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Sooo, why are you still hanging around?",
	"",
	"",
	"Shouldn't you be at www.demoscene.no now,",
	"getting your fix?",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"(WRAPPING)",
	NULL
};

#define SCROLLHEIGHT 6

int main(){
	int i;
	MSG msg;
	float starttime;
	matrix temp_matrix;
	file *musikk;
	FMUSIC_MODULE *mod;

	tyfuus_bitmap *texture, *bg;
	tyfuus_bitmap *screen = tyfuus_make_bitmap( WIDTH, HEIGHT );
	tyfuus_bitmap *motionblur = tyfuus_make_bitmap( WIDTH, HEIGHT );
	tyfuus_bitmap *blurbuffer = tyfuus_make_bitmap( WIDTH, HEIGHT );
	tyfuus_bitmap *scrolltext = tyfuus_make_bitmap( WIDTH/2, HEIGHT*SCROLLHEIGHT );

	object *sphere;
	camera cam;

	memset( scrolltext->data, 0xc0, 256*HEIGHT*SCROLLHEIGHT*4 );
	for(i=0;i<HEIGHT*SCROLLHEIGHT;i++){
		scrolltext->data[i*scrolltext->width] = 0;
	}

	tyfuus_init_font( WIDTH/2, HEIGHT*SCROLLHEIGHT, "verdana", 12 );
	i=0;
	while(scroller[i]){
		tyfuus_print( scrolltext, 10, 384+i*12, scroller[i], 0 );
		i++;
	}


	if(!leepra_open( "demoscene.no ruler", WIDTH, HEIGHT, TRUE)) Error( "failed to initialize directdraw" );

	FSOUND_File_SetCallbacks(fileopen, fileclose, fileread, fileseek, filetell);
	if (!FSOUND_Init(44100, 0)) Error("Error upon initialization\n");

	mod = FMUSIC_LoadSong(NULL, NULL);
	if(!mod) Error("Error loading song\n");

	if( !(texture=tyfuus_load_bitmap("texture.jpg")) ) Error( "failed to load image" );
	if( !(bg=tyfuus_load_bitmap("bg.gif")) ) Error( "failed to load image" );

	if( !(sphere = load_object()) ) Error( "failed to load object" );

	sphere->materials[0].envmap = texture->data;
	sphere->materials[0].texture = texture->data;
	sphere->materials[0].rendermode = RENDER_ENVMAP;

	init_kancr();
	init_timer();

	memcpy( screen->data, bg->data, WIDTH*HEIGHT*4 );

	FMUSIC_PlaySong(mod);
	starttime = get_time();
	do{
		float time = (get_time()-starttime)*0.5f;

		ye_old_snowroutine( scrolltext );
		memcpy( screen->data, bg->data, WIDTH*HEIGHT*4 );
		blit( screen, scrolltext, (int)(time*20)%((SCROLLHEIGHT-1)*HEIGHT) );

		start_frame();
		set_fov( 110 );

		set_camera( &cam, vector_make(0,-3,-15), vector_make(-8,0,0), 0 );

		matrix_rotate( sphere->matrix, vector_make(0,time,0) );
		matrix_rotate( temp_matrix, vector_make(M_PI+time,0,M_PI/2-time) );
		matrix_multiply( sphere->matrix, temp_matrix );
		matrix_scale( temp_matrix, vector_make(18,18,18));
		matrix_multiply( sphere->matrix, temp_matrix );

		draw_object( screen->data, sphere, &cam );
//		tyfuus_multiply( screen, bg2 );

		leepra_update( screen->data );
		while (PeekMessage(&msg,NULL,0,0,PM_REMOVE)){ 
		    TranslateMessage(&msg);
		    DispatchMessage(&msg); 
		    if (msg.message == WM_QUIT)
				break;
		}
	}while((msg.message!=WM_QUIT) && !GetAsyncKeyState(VK_ESCAPE));

	FMUSIC_FreeSong(mod);
	FSOUND_Close();

	leepra_close();

	free_object( sphere );
	tyfuus_free_bitmap(texture);

	tyfuus_free_bitmap(screen);
	tyfuus_free_bitmap(motionblur);
	return 0;
}
