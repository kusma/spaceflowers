#ifndef __3D_H__
#define __3D_H__

#ifdef WIDTH
#undef WIDTH
#endif
#ifdef HEIGHT
#undef HEIGHT
#endif

#define WIDTH 512
#define HEIGHT 256

#include "vector.h"
//#include "2d.h"
#include "matrix.h"

typedef struct{
	int index;
	float u, v;
	Vector normal;
}Vertex;

typedef struct{
	unsigned int *texture;
	unsigned int *lightmap;
}Material;

typedef struct{
	Vertex a, b, c;
	Vector normal;
	int smooth;
	int mesh;
//	Material *material;
}Face;

typedef struct{
	Material *material;
}Mesh;

typedef struct{
	Matrix matrix;
	Vector *vertices;
	int vertex_count;
	Face *faces;
	int face_count;
	Mesh *meshes;
	int mesh_count;
}Object;

typedef struct{
	int x, y, z;
	int u,v;
	int u2, v2;
}Vertex2D;


Object *load_lwo( char *filename );

// lowlevel
void generate_normals( Object& object );
void render_object( Object &object, unsigned int *buffer );

// lowest level
void flat_triangle( unsigned int *target, Vertex2D v[3], unsigned int color );

#endif