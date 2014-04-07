#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include "tyfuus.h"
#include "vector.h"
#include "matrix.h"
#include "kancr.h"

#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979
#endif


#define WIDTH 320
#define HEIGHT 240

vector direction[((WIDTH/8)+1)*((HEIGHT/8)+1)];

#define TUNNEL_RADIUS 2.0f
float tunnel_radius = TUNNEL_RADIUS;
float tunnel_radius_squared = TUNNEL_RADIUS*TUNNEL_RADIUS;

_inline float tunnel_intersect( vector origin, vector direction ){
	float a = (direction.x*direction.x) + (direction.y*direction.y);
	float b = 2*(origin.x*direction.x + origin.y*direction.y);
	float c = (origin.x*origin.x) + (origin.y*origin.y) - tunnel_radius_squared;

	float delta = (b*b) - 4*a*c;

#if 0
	float t = (-b -sqrt(delta))/(2*a);
#else
	float sqrtd = (float)sqrt(delta);
	float amul = 1.0f/(2*a);
	float t1 = (-b + sqrtd)*amul;
	float t2 = (-b - sqrtd)*amul;
	float t=t1;
	if(t2<t1) t=t2;
#endif
	return t;
}

vector plane_normal = {0,1,0};
float plane_distance = 1;
_inline float plane_intersect( vector origin, vector direction ){
	float t;
	origin = vector_make( 10,0,0 );
	t = vector_dotproduct( plane_normal, direction ) / -( vector_dotproduct( plane_normal, origin ) + (plane_distance) );
	return t;
}


vector trace_tunnel_ray( vector origin, vector direction ){
	vector intersection;
	float t = tunnel_intersect( origin, direction );
	intersection = vector_add(origin,vector_scale(direction,t));
	return intersection;
}

vector trace_plane_ray( vector origin, vector direction ){
	vector intersection;
	float t = plane_intersect( origin, direction );
	intersection = vector_add(origin,vector_scale(direction,t));
	return intersection;
}


void init_tunnel(){
	int x,y;
	vector* dir = direction;

	for(y=0; y<(HEIGHT/8+1); y++)
		for(x=0; x<(WIDTH/8+1); x++){
			*dir = vector_make(
				((float)x)-((WIDTH/8+1)/2),
				((float)-y)+((HEIGHT/8+1)/2),
				15.0f );
			vector_normalize( *dir );
			dir++;
		}
}

void render_tunnel( tyfuus_grid *grid, matrix m ){
	int i;
	vector origin = vector_make(m[12],m[13],m[14]);
	vector *dir = direction;
	tyfuus_grid_node *node = grid->data;
	for(i=(grid->width*grid->height);i;i--){
		int shade;
		vector direction = matrix_rotatevector( m, *dir++ );
		vector intersection = trace_tunnel_ray( origin, direction );
		
		node->u = (float)fabs(atan2(intersection.x, intersection.y)*(256.f/M_PI));
		node->v = (intersection.z*32.f);

		shade = (int)(fabs((origin.z-intersection.z))*32);
		if(shade>255) shade=255;
		if(shade<0) shade=0;
		node->shade = ~(char)shade;
		node++;
	}
}

void render_plane( tyfuus_grid *grid, matrix m ){
	int i;
	vector origin = vector_make(m[12],m[13],m[14]);

	vector *dir = direction;
	tyfuus_grid_node *node = grid->data;
	for(i=(grid->width*grid->height);i;i--){
		vector direction = matrix_rotatevector( m, *dir++ );
		vector intersection = trace_plane_ray( origin, direction );
		
		node->u = intersection.x;
		node->v = intersection.y;
		node->shade = 255;

		node++;
	}
}


void flat_grid( tyfuus_grid *grid ){
	int x,y;
	int width=grid->width;
	int height=grid->height;
	tyfuus_grid_node *node = grid->data;

	for(y=0;y<height;y++)
		for(x=0;x<width;x++){
			node->u = ((float)256/240)*(y*8);
			node->v = ((float)256/320)*(x*8);
			node++;
		}
}

void fuck_grid( tyfuus_grid *grid, float time, float wavelen, float strength ){
	int x,y;
	int width=grid->width;
	int height=grid->height;
	tyfuus_grid_node *node = grid->data;

	for(y=0;y<height;y++)
		for(x=0;x<width;x++){
			float sx = (float)sin(time+(x*wavelen));
			float sy = (float)sin(time*1.1f+(y*wavelen));
			node->u += (float)sin(sx)*strength;
			node->v += (float)cos(sy+sx)*strength;
			node++;
		}
}

__inline unsigned int blend_colors(unsigned int c1, unsigned int c2){
	_asm{
		movd mm0, c1
		movd mm1, c2
		punpcklbw mm0, mm5
		punpcklbw mm1, mm5

		pmullw mm0, mm7
		psrlw mm0, 8
		pmullw mm1, mm6
		psrlw mm1, 8
		paddw mm0, mm1

		packuswb mm0, mm5
		movd c1, mm0
	}
	return c1;
}

