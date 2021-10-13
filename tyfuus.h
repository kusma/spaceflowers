/*
	Basic bitmap-routines
*/
typedef struct{
	unsigned int width;
	unsigned int height;
	unsigned int* data;
}tyfuus_bitmap;

typedef struct{
	unsigned int width;
	unsigned int height;
	unsigned int* data;
}tyfuus_offsetmap;

typedef struct{
	float u, v;
	unsigned char shade;
}tyfuus_grid_node;

typedef struct{
	unsigned int width;
	unsigned int height;
	tyfuus_grid_node *data;
}tyfuus_grid;


void tyfuus_multiply( tyfuus_bitmap *bitmap1, tyfuus_bitmap *bitmap2 );
void tyfuus_blend( tyfuus_bitmap *bitmap1, tyfuus_bitmap *bitmap2, unsigned char alpha );
void tyfuus_alphablend( tyfuus_bitmap *bitmap1, tyfuus_bitmap *bitmap2, unsigned char alpha );
void tyfuus_alphablend_opaque( tyfuus_bitmap *bitmap1, tyfuus_bitmap *bitmap2 );

void tyfuus_colorfade( tyfuus_bitmap *bitmap, unsigned int color, unsigned char alpha );
void tyfuus_gamma( tyfuus_bitmap *bitmap1, float gamma );
void tyfuus_invert( tyfuus_bitmap *bitmap1, unsigned char invert );

tyfuus_bitmap *tyfuus_load_bitmap( char *filename );
tyfuus_bitmap *tyfuus_load_bitmap_alpha( char *filename, char *alphaname );
tyfuus_bitmap *tyfuus_make_bitmap( int width, int height );
void tyfuus_free_bitmap( tyfuus_bitmap *bitmap );

tyfuus_offsetmap *tyfuus_make_radialmap( int width, int height );
tyfuus_offsetmap *tyfuus_make_zoommap( int width, int height );
void tyfuus_free_offsetmap( tyfuus_offsetmap *offsetmap );
void tyfuus_draw_offsetmap(tyfuus_bitmap *bitmap, tyfuus_offsetmap *offset_table, int xoffs, int yoffs, int x1, int y1, int x2, int y2, int x3, int y3, tyfuus_bitmap *texture );

void tyfuus_free_grid( tyfuus_grid *grid );
tyfuus_grid *tyfuus_make_grid( int width, int height );
void tyfuus_expand_grid( tyfuus_bitmap *bitmap, tyfuus_grid *grid, tyfuus_bitmap *texture );

void tyfuus_init_font( int width, int height, char *fontname, int size );
void tyfuus_free_font();
void tyfuus_print( tyfuus_bitmap *target, int xpos, int ypos, char* text, unsigned int color );


/* lowlevel */