/*
	Lib for Eventyrlig og Ekstatisk Pixelbasert Realtime Animasjon
	aka LEEPRA.
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef __cplusplus
extern "C"{
#endif

/*
	There might be some braindead compilers out there.
*/

int leepra_open( char* title, int width, int height, BOOL fullscreen);
HWND leepra_get_window();
void leepra_update( void* data );
void leepra_close();

#ifdef __cplusplus
}
#endif
