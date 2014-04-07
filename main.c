#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "resource.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#pragma intrinsic( cos, sin )
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

#include "bassmod/bassmod.h"
#include "leepra.h"
#include "tyfuus.h"
#include "effects.h"
#include "file.h"
#include "kancr.h"

LRESULT CALLBACK dialog_callback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL windowed=FALSE;
BOOL nosound=FALSE;

#define WIDTH 320
#define HEIGHT 240

unsigned int orange = 0xef6f00;
unsigned int blue = 0x3f3f7f;

int instr[16];
float last_instr[16];
int instr_skip[16];

float last_fx[256];
int fx_skip[256];
int last_fx_value;

_inline float log_sync( float start, float end, float time ){
	float loog = (float)log((time-start)/(end-start));
	if( (loog>1.f)||_isnan(loog) ) loog = 1.f;
	if( loog<0.f ) loog = 0.f;
	return loog;	
}

void CALLBACK instr_sync(HSYNC handle, DWORD data, DWORD user) {
	int timei = BASSMOD_MusicGetPosition();
	int order = LOWORD( timei );
	float row = (float)HIWORD(timei)/256;
	float time = (order*0x80)+ row;

	last_instr[user] = time;
	instr_skip[user]++;
}

int textsync=0;

void CALLBACK fx_sync(HSYNC handle, DWORD data, DWORD user) {
	int timei = BASSMOD_MusicGetPosition();
	int order = LOWORD( timei );
	float row = (float)HIWORD(timei)/256;
	float time = (order*0x80)+ row;

	if( ((order==26)||(order==27))&& data<127 ){
		textsync++;
		return;
	}

	last_fx_value = data;
	last_fx[data] = time;
	fx_skip[data]++;
}


void Error(char* string){
	leepra_close();
	MessageBox( NULL, string, NULL, MB_OK );
	exit(1);
}

object *load_kjeller();
object *load_batteri();

