#ifndef __KANCR_H__
#define __KANCR_H__

#include "vector.h"
#include "matrix.h"

typedef struct{
	float x, y;
	float z;
}vertex_2d;

typedef struct{
	int index;
	int normal;
	int texcoord;
}vertex;

typedef struct{
	float u, v;
}texcoord;


#define RENDER_FLATSHADE			1
#define RENDER_GOURARDSHADE			2
#define RENDER_TEXTUREMAP			4
#define RENDER_PERSPECTIVECORRECT	8
#define RENDER_ENVMAP				16
#define RENDER_PHONGMAP				32
#define RENDER_BUMPMAP				64
#define RENDER_ENVBUMPMAP			128
/* ... */
#define RENDER_DOUBLESIDED			2147483648

typedef struct{
	unsigned int color;
	unsigned int *texture;
	unsigned int *envmap;
	unsigned int rendermode;
}material;

typedef struct{
	vertex vertex[3];
	vector normal;

	int material;
}face;

typedef struct{
	matrix matrix;

	BOOL recalc_normals;
	BOOL recalc_bounding_sphere;
	float bounding_sphere;

	int pointcount;
	vector *points;

	int normalcount;
	vector *normals;

	int texcoordcount;
	texcoord *texcoords;

	int materialcount;
	material *materials;

	int facecount;
	face *faces;
}object;

typedef struct{
	vector normal;
	float distance;
}plane;

typedef struct{
	vector position;
	vector target;
	float roll;
	matrix matrix;
}camera;

void init_kancr();
void set_fov( float input );

object *make_object(
	vector *points, int pointcount,
	int *point_indices, int *uv_indices, int *material_indices, int facecount,
	float *texcoords, int texcoordcount,
	material *materials, int materialcount);

object *copy_object( object *source );

void free_object( object *object );
	
void generate_normals( object *object );
float bounding_sphere( object *object );

void start_frame();
void draw_object( unsigned int *screen, object *object, camera *camera );

void set_camera( camera *cam, vector pos, vector target, float roll );

object *load_lwo(char *filename);

#define WIDTH 512
#define HEIGHT 384

#include "poly.h"


#endif /* __KANCR_H__ */