__inline unsigned int get_subpixel320( unsigned int *source, unsigned int x, unsigned int y ){
	unsigned int color;
	unsigned int xblend, yblend;

	xblend = x&0xFF;
	xblend |= (xblend<<24)|(xblend<<16)|(xblend<<8);
	yblend = y&0xFF;
	yblend |= (yblend<<24)|(yblend<<16)|(yblend<<8);

	_asm{
		mov esi, source
		mov eax, x
		mov ebx, y
		shr eax, 8
		shr ebx, 8

		lea ebx, [ebx + ebx*4]
		shl ebx, 6
		add ebx, eax

		movd mm0, [esi+ebx*4]
		movd mm1, [esi+ebx*4+4]
		mov eax, 0xFFFFFFFF
		movd mm3, eax
		movd mm4, xblend
		punpcklbw mm0, mm5
		punpcklbw mm1, mm5
		punpcklbw mm3, mm5
		punpcklbw mm4, mm5
		psubw mm3, mm4

		pmullw mm1, mm4
		pmullw mm0, mm3
		paddw mm0, mm1

		movd mm1, [esi+ebx*4+320*4]
		movd mm2, [esi+ebx*4+321*4]
		psrlw mm0, 8
		punpcklbw mm1, mm5
		punpcklbw mm2, mm5
		pmullw mm1, mm3
		pmullw mm2, mm4
		paddw mm1, mm2

		mov eax, 0xFFFFFFFF
		movd mm3, eax
		movd mm4, yblend
		punpcklbw mm3, mm5
		punpcklbw mm4, mm5
		psrlw mm1, 8
		psubw mm3, mm4

		pmullw mm1, mm4
		pmullw mm0, mm3
		paddw mm0, mm1
		psrlw mm0, 8

		packuswb mm0, mm5
		movd color, mm0
	}
	return color;
}

//#define BLUR_OPACITY 230
void radial_blur( unsigned int *buffer, int xpos, int ypos, float strength, unsigned char blur_opacity ){
	int x,y;
	unsigned int old_pixel;
	unsigned int new_pixel;
	int newx, newy;
	unsigned int *trask_me_baby;
	int sdiv;

//	if( strength==0.f ) return;

	sdiv = (int)(65535.f/(1.0f+strength));

	_asm{
		pxor mm5, mm5
		xor eax, eax
		mov al, blur_opacity
		mov ah, al
		mov ebx, eax
		shl ebx, 16
		or eax, ebx

		movd mm6, eax
		xor eax, eax
		sub eax, 1
		movd mm7, eax
		punpcklbw mm6, mm5
		punpcklbw mm7, mm5
		psubw mm7, mm6
	}

	for(y=ypos,trask_me_baby=buffer+y*WIDTH; y>-1; y--){
		newy = ((ypos-y)*sdiv)>>8;
		for(x=xpos; x>-1; x--){
			newx = ((xpos-x)*sdiv)>>8;
			old_pixel = *(trask_me_baby+x);
			new_pixel = get_subpixel320(buffer, (xpos<<8)-newx , (ypos<<8)-newy);
			new_pixel = blend_colors(old_pixel, new_pixel);
			buffer[x+(y*WIDTH)] = new_pixel;
		}
		for(x=xpos+1; x<WIDTH; x++){
			newx = ((xpos-x)*sdiv)>>8;
			old_pixel = *(trask_me_baby+x);
			new_pixel = get_subpixel320(buffer, (xpos<<8)-newx , (ypos<<8)-newy);
			new_pixel = blend_colors(old_pixel, new_pixel);
			buffer[x+(y*WIDTH)] = new_pixel;
		}
		trask_me_baby -= WIDTH;
	}
	for(y=ypos+1,trask_me_baby=buffer+y*WIDTH; y<HEIGHT; y++){
		newy = ((ypos-y)*sdiv)>>8;
		for(x=xpos; x>-1; x--){
			newx = ((xpos-x)*sdiv)>>8;
			old_pixel = *(trask_me_baby+x);
			new_pixel = get_subpixel320(buffer, (xpos<<8)-newx , (ypos<<8)-newy);
			new_pixel = blend_colors(old_pixel, new_pixel);
			buffer[x+(y*WIDTH)] = new_pixel;
		}
		for(x=xpos+1; x<WIDTH; x++){
			newx = ((xpos-x)*sdiv)>>8;
			old_pixel = *(trask_me_baby+x);
			new_pixel = get_subpixel320(buffer, (xpos<<8)-newx , (ypos<<8)-newy);
			new_pixel = blend_colors(old_pixel, new_pixel );
			buffer[x+(y*WIDTH)] = new_pixel;
		}
		trask_me_baby += WIDTH;
	}
	_asm emms;
}

unsigned long irand( void ){ 
   static unsigned long randSeed = 22222; 
   randSeed = (randSeed * 196314165) + 907633515; 
   return randSeed; 
}

void bamm_blit( unsigned int *dst, unsigned int *src, int amount ){
	int y;

	if(amount==0){
		memcpy( dst, src, 320*240*4 );
		return;
	}

	for( y=240; y; y-- ){
		int offset = (irand()%(amount+1))-(amount>>1);
		if(offset>300)offset=300;
		if(offset<-300)offset=-300;

		if(offset<0){
			memcpy( dst, (src-offset), (320+offset)*sizeof(unsigned int));
			dst += 320+offset;
			memset( dst, 0, -offset);
			dst -= offset;
		}else{
			memset( dst, 0, offset );
			dst += offset;
			memcpy( dst, src, (320-offset)*sizeof(unsigned int));
			dst += 320-offset;
		}
		src += 320;
	}

}

void blob_distort( object *source, object *destination, vector wobble, vector dill, vector strength ){
	int i;
	for( i = 0; i<source->pointcount; i++ ){
		vector temp = source->points[i];
		float mul = 1.f;
		mul += (float)sin((temp.x*dill.z)+wobble.x)*strength.x;
		mul += (float)sin((temp.y*dill.y)+wobble.y)*strength.y;
		mul += (float)sin((temp.z*dill.z)+wobble.z)*strength.z;
		destination->points[i] = vector_scale(temp, mul);
	}
	destination->recalc_normals = TRUE;
}