int main(){
	matrix temp_matrix;

	tyfuus_bitmap *loader, *loader256;
	tyfuus_bitmap *texture, *overlay, *overlay2, *dizzydog, *noname, *udefinerbar, *udefinerbar2, *udefinerbar_env, *deform, *flower_texture, *spaceflowers, *trash;
	tyfuus_bitmap *kjeller_gulv, *kjeller_roer;

	tyfuus_bitmap *greets[36];
	tyfuus_bitmap *batteri_shadowbox, *batteri_shadow, *batteri_vegger, *batteri_veggerpr, *batteri_roer;

	tyfuus_bitmap *invertcube_texture, *spike_envmap;

	tyfuus_bitmap *walterkluge[4];
	tyfuus_bitmap *gastanks[5];
	tyfuus_bitmap *gas[5];
	tyfuus_bitmap *standup[4];

	tyfuus_bitmap *screen = tyfuus_make_bitmap( WIDTH, HEIGHT );
	tyfuus_bitmap *motionblur = tyfuus_make_bitmap( WIDTH, HEIGHT );
	tyfuus_bitmap *blendbuffer = tyfuus_make_bitmap( WIDTH, HEIGHT );
	tyfuus_grid *grid;
	tyfuus_offsetmap *offset;
	object *spike, *kjeller, *batteri;
	object *invertcube, *flower, *sphere, *sphere_deformed;
	camera cam;
	file *musikk;

	HRESULT result = DialogBox( GetModuleHandle(NULL), (LPCTSTR)IDD_DIALOG, NULL, (DLGPROC)dialog_callback );
	if(FAILED(result)) MessageBox( NULL, "could not initialize dialogbox, using default settings", NULL, MB_OK );

	if(!nosound){
		if( BASSMOD_Init( -1, 44100, BASS_DEVICE_LEAVEVOL ) !=TRUE ){
			MessageBox(NULL, "Could not init audio, going nosound", "Error", MB_OK);
			BASSMOD_Init( -2, 44100, 0 );
		}
	}else{
			BASSMOD_Init( -2, 44100, 0 );
	}


	if( !(loader=tyfuus_load_bitmap("loader.jpg")) ) Error( "failed to load image" );
	if(!leepra_open( "walter kluge drepte barten til merethe", WIDTH, HEIGHT, !windowed )) Error( "failed to initialize directdraw" );
	leepra_update( loader->data );

	if(!(musikk = file_open("roar.xm"))) Error("quit fucking with our datafiles!");
	if(!BASSMOD_MusicLoad( TRUE, musikk->data,0, 0, 0)) Error("Could not load audiofile");
	file_close( musikk );


	/* BILDER */
	if( !(texture=tyfuus_load_bitmap("texture.jpg")) ) Error( "failed to load image" );
	if( !(overlay = tyfuus_load_bitmap("overlay.gif")) ) Error( "failed to load image" );
	if( !(overlay2 = tyfuus_load_bitmap("overlay2.gif")) ) Error( "failed to load image" );
	if( !(loader256=tyfuus_load_bitmap("loader256.jpg")) ) Error( "failed to load image" );
	if( !(dizzydog=tyfuus_load_bitmap_alpha("dizzydog.jpg", "dizzydog-alpha.gif")) ) Error( "failed to load image" );;
	if( !(noname=tyfuus_load_bitmap("noname.jpg")) ) Error( "failed to load image" );

	if( !(kjeller_gulv=tyfuus_load_bitmap("kjeller_gulv.jpg")) ) Error( "failed to load image" );
	if( !(kjeller_roer=tyfuus_load_bitmap("kjeller_roer.jpg")) ) Error( "failed to load image" );

	if( !(udefinerbar=tyfuus_load_bitmap("udefinerbar.jpg")) ) Error( "failed to load image" );
	if( !(udefinerbar2=tyfuus_load_bitmap("udefinerbar2.jpg")) ) Error( "failed to load image" );
	if( !(udefinerbar_env=tyfuus_load_bitmap("udefinerbar_env.jpg")) ) Error( "failed to load image" );
	if( !(flower_texture=tyfuus_load_bitmap("flower_texture.jpg")) ) Error( "failed to load image" );
	if( !(deform=tyfuus_load_bitmap("deform.jpg")) ) Error( "failed to load image" );

	if( !(batteri_shadowbox=tyfuus_load_bitmap("batteri_shadowbox.jpg")) ) Error( "failed to load image" );
	if( !(batteri_roer=tyfuus_load_bitmap("batteri_roer.jpg")) ) Error( "failed to load image" );
	if( !(batteri_shadow=tyfuus_load_bitmap("batteri_shadow.jpg")) ) Error( "failed to load image" );
	if( !(batteri_vegger=tyfuus_load_bitmap("batteri_vegger.jpg")) ) Error( "failed to load image" );
	if( !(batteri_veggerpr=tyfuus_load_bitmap("batteri_veggerpr.jpg")) ) Error( "failed to load image" );

	if( !(spaceflowers=tyfuus_load_bitmap("spaceflowers.jpg")) ) Error( "failed to load image" );
	if( !(trash=tyfuus_load_bitmap("trash.jpg")) ) Error( "failed to load image" );

	if( !(invertcube_texture=tyfuus_load_bitmap("invertcube_texture.jpg")) ) Error( "failed to load image" );
	if( !(spike_envmap=tyfuus_load_bitmap("spike_envmap.jpg")) ) Error( "failed to load image" );

	if( !(walterkluge[0]=tyfuus_load_bitmap("walterkluge1.gif")) ) Error( "failed to load image" );
	if( !(walterkluge[1]=tyfuus_load_bitmap("walterkluge2.gif")) ) Error( "failed to load image" );
	if( !(walterkluge[2]=tyfuus_load_bitmap("walterkluge3.gif")) ) Error( "failed to load image" );
	if( !(walterkluge[3]=tyfuus_load_bitmap("walterkluge4.gif")) ) Error( "failed to load image" );

	if( !(gastanks[0]=tyfuus_load_bitmap("gastanks1.gif")) ) Error( "failed to load image" );
	if( !(gastanks[1]=tyfuus_load_bitmap("gastanks2.gif")) ) Error( "failed to load image" );
	if( !(gastanks[2]=tyfuus_load_bitmap("gastanks3.gif")) ) Error( "failed to load image" );
	if( !(gastanks[3]=tyfuus_load_bitmap("gastanks4.gif")) ) Error( "failed to load image" );
	if( !(gastanks[4]=tyfuus_load_bitmap("gastanks5.gif")) ) Error( "failed to load image" );

	if( !(gas[0]=tyfuus_load_bitmap("gas1.gif")) ) Error( "failed to load image" );
	if( !(gas[1]=tyfuus_load_bitmap("gas2.gif")) ) Error( "failed to load image" );
	if( !(gas[2]=tyfuus_load_bitmap("gas3.gif")) ) Error( "failed to load image" );
	if( !(gas[3]=tyfuus_load_bitmap("gas4.gif")) ) Error( "failed to load image" );
	if( !(gas[4]=tyfuus_load_bitmap("gas5.gif")) ) Error( "failed to load image" );

	if( !(standup[0]=tyfuus_load_bitmap("standup1.gif")) ) Error( "failed to load image" );
	if( !(standup[1]=tyfuus_load_bitmap("standup2.gif")) ) Error( "failed to load image" );
	if( !(standup[2]=tyfuus_load_bitmap("standup3.gif")) ) Error( "failed to load image" );
	if( !(standup[3]=tyfuus_load_bitmap("standup4.gif")) ) Error( "failed to load image" );

	if( !(greets[0]=tyfuus_load_bitmap("greets01.gif")) ) Error( "failed to load image" );
	if( !(greets[1]=tyfuus_load_bitmap("greets02.gif")) ) Error( "failed to load image" );
	if( !(greets[2]=tyfuus_load_bitmap("greets03.gif")) ) Error( "failed to load image" );
	if( !(greets[3]=tyfuus_load_bitmap("greets04.gif")) ) Error( "failed to load image" );
	if( !(greets[4]=tyfuus_load_bitmap("greets05.gif")) ) Error( "failed to load image" );
	if( !(greets[5]=tyfuus_load_bitmap("greets06.gif")) ) Error( "failed to load image" );
	if( !(greets[6]=tyfuus_load_bitmap("greets07.gif")) ) Error( "failed to load image" );
	if( !(greets[7]=tyfuus_load_bitmap("greets08.gif")) ) Error( "failed to load image" );
	if( !(greets[8]=tyfuus_load_bitmap("greets09.gif")) ) Error( "failed to load image" );
	if( !(greets[9]=tyfuus_load_bitmap("greets11.gif")) ) Error( "failed to load image" );
	if( !(greets[10]=tyfuus_load_bitmap("greets12.gif")) ) Error( "failed to load image" );
	if( !(greets[11]=tyfuus_load_bitmap("greets13.gif")) ) Error( "failed to load image" );
	if( !(greets[12]=tyfuus_load_bitmap("greets14.gif")) ) Error( "failed to load image" );
	if( !(greets[13]=tyfuus_load_bitmap("greets15.gif")) ) Error( "failed to load image" );
	if( !(greets[14]=tyfuus_load_bitmap("greets16.gif")) ) Error( "failed to load image" );
	if( !(greets[15]=tyfuus_load_bitmap("greets17.gif")) ) Error( "failed to load image" );
	if( !(greets[16]=tyfuus_load_bitmap("greets18.gif")) ) Error( "failed to load image" );
	if( !(greets[17]=tyfuus_load_bitmap("greets19.gif")) ) Error( "failed to load image" );

	if( !(greets[18]=tyfuus_load_bitmap("greets21.gif")) ) Error( "failed to load image" );
	if( !(greets[19]=tyfuus_load_bitmap("greets22.gif")) ) Error( "failed to load image" );
	if( !(greets[20]=tyfuus_load_bitmap("greets23.gif")) ) Error( "failed to load image" );
	if( !(greets[21]=tyfuus_load_bitmap("greets24.gif")) ) Error( "failed to load image" );
	if( !(greets[22]=tyfuus_load_bitmap("greets25.gif")) ) Error( "failed to load image" );
	if( !(greets[23]=tyfuus_load_bitmap("greets26.gif")) ) Error( "failed to load image" );
	if( !(greets[24]=tyfuus_load_bitmap("greets27.gif")) ) Error( "failed to load image" );
	if( !(greets[25]=tyfuus_load_bitmap("greets28.gif")) ) Error( "failed to load image" );
	if( !(greets[26]=tyfuus_load_bitmap("greets29.gif")) ) Error( "failed to load image" );
	if( !(greets[27]=tyfuus_load_bitmap("greets31.gif")) ) Error( "failed to load image" );
	if( !(greets[28]=tyfuus_load_bitmap("greets32.gif")) ) Error( "failed to load image" );
	if( !(greets[29]=tyfuus_load_bitmap("greets33.gif")) ) Error( "failed to load image" );
	if( !(greets[30]=tyfuus_load_bitmap("greets34.gif")) ) Error( "failed to load image" );
	if( !(greets[31]=tyfuus_load_bitmap("greets35.gif")) ) Error( "failed to load image" );
	if( !(greets[32]=tyfuus_load_bitmap("greets36.gif")) ) Error( "failed to load image" );
	if( !(greets[33]=tyfuus_load_bitmap("greets37.gif")) ) Error( "failed to load image" );
	if( !(greets[34]=tyfuus_load_bitmap("greets38.gif")) ) Error( "failed to load image" );
	if( !(greets[35]=tyfuus_load_bitmap("greets39.gif")) ) Error( "failed to load image" );


	/* MODELLER */
	if( !(invertcube = load_lwo("invertcube.lwo")) ) Error( "failed to load object" );
	if( !(spike = load_lwo("spike.lwo")) ) Error( "failed to load object" );
	if( !(kjeller = load_kjeller()) ) Error( "failed to load object" );
	if( !(flower = load_lwo("flower.lwo")) ) Error( "failed to load object" );
	if( !(sphere = load_lwo("sphere.lwo")) ) Error( "failed to load object" );
	if( !(batteri = load_batteri()) ) Error( "failed to load object" );


	/* MATERIALER */
	spike->materials[0].envmap = udefinerbar2->data;
	spike->materials[0].rendermode = RENDER_ENVMAP;

	invertcube->materials[0].texture = invertcube_texture->data;
	invertcube->materials[0].rendermode = RENDER_TEXTUREMAP|RENDER_FLATSHADE;

	flower->materials[0].texture = flower_texture->data;
	flower->materials[0].envmap = udefinerbar->data;
	flower->materials[0].rendermode = RENDER_TEXTUREMAP | RENDER_ENVMAP;

	sphere->materials[0].envmap = udefinerbar->data;
	sphere->materials[0].rendermode = RENDER_ENVMAP;

	sphere_deformed = copy_object( sphere );

	/* funky old kjeller */
	kjeller->materials[1].rendermode = RENDER_TEXTUREMAP | RENDER_FLATSHADE;
	kjeller->materials[1].texture = kjeller_gulv->data;

	kjeller->materials[0].rendermode = RENDER_TEXTUREMAP | RENDER_ENVMAP;
	kjeller->materials[0].texture = kjeller_roer->data;
	kjeller->materials[0].envmap = udefinerbar_env->data;

	/* the magic battery from indre troms */
	batteri->materials[0].rendermode = RENDER_FLATSHADE|RENDER_TEXTUREMAP;
	batteri->materials[0].texture = batteri_shadowbox->data;

	batteri->materials[1].rendermode = RENDER_ENVMAP;
	batteri->materials[1].envmap = udefinerbar->data;

	batteri->materials[2].rendermode = RENDER_ENVMAP;
	batteri->materials[2].envmap = batteri_roer->data;

	batteri->materials[3].rendermode = RENDER_ENVMAP;
	batteri->materials[3].envmap = udefinerbar->data;

	batteri->materials[4].rendermode = RENDER_FLATSHADE|RENDER_TEXTUREMAP;
	batteri->materials[4].texture = batteri_shadow->data;

	batteri->materials[5].rendermode = RENDER_FLATSHADE|RENDER_TEXTUREMAP;
	batteri->materials[5].texture = batteri_vegger->data;

	batteri->materials[6].rendermode = RENDER_FLATSHADE|RENDER_TEXTUREMAP;
	batteri->materials[6].texture = batteri_veggerpr->data;


	memset( instr_skip, 0, 4*16 );
	memset( fx_skip, 0, 4*256 );
	init_tunnel();
	init_kancr();
	grid = tyfuus_make_grid(WIDTH, HEIGHT);
	offset = tyfuus_make_zoommap( WIDTH<<1, HEIGHT<<1 );

	memcpy( motionblur->data, loader->data, 320*240*4 );

	BASSMOD_MusicSetPositionScaler(256);

	instr[0] = BASSMOD_MusicSetSync(BASS_SYNC_MUSICINST, MAKELONG(4,-1), &instr_sync, 0);
	instr[1] = BASSMOD_MusicSetSync(BASS_SYNC_MUSICINST, MAKELONG(0x18,-1), &instr_sync, 1);
	instr[2] = BASSMOD_MusicSetSync(BASS_SYNC_MUSICINST, MAKELONG(1,-1), &instr_sync, 2);
	instr[3] = BASSMOD_MusicSetSync(BASS_SYNC_MUSICINST, MAKELONG(0x1B,-1), &instr_sync, 3);

	instr[4] = BASSMOD_MusicSetSync(BASS_SYNC_MUSICFX, 1, &fx_sync, 0);

//	BASSMOD_MusicSetPosition( MAKELONG(8,0) );

	BASSMOD_MusicPlay();


	while((!GetAsyncKeyState(VK_ESCAPE))&&BASSMOD_MusicIsActive()){
		int timei = BASSMOD_MusicGetPosition();
		int order = LOWORD( timei );
		float row = (float)HIWORD(timei)/256;
		float time = (order*0x80)+ row;

		if(order<2)
			memcpy( screen->data, loader->data, 320*240*4 );
		else if(order<4){

			flat_grid( grid );
			fuck_grid( grid, time*0.001f, 0.2f, (time-2*0x80)/4 );
			tyfuus_expand_grid( screen, grid, loader256 );
			tyfuus_blend( screen, motionblur, 255-4 );
			tyfuus_gamma( screen, 1.001f );
			memcpy( motionblur->data, screen->data, 320*240*4 );

		}else if(order<6){

			float flash = (1-log_sync( last_instr[0], last_instr[0]+5, time ))*2;
			tyfuus_draw_offsetmap( screen, offset,
				(int)((time+instr_skip[0])*7),	(int)(sin(time*0.05f)*200),
				(int)(sin(time*0.1+instr_skip[0])*(WIDTH/2.5)),		(int)(sin(time*-0.073f+instr_skip[0])*(HEIGHT/2.5)),
				(int)(sin(time*0.0713f+instr_skip[0])*(WIDTH/2.5)),	(int)(sin(time*-0.08131f+instr_skip[0])*(HEIGHT/2.5)),
				(int)(sin(time*0.0921f+instr_skip[0])*(WIDTH/2.5)),		(int)(sin(time*-0.101f+instr_skip[0])*(HEIGHT/2.5)),
				texture );
			tyfuus_colorfade( screen, orange, (unsigned char)(flash*64) );
			tyfuus_gamma( screen, (float)(1.f+flash*0.1f) );
			tyfuus_multiply( screen, overlay );

		}else if( order<8 ){
			float blur_sync = 1-log_sync( last_fx[0], last_fx[0]+8, time );
			float gamma_sync = 1-log_sync( last_fx[0], last_fx[0]+15, time );
			start_frame();
			set_fov( 120 );
			set_camera( &cam, vector_scale(vector_normalize(vector_make( (float)sin(time/38+last_fx[0]),-0.5,(float)cos(time/38+last_fx[0]) )),30), vector_make((float)sin(time/16)*8, 4,0), 0 );

			matrix_scale( invertcube->matrix, vector_make(200,200,200) );
			draw_object( screen->data, invertcube, &cam );

			tyfuus_gamma( screen, 1.0f+gamma_sync*0.3f );
			tyfuus_multiply( screen, trash );

			matrix_rotate( spike->matrix, vector_make(time/9,time/8+(float)cos(-time/(M_PI)/4),time/10) );
			matrix_scale( temp_matrix, vector_make(9+blur_sync*3,9+blur_sync*3,9+blur_sync*3) );
			matrix_multiply( spike->matrix, temp_matrix );
			set_fov( 90 );
			draw_object( screen->data, spike, &cam );
			radial_blur(screen->data, (int)(160+sin(sin(time/18))*160), (int)(120+sin(time/19)*120), blur_sync*0.008f, 230);
			radial_blur(screen->data, (int)(160+sin(sin(time/18))*160), (int)(120+sin(time/19)*120), blur_sync*0.016f, 230);
		}else if(order<10){
			float flash = (1-log_sync( last_instr[0], last_instr[0]+5, time ))*2;
			tyfuus_draw_offsetmap( screen, offset,
				(int)((time+instr_skip[0])*7),	(int)(sin(time*0.05f)*200),
				(int)(sin(time*0.1+instr_skip[0])*(WIDTH/2.5)),		(int)(sin(time*-0.073f+instr_skip[0])*(HEIGHT/2.5)),
				(int)(sin(time*0.0713f+instr_skip[0])*(WIDTH/2.5)),	(int)(sin(time*-0.08131f+instr_skip[0])*(HEIGHT/2.5)),
				(int)(sin(time*0.0921f+instr_skip[0])*(WIDTH/2.5)),		(int)(sin(time*-0.101f+instr_skip[0])*(HEIGHT/2.5)),
				texture );
			tyfuus_colorfade( screen, orange, (unsigned char)(flash*64) );
			tyfuus_gamma( screen, (float)(1.f+flash*0.1f) );
			tyfuus_multiply( screen, overlay2 );

			tyfuus_alphablend( screen, dizzydog, (unsigned char)(flash*255.f) );
		}else if(order<12){

			float blur_sync = 1-log_sync( last_fx[0], last_fx[0]+10, time );
			start_frame();
			set_fov( 120 );
			set_camera( &cam, vector_scale(vector_normalize(vector_make( (float)sin(time/38+last_fx[0]),-0.5,(float)cos(time/38+last_fx[0]) )),30), vector_make((float)sin(time/16)*8, 4,0), 0 );

			matrix_scale( invertcube->matrix, vector_make(200,200,200) );
			draw_object( screen->data, invertcube, &cam );
			tyfuus_multiply( screen, trash );
			set_fov( 100 );

			set_camera( &cam, vector_make( 0, 15 ,-20), vector_make(13,4,0), -0.1f );

			matrix_scale(flower->matrix, vector_make(3,3,3));
			matrix_rotate( temp_matrix, vector_make(0,time/20,0));
			matrix_multiply( flower->matrix, temp_matrix );
			draw_object( screen->data, flower, &cam );
/*
		}else if( order==11){
			float blur_sync = 1-log_sync( last_fx[0], last_fx[0]+10, time );
			start_frame();
			set_fov( 120 );
			set_camera( &cam, vector_scale(vector_normalize(vector_make( (float)sin(time/38),-0.5,(float)cos(time/38) )),30), vector_make((float)sin(time/16)*8, 4,0), 0 );

			matrix_scale( invertcube->matrix, vector_make(200,200,200) );
			draw_object( screen->data, invertcube, &cam );

			tyfuus_multiply( screen, trash );

			matrix_rotate( spike->matrix, vector_make(time/7,time/8+(float)cos(-time/(M_PI)/2),time/8) );
			matrix_scale( temp_matrix, vector_make(10,10,10) );
			matrix_multiply( spike->matrix, temp_matrix );
			set_fov( 90 );
			draw_object( screen->data, spike, &cam );
*/
			if( order==11 && row>64){
				float intime = (row-64)/64;
				blur_sync = intime*2;
				if( intime>0.5f )
					tyfuus_blend( screen, noname, (unsigned char)((intime-0.5f)*511) );
			}
			tyfuus_gamma( screen, 1.0f+blur_sync*0.08f );

			radial_blur(screen->data, (int)(160+sin(sin(time/18))*160), (int)(120+sin(time/19)*120), blur_sync*0.01f, 230);
			radial_blur(screen->data, (int)(160+sin(sin(time/18))*160), (int)(120+sin(time/19)*120), blur_sync*0.02f, 230);
		}else if( order==12){
			float blur_sync;
			unsigned char alpha;
			if( row<64 ){
				blur_sync = (1.f-row/64)*2;
				alpha = 230;
			}else{
				blur_sync = 0.f;
				alpha = (unsigned char)(230-(row-64)*(230.f/64.f));
			}

			if(blur_sync<0.f) blur_sync = 0.f;
			memcpy( screen->data, noname->data, 320*240*4 );
			tyfuus_gamma( screen, 1.0f+blur_sync*0.08f );
			radial_blur(screen->data, (int)(160+sin(sin(time/18))*160), (int)(120+sin(time/19)*120), blur_sync*0.01f,alpha);
			radial_blur(screen->data, (int)(160+sin(sin(time/18))*160), (int)(120+sin(time/19)*120), blur_sync*0.02f,alpha);
		}else if( order<15){
			memcpy( screen->data, noname->data, 320*240*4 );
		}else if( order==15){

			vector centrum = vector_make(0,0,25);
			start_frame();
			set_fov( 120 );
			set_camera( &cam, vector_add( centrum, vector_make( (float)sin(time/32+instr_skip[0])*12, -4, (float)cos(time/32+instr_skip[0])*12)), centrum, 0 );

			matrix_scale( kjeller->matrix, vector_make(100, 100, 100) );
			matrix_rotate( temp_matrix, vector_make( -M_PI/2, 0, 0 ) );
			matrix_multiply( kjeller->matrix, temp_matrix );
			matrix_translate( temp_matrix, vector_make( 0, 0, 0 ) );
			matrix_multiply( kjeller->matrix, temp_matrix );
			draw_object( screen->data, kjeller, &cam );

			matrix_translate( sphere_deformed->matrix, centrum);
			matrix_scale( temp_matrix, vector_make(0.5,0.5,0.5));
			matrix_multiply( sphere_deformed->matrix, temp_matrix );
			blob_distort( sphere, sphere_deformed,
				vector_make(time*0.07f,-time*0.0812f,time*0.08333f),
				vector_make(0.5f,0.51f,0.3f),
				vector_make(0.3f,0.3f,0.3f) );
			draw_object( screen->data, sphere_deformed, &cam );

			tyfuus_blend( screen, motionblur, 235 );
			memcpy( motionblur->data, screen->data, 320*240*4 );

			tyfuus_blend( screen, noname, (unsigned char)((128-row)*(1.99f)) );
		}else if(order<22){
			float sync = 1-log_sync( last_instr[2], last_instr[2]+5, time );
			vector centrum = vector_make(0,0,25);
			start_frame();
			set_fov( 120+sync*15 );
			set_camera( &cam, vector_add( centrum, vector_make( (float)sin(time/32+instr_skip[0])*12, -4, (float)cos(time/32+instr_skip[0])*12)), centrum, 0 );

			matrix_scale( kjeller->matrix, vector_make(100, 100, 100) );
			matrix_rotate( temp_matrix, vector_make( -M_PI/2, 0, 0 ) );
			matrix_multiply( kjeller->matrix, temp_matrix );
			matrix_translate( temp_matrix, vector_make( 0, 0, 0 ) );
			matrix_multiply( kjeller->matrix, temp_matrix );
			draw_object( screen->data, kjeller, &cam );

			blob_distort( sphere, sphere_deformed,
				vector_make(time*0.07f+sync,-time*0.0812f+sync,time*0.08333f+sync),
				vector_make(0.5f,0.51f,0.3f),
				vector_make(0.3f,0.3f,0.3f) );

			matrix_translate( sphere_deformed->matrix, centrum);
			matrix_scale( temp_matrix, vector_make(0.5f,0.5f,0.5f));
			matrix_multiply( sphere_deformed->matrix, temp_matrix );
			draw_object( screen->data, sphere_deformed, &cam );

			tyfuus_blend( screen, motionblur, 235 );
			memcpy( motionblur->data, screen->data, 320*240*4 );
			tyfuus_gamma( screen, 1.0f+sync*0.15f );

			if( order==0x12||order==0x13 )
				if((instr_skip[3]-1)<18)
					tyfuus_multiply( screen, greets[instr_skip[3]-1]);

		}else if(order<26){
			float flash = 1-log_sync( last_fx[0], last_fx[0]+3, time );
			float flash2 = 0.f;
			float sync = 1-log_sync( last_instr[2], last_instr[2]+5, time );

			flat_grid( grid );
			fuck_grid( grid, time/10, 0.2f, 45.5f );

			if(order==23&&flash>0.f){
				flash2 = flash;
				flash = 0;
				tyfuus_expand_grid( blendbuffer, grid, deform );
				radial_blur(blendbuffer->data, (int)(160+sin(sin(time/18))*160), (int)(120+sin(time/19)*120), 0.1f, 190);
				radial_blur(blendbuffer->data, (int)(160+sin(1+sin(time/18))*160), (int)(120+sin(1+time/19)*120), 0.1f, 190);
				memset( screen->data, 0, 320*240*4 );
				bamm_blit( screen->data, blendbuffer->data, flash2*70 );
			}else{
				tyfuus_expand_grid( screen, grid, deform );
				radial_blur(screen->data, (int)(160+sin(sin(time/18))*160), (int)(120+sin(time/19)*120), 0.1f, 190);
				radial_blur(screen->data, (int)(160+sin(1+sin(time/18))*160), (int)(120+sin(1+time/19)*120), 0.1f, 190);
			}


			tyfuus_gamma( screen, 1.1f+sync*0.2f );
			tyfuus_colorfade( screen, 0xFFFFFF, (unsigned char)(flash*255));

			if( order==24||order==25 )
					tyfuus_multiply( screen, greets[(instr_skip[3]-1)%36]);

		}else if( order<30){
			float sync = 1-log_sync( last_instr[2], last_instr[2]+10, time );
			float sync2 = 1-log_sync( last_instr[3], last_instr[3]+10, time );

			set_fov( 130 );
			start_frame();

			if(order>27) time += instr_skip[2]*15;


			switch(last_fx_value){

			case 0xFF:
				set_camera( &cam, vector_make( (float)sin(time/16)*20, 10, (float)cos(time/16)*18), vector_make(0,0,0), 0 );
				break;

			case 0xFE:
				set_camera( &cam, vector_make( (float)sin(time/24)*18, 17, 3+(float)cos(time/32)*4), vector_make(0,0,0), 0 );
				break;

			case 0xFD:
				set_camera( &cam, vector_make( (float)sin(time/32+1)*18, 14, (float)cos(time/16)*16), vector_make(0,0,0), 0 );
				break;

			default:
				set_camera( &cam, vector_make( (float)sin(time/16)*28, -3, (float)-cos(time/16)*21-1), vector_make(0,2,0), 0 );
				break;

			}
			matrix_scale( batteri->matrix, vector_make(70,70,70) );
			matrix_rotate( temp_matrix, vector_make(-M_PI/2,0,0) );
			matrix_multiply( batteri->matrix, temp_matrix );
			draw_object( screen->data, batteri, &cam );
			tyfuus_gamma( screen, 1.1f );
			if(order==26) tyfuus_multiply( screen, gastanks[(textsync-1)%5] );
			if(order==27) tyfuus_multiply( screen, gas[(textsync-1)%5] );
			if(order==28) tyfuus_multiply( screen, walterkluge[(instr_skip[2])&3] );
			if(order==29) tyfuus_multiply( screen, standup[(instr_skip[2])&3] );
		}else{
			memcpy( screen->data, spaceflowers->data, 320*240*4 );
		}

		leepra_update( screen->data );
	}

	BASSMOD_MusicFree();
	BASSMOD_Free();
//	ExitProcess(0);
	leepra_close();

	tyfuus_free_offsetmap( offset );
	tyfuus_free_grid( grid );

	free_object( spike );
	free_object( invertcube );
	free_object( kjeller );
	free_object( batteri );
	free_object( sphere );
	free_object( sphere_deformed );
	free_object( flower );

	tyfuus_free_bitmap(texture);
	tyfuus_free_bitmap(overlay);
	tyfuus_free_bitmap(overlay2);
	tyfuus_free_bitmap(loader);
	tyfuus_free_bitmap(loader256);
	tyfuus_free_bitmap(dizzydog);
	tyfuus_free_bitmap(noname);
	tyfuus_free_bitmap(udefinerbar);
	tyfuus_free_bitmap(udefinerbar2);
	tyfuus_free_bitmap(udefinerbar_env);
	tyfuus_free_bitmap(deform);
	tyfuus_free_bitmap(flower_texture);
	tyfuus_free_bitmap(spaceflowers);
	tyfuus_free_bitmap(trash);

	tyfuus_free_bitmap(kjeller_gulv);
	tyfuus_free_bitmap(kjeller_roer);

	tyfuus_free_bitmap(batteri_shadowbox);
	tyfuus_free_bitmap(batteri_shadow);
	tyfuus_free_bitmap(batteri_vegger);
	tyfuus_free_bitmap(batteri_veggerpr);
	tyfuus_free_bitmap(batteri_roer);

	tyfuus_free_bitmap(invertcube_texture);
	tyfuus_free_bitmap(spike_envmap);

	tyfuus_free_bitmap(screen);
	tyfuus_free_bitmap(motionblur);
	tyfuus_free_bitmap(blendbuffer);

	tyfuus_free_bitmap(walterkluge[0]);
	tyfuus_free_bitmap(walterkluge[1]);
	tyfuus_free_bitmap(walterkluge[2]);
	tyfuus_free_bitmap(walterkluge[3]);

	tyfuus_free_bitmap(gastanks[0]);
	tyfuus_free_bitmap(gastanks[1]);
	tyfuus_free_bitmap(gastanks[2]);
	tyfuus_free_bitmap(gastanks[3]);
	tyfuus_free_bitmap(gastanks[4]);

	tyfuus_free_bitmap(gas[0]);
	tyfuus_free_bitmap(gas[1]);
	tyfuus_free_bitmap(gas[2]);
	tyfuus_free_bitmap(gas[3]);
	tyfuus_free_bitmap(gas[4]);

	tyfuus_free_bitmap(standup[0]);
	tyfuus_free_bitmap(standup[1]);
	tyfuus_free_bitmap(standup[2]);
	tyfuus_free_bitmap(standup[3]);

	tyfuus_free_bitmap(greets[0]);
	tyfuus_free_bitmap(greets[1]);
	tyfuus_free_bitmap(greets[2]);
	tyfuus_free_bitmap(greets[3]);
	tyfuus_free_bitmap(greets[4]);
	tyfuus_free_bitmap(greets[5]);
	tyfuus_free_bitmap(greets[6]);
	tyfuus_free_bitmap(greets[7]);
	tyfuus_free_bitmap(greets[8]);
	tyfuus_free_bitmap(greets[9]);
	tyfuus_free_bitmap(greets[10]);
	tyfuus_free_bitmap(greets[11]);
	tyfuus_free_bitmap(greets[12]);
	tyfuus_free_bitmap(greets[13]);
	tyfuus_free_bitmap(greets[14]);
	tyfuus_free_bitmap(greets[15]);
	tyfuus_free_bitmap(greets[16]);
	tyfuus_free_bitmap(greets[17]);
	tyfuus_free_bitmap(greets[18]);
	tyfuus_free_bitmap(greets[19]);
	tyfuus_free_bitmap(greets[20]);
	tyfuus_free_bitmap(greets[21]);
	tyfuus_free_bitmap(greets[22]);
	tyfuus_free_bitmap(greets[23]);
	tyfuus_free_bitmap(greets[24]);
	tyfuus_free_bitmap(greets[25]);
	tyfuus_free_bitmap(greets[26]);
	tyfuus_free_bitmap(greets[27]);
	tyfuus_free_bitmap(greets[28]);
	tyfuus_free_bitmap(greets[29]);
	tyfuus_free_bitmap(greets[30]);
	tyfuus_free_bitmap(greets[31]);
	tyfuus_free_bitmap(greets[32]);
	tyfuus_free_bitmap(greets[33]);
	tyfuus_free_bitmap(greets[34]);
	tyfuus_free_bitmap(greets[35]);

	return 0;
}


LRESULT CALLBACK dialog_callback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message){
	case WM_COMMAND:
		if(LOWORD(wParam) == IDOK) {
			if(IsDlgButtonChecked(hDlg, IDC_WINDOWED)) windowed=TRUE;
			if(IsDlgButtonChecked(hDlg, IDC_NOSOUND)) nosound=TRUE;
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}else if(LOWORD(wParam)==2){
			EndDialog( hDlg, LOWORD(wParam));
			ExitProcess(0);
		}
	break;
	case WM_CLOSE:
		EndDialog( hDlg, LOWORD(wParam));
		ExitProcess(0);
	break;
	}

	return FALSE;
}