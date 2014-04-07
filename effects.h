#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "kancr.h"
#include "matrix.h"

void flat_grid( tyfuus_grid *grid );
void init_tunnel();
void render_tunnel( tyfuus_grid *grid, matrix m );
void render_plane( tyfuus_grid *grid, matrix m );
void fuck_grid( tyfuus_grid *grid, float time, float wavelen, float strength );
void radial_blur( unsigned int *buffer, int xpos, int ypos, float strength, unsigned char blur_opacity );
void bamm_blit( unsigned int *dst, unsigned int *src, int amount );
//void bamm( unsigned int *dst, int amount );
void blob_distort( object *source, object *destination, vector wobble, vector dill, vector strength );