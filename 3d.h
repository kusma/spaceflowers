#ifndef __3D_H__
#define __3D_H__

#include "vector.h"
#include "matrix.h"

typedef struct{
	int index;
	int normal;
	float u, v;
}vertex;

typedef struct{
	unsigned int *texture;
	unsigned int *lightmap;
}material;

typedef struct{
	vertex vertex[3];
	vector normal;
	int smooth;
	int mesh;
}face;

typedef struct{
	material *material;
}mesh;

typedef struct{
	matrix matrix;
	vector *vertices;
	int vertex_count;
	vector *normals;
	int normal_count;
	face *faces;
	int face_count;
	mesh *meshes;
	int mesh_count;
}object;

typedef struct{
	int x, y;
	unsigned int z;
	int u,v;
	int u2, v2;
}vertex2D;

#include <math.h>

/* lowlevel */
void start_frame();
void generate_normals( object *object );
void render_object( object *object, unsigned int *buffer );
/*
  void end_frame( unsigned int *buffer ); <- tegne et s-buffer?
*/

/* lowest level */
void flat_triangle( unsigned int *target, vertex2D v[3], unsigned int color );
void texture_triangle( unsigned int* target, vertex2D v[3], unsigned int* texture );

#endif /* __3D_H__ */