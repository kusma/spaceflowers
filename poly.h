#ifndef __POLY_H__
#define __POLY_H__

void draw_flat_triangle( unsigned int *buffer, vertex_2d v[3], unsigned int color  );
void draw_texture_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], unsigned int *texture );
void draw_perfect_texture_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], unsigned int *texture );
void draw_perfect_texture_flat_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], unsigned int *texture, unsigned int shade );
void draw_perfect_multi_texture_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], texcoord euv[3], unsigned int *texture1, unsigned int *texture2 );
void draw_perfect_bump_texture_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], texcoord euv[3], unsigned int *texture1, unsigned int *texture2 );
void draw_perfect_envbump_texture_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], texcoord euv[3], unsigned int *texture1, unsigned int *texture2 );

//void draw_line( unsigned int* buffer, vertex_2d v1, vertex_2d v2, unsigned int color );
void draw_pixel( unsigned int *screen, vertex_2d v );

#endif /* __POLY_H__ */