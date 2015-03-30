#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "kancr.h"

typedef struct{
	int start, stop;
	unsigned int zstart, zstop;
	int ustart, ustop;
	int vstart, vstop;
	int u2start, u2stop;
	int v2start, v2stop;
}poly_edge;

poly_edge edges[HEIGHT];
unsigned int zbuffer[WIDTH*HEIGHT];

__inline void xchg_int( int *a, int *b ){
	int c = *a;
	*a = *b;
	*b = c;
}

_inline void xchg_vertex2d( vertex_2d *a, vertex_2d *b){
	vertex_2d piss = *a;
	*a = *b;
	*b = piss;
}

_inline void xchg_texcoord( texcoord *a, texcoord *b){
	texcoord piss = *a;
	*a = *b;
	*b = piss;
}

__inline void flat_hline( unsigned int* target, unsigned int* ztarget, poly_edge *edge, int y, unsigned int color ){
	unsigned int counter;
	unsigned int *bufferp;
	unsigned int *zbufferp;
	unsigned int z;

	int x1 = edge->start;
	int x2 = edge->stop;
	int z1 = edge->zstart;
	int z2 = edge->zstop;
	int zdelta;

	if( x1==x2 ) return;
	if( x2 < x1 ){
		xchg_int( &x1, &x2 );
		xchg_int( &z1, &z2 );
	}

	zdelta = (z2-z1)/(x2-x1);
	z = z1 + (zdelta>>1);

	bufferp = target+x1;
	zbufferp = ztarget+x1;

	for( counter=x2-x1; counter; counter-- ){
		if(*zbufferp<z){
			*bufferp = color;
			*zbufferp = z;
		}
		z += zdelta;
		bufferp++;
		zbufferp++;
	}
}

_inline BOOL backface_2d( vertex_2d v[3] ){
  float dx1, dy1, dx2, dy2;

  dx1 = v[2].x - v[0].x;
  dy1 = v[2].y - v[0].y;
  dx2 = v[2].x - v[1].x;
  dy2 = v[2].y - v[1].y;

  if( ( dx1 * ( dy2 - dy1 ) - ( dx2 - dx1) * dy1 ) > 0) return TRUE;
  else return FALSE;
}

void draw_flat_triangle( unsigned int *buffer, vertex_2d v[3], unsigned int color ){
	poly_edge *current_edge = edges;
	unsigned int *zbuffer_shit = zbuffer;
	int total_len = 0;

	float x1, x2;
	float z1, z2;
	float xdelta1, xdelta2;
	float zdelta1, zdelta2;

	float ylen1, ylen2;
	float xlen1, xlen2;
	float zlen1, zlen2;
	float ex, ey;
	float zdiv[3];

	float ymul1;
	float ymul2;

	int counter;
	int ay, by;

	int fxdelta1, fxdelta2;
	int fzdelta1, fzdelta2;
	int fx1, fx2;
	int fz1, fz2;
	int y;

	if(v[1].y > v[2].y) xchg_vertex2d(&v[1], &v[2]);
	if(v[0].y > v[2].y) xchg_vertex2d(&v[0], &v[2]);
	if(v[0].y > v[1].y) xchg_vertex2d(&v[0], &v[1]);

	ylen1 = v[1].y-v[0].y;
	ylen2 = v[2].y-v[1].y;

	// kalkuler lengste scanline (den ved v[1]) osv,
	// for å slippe divisjoner i hline
	zdiv[0] = 1.0f/v[0].z;
	zdiv[1] = 1.0f/v[1].z;
	zdiv[2] = 1.0f/v[2].z;

	xlen1 = v[1].x-v[0].x;
	xlen2 = v[2].x-v[0].x;
	zlen1 = zdiv[1]-zdiv[0];
	zlen2 = zdiv[2]-zdiv[0];

	ymul1 = 1.f/(ylen1+1);
	ymul2 = 1.f/(ylen1+ylen2+1);

	xdelta1 = xlen1*ymul1;
	xdelta2 = xlen2*ymul2;
	zdelta1 = zlen1*ymul1;
	zdelta2 = zlen2*ymul2;

	ey = ((int)(v[0].y+1.f))-v[0].y;
	ex = xdelta1*ey;
	x1 = v[0].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[0]+ex;

	ex = xdelta2*ey;
	x2 = v[0].x+ex;
	ex = zdelta2*ey;
	z2 = zdiv[0]+ex;

	ay = (int)v[0].y;
	by = (int)v[1].y;

	fxdelta1 = (int)(xdelta1*65536.f);
	fx1 = (int)(x1*65536.f);
	fxdelta2 = (int)(xdelta2*65536.f);
	fx2 = (int)(x2*65536.f);

	fzdelta1 = (int)(zdelta1*4294967296.f);
	fz1 = (int)(z1*4294967296.f);
	fzdelta2 = (int)(zdelta2*4294967296.f);
	fz2 = (int)(z2*4294967296.f);

	if((by-ay)>0){
		for(counter=(by-ay);counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			current_edge++;
			total_len++;
		}
	}

	xlen1 = v[2].x-v[1].x;
	zlen1 = zdiv[2]-zdiv[1];
	ymul1 = 1/(ylen2+1);
	xdelta1 = xlen1*ymul1;
	zdelta1 = zlen1*ymul1;
	ey = ((int)(v[1].y+1.f))-v[1].y;
	ex = xdelta1*ey;
	x1 = v[1].x+ex;

	ex = zdelta1*ey;
	z1 = zdiv[1]+ex;

	ay = (int)v[1].y;
	by = (int)v[2].y;

	if((by-ay)>0){
		fxdelta1 = (int)(xdelta1*65536.f);
		fx1 = (int)(x1*65536.f);
		fzdelta1 = (int)(zdelta1*4294967296.f);
		fz1 = (int)(z1*4294967296.f);
		for(counter=by-ay;counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			current_edge++;
			total_len++;
		}
	}

	current_edge = edges;
	y = (int)v[0].y;
	buffer += (int)v[0].y*WIDTH;
	zbuffer_shit += (int)v[0].y*WIDTH;
	for(;total_len;total_len--){
		flat_hline( buffer, zbuffer_shit, current_edge, y, color );
		buffer += WIDTH;
		zbuffer_shit += WIDTH;
		current_edge++;
		y++;
	}
}

__inline void perfect_texture_flat_hline( unsigned int* target, unsigned int* ztarget, poly_edge *edge, int y, unsigned int *texture, unsigned int shade ){
	unsigned int *bufferp;
	unsigned int *zbufferp;

	int x1 = edge->start;
	int x2 = edge->stop;
	int z1 = edge->zstart;
	int z2 = edge->zstop;
	int u1 = edge->ustart;
	int u2 = edge->ustop;
	int v1 = edge->vstart;
	int v2 = edge->vstop;
	int outer_delta_u;
	int outer_delta_v;
	int inner_delta_u;
	int inner_delta_v;
	int delta_z;
	int len;
	int segs;
	unsigned int z;
	int z_div;

	int next_outer_u;
	int next_outer_v;
	int next_outer_z;

	int next_inner_u;
	int next_inner_v;

	int outer_u;
	int outer_v;
	int inner_u;
	int inner_v;

	int cl, ch;

	if( x1==x2 ) return;
	if( x2 < x1 ){
		xchg_int( &x1, &x2 );
		xchg_int( &z1, &z2 );
		xchg_int( &u1, &u2 );
		xchg_int( &v1, &v2 );
	}

	delta_z = (z2-z1)/(x2-x1);
	outer_delta_u = (u2-u1)/(x2-x1);
	outer_delta_v = (v2-v1)/(x2-x1);
	outer_u = u1;
	outer_v = v1;

	bufferp = target+x1;
	zbufferp = ztarget+x1;

	len = x2-x1;
	segs = len>>3;
	cl = len-(segs<<3);

	_asm{
		pxor mm5,mm5
		movd mm1, shade
		punpcklbw mm1, mm5
	}

	if(z1 != 0)
 		z_div = 0xFFFFFF/(z1>>14);
	else
		z_div = 0;
	inner_u = (outer_u*z_div)>>6;
	inner_v = (outer_v*z_div)>>6;
	next_outer_u = u1+(outer_delta_u*cl);
	next_outer_v = v1+(outer_delta_v*cl);
	next_outer_z = z1+(delta_z*cl);

	z = z1;

	if((next_outer_z>>14)!=0) 
	 	z_div = 0xFFFFFF/(next_outer_z>>14);
	else
		z_div = 0;
	next_inner_u = (next_outer_u*z_div)>>6;
	next_inner_v = (next_outer_v*z_div)>>6;

	if(cl>0){
		inner_delta_u = (next_inner_u-inner_u)/cl;
		inner_delta_v = (next_inner_v-inner_v)/cl;
	}

	for( ch=1+segs; ch; ch-- ){
		for(;cl;cl--){
			if(*zbufferp<z){
				unsigned int color = texture[((inner_u>>16)&0xFF)|((inner_v>>8)&0xFF00)];
				_asm{
					movd mm0, color
					punpcklbw mm0, mm5
					pmullw mm0, mm1
					psrlw mm0, 8
					packuswb mm0, mm0
					movd color, mm0
				}
				*bufferp = color;
				*zbufferp = z;
			}
			z += delta_z;
			inner_u += inner_delta_u;
			inner_v += inner_delta_v;
			bufferp++;
			zbufferp++;
		}
		outer_u = next_outer_u;
		outer_v = next_outer_v;
		next_outer_u += outer_delta_u<<3;
		next_outer_v += outer_delta_v<<3;
		next_outer_z += delta_z<<3;

		if((next_outer_z>>14)!=0) 
	 		z_div = 0xFFFFFF/(next_outer_z>>14);
		else
			z_div = 0;
		next_inner_u = (next_outer_u*z_div)>>6;
		next_inner_v = (next_outer_v*z_div)>>6;
		inner_delta_u = (next_inner_u-inner_u)>>3;
		inner_delta_v = (next_inner_v-inner_v)>>3;

		cl = 8;
	}
}

void draw_perfect_texture_flat_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], unsigned int *texture, unsigned int shade ){
	poly_edge *current_edge = edges;
	unsigned int *zbuffer_shit = zbuffer;
	int total_len = 0;

	float x1, x2;
	float z1, z2;
	float u1, u2;
	float v1, v2;
	float xdelta1, xdelta2;
	float zdelta1, zdelta2;
	float udelta1, udelta2;
	float vdelta1, vdelta2;

	float ylen1, ylen2;
	float xlen1, xlen2;
	float zlen1, zlen2;
	float ulen1, ulen2;
	float vlen1, vlen2;
	float ex, ey;
	float zdiv[3];
	float udiv[3];
	float vdiv[3];

	float ymul1;
	float ymul2;

	int counter;
	int ay, by;

	int fxdelta1, fxdelta2;
	int fzdelta1, fzdelta2;
	int fudelta1, fudelta2;
	int fvdelta1, fvdelta2;
	int fx1, fx2;
	int fz1, fz2;
	int fu1, fu2;
	int fv1, fv2;
	int y;

	if(v[1].y > v[2].y){
		xchg_vertex2d(&v[1], &v[2]);
		xchg_texcoord(&uv[1], &uv[2]);
	}
	if(v[0].y > v[2].y){
		xchg_vertex2d(&v[0], &v[2]);
		xchg_texcoord(&uv[0], &uv[2]);
	}
	if(v[0].y > v[1].y){
		xchg_vertex2d(&v[0], &v[1]);
		xchg_texcoord(&uv[0], &uv[1]);
	}

	ylen1 = v[1].y-v[0].y;
	ylen2 = v[2].y-v[1].y;

	// kalkuler lengste scanline (den ved v[1]) osv,
	// for å slippe divisjoner i hline
	zdiv[0] = 1.0f/v[0].z;
	zdiv[1] = 1.0f/v[1].z;
	zdiv[2] = 1.0f/v[2].z;

	udiv[0] = uv[0].u*zdiv[0];
	udiv[1] = uv[1].u*zdiv[1];
	udiv[2] = uv[2].u*zdiv[2];

	vdiv[0] = uv[0].v*zdiv[0];
	vdiv[1] = uv[1].v*zdiv[1];
	vdiv[2] = uv[2].v*zdiv[2];

	xlen1 = v[1].x-v[0].x;
	xlen2 = v[2].x-v[0].x;
	zlen1 = zdiv[1]-zdiv[0];
	zlen2 = zdiv[2]-zdiv[0];
	ulen1 = udiv[1]-udiv[0];
	ulen2 = udiv[2]-udiv[0];
	vlen1 = vdiv[1]-vdiv[0];
	vlen2 = vdiv[2]-vdiv[0];

	ymul1 = 1.f/(ylen1+1);
	ymul2 = 1.f/(ylen1+ylen2+1);

	xdelta1 = xlen1*ymul1;
	xdelta2 = xlen2*ymul2;
	zdelta1 = zlen1*ymul1;
	zdelta2 = zlen2*ymul2;
	udelta1 = ulen1*ymul1;
	udelta2 = ulen2*ymul2;
	vdelta1 = vlen1*ymul1;
	vdelta2 = vlen2*ymul2;

	ey = ((int)(v[0].y+1.f))-v[0].y;
	ex = xdelta1*ey;
	x1 = v[0].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[0]+ex;
	ex = udelta1*ey;
	u1 = udiv[0]+ex;
	ex = vdelta1*ey;
	v1 = vdiv[0]+ex;

	ex = xdelta2*ey;
	x2 = v[0].x+ex;
	ex = zdelta2*ey;
	z2 = zdiv[0]+ex;
	ex = udelta2*ey;
	u2 = udiv[0]+ex;
	ex = vdelta2*ey;
	v2 = vdiv[0]+ex;

	ay = (int)v[0].y;
	by = (int)v[1].y;

	fxdelta1 = (int)(xdelta1*65536.f);
	fx1 = (int)(x1*65536.f);
	fxdelta2 = (int)(xdelta2*65536.f);
	fx2 = (int)(x2*65536.f);

	fzdelta1 = (int)(zdelta1*4294967296.f);
	fz1 = (int)(z1*4294967296.f);
	fzdelta2 = (int)(zdelta2*4294967296.f);
	fz2 = (int)(z2*4294967296.f);

	fudelta1 = (int)(udelta1*65536.f);
	fu1 = (int)(u1*65536.f);
	fudelta2 = (int)(udelta2*65536.f);
	fu2 = (int)(u2*65536.f);

	fvdelta1 = (int)(vdelta1*65536.f);
	fv1 = (int)(v1*65536.f);
	fvdelta2 = (int)(vdelta2*65536.f);
	fv2 = (int)(v2*65536.f);


	if((by-ay)>0){
		for(counter=(by-ay);counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			current_edge++;
			total_len++;
		}
	}

	xlen1 = v[2].x-v[1].x;
	zlen1 = zdiv[2]-zdiv[1];
	ulen1 = udiv[2]-udiv[1];
	vlen1 = vdiv[2]-vdiv[1];

	ymul1 = 1/(ylen2+1);
	xdelta1 = xlen1*ymul1;
	zdelta1 = zlen1*ymul1;
	udelta1 = ulen1*ymul1;
	vdelta1 = vlen1*ymul1;

	ey = ((int)(v[1].y+1.f))-v[1].y;

	ex = xdelta1*ey;
	x1 = v[1].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[1]+ex;
	ex = udelta1*ey;
	u1 = udiv[1]+ex;
	ex = vdelta1*ey;
	v1 = vdiv[1]+ex;

	ay = (int)v[1].y;
	by = (int)v[2].y;

	if((by-ay)>0){
		fxdelta1 = (int)(xdelta1*65536.f);
		fx1 = (int)(x1*65536.f);
		fzdelta1 = (int)(zdelta1*4294967296.f);
		fz1 = (int)(z1*4294967296.f);
		fudelta1 = (int)(udelta1*65536.f);
		fu1 = (int)(u1*65536.f);
		fvdelta1 = (int)(vdelta1*65536.f);
		fv1 = (int)(v1*65536.f);
		for(counter=by-ay;counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			current_edge++;
			total_len++;
		}
	}

	current_edge = edges;
	y = (int)v[0].y;
	buffer += (int)v[0].y*WIDTH;
	zbuffer_shit += (int)v[0].y*WIDTH;
	for(;total_len;total_len--){
		perfect_texture_flat_hline( buffer, zbuffer_shit, current_edge, y, texture, shade );
		buffer += WIDTH;
		zbuffer_shit += WIDTH;
		current_edge++;
		y++;
	}
	_asm emms;
}

__inline void perfect_texture_hline( unsigned int* target, unsigned int* ztarget, poly_edge *edge, int y, unsigned int *texture ){
	unsigned int *bufferp;
	unsigned int *zbufferp;

	int x1 = edge->start;
	int x2 = edge->stop;
	int z1 = edge->zstart;
	int z2 = edge->zstop;
	int u1 = edge->ustart;
	int u2 = edge->ustop;
	int v1 = edge->vstart;
	int v2 = edge->vstop;
	int outer_delta_u;
	int outer_delta_v;
	int inner_delta_u;
	int inner_delta_v;
	int delta_z;
	int len;
	int segs;
	int z;

	int next_outer_u;
	int next_outer_v;
	int next_outer_z;

	int next_inner_u;
	int next_inner_v;

	int outer_u;
	int outer_v;
	int inner_u;
	int inner_v;

	int cl, ch;

	if( x1==x2 ) return;
	if( x2 < x1 ){
		xchg_int( &x1, &x2 );
		xchg_int( &z1, &z2 );
		xchg_int( &u1, &u2 );
		xchg_int( &v1, &v2 );
	}

	delta_z = (z2-z1)/(x2-x1);
	outer_delta_u = (u2-u1)/(x2-x1);
	outer_delta_v = (v2-v1)/(x2-x1);
	outer_u = u1;
	outer_v = v1;

	bufferp = target+x1;
	zbufferp = ztarget+x1;

	len = x2-x1;
	segs = len>>3;
	cl = len-(segs<<3);

	if(z1 != 0)
 		z = 0xFFFFFF/(z1>>14);
	else
		z = 0;
	inner_u = (outer_u*z)>>6;
	inner_v = (outer_v*z)>>6;
	next_outer_u = u1+(outer_delta_u*cl);
	next_outer_v = v1+(outer_delta_v*cl);
	next_outer_z = z1+(delta_z*cl);

	if((next_outer_z>>14)!=0) 
	 	z = 0xFFFFFF/(next_outer_z>>14);
	else
		z = 0;
	next_inner_u = (next_outer_u*z)>>6;
	next_inner_v = (next_outer_v*z)>>6;

	if(cl>0){
		inner_delta_u = (next_inner_u-inner_u)/cl;
		inner_delta_v = (next_inner_v-inner_v)/cl;
	}

	for( ch=1+segs; ch; ch-- ){
		for(;cl;cl--){
			if(*zbufferp<z1){
				*bufferp = texture[((inner_u>>16)&0xFF)|((inner_v>>8)&0xFF00)];
				*zbufferp = z1;
			}
			z1 += delta_z;
			inner_u += inner_delta_u;
			inner_v += inner_delta_v;
			bufferp++;
			zbufferp++;
		}
		outer_u = next_outer_u;
		outer_v = next_outer_v;
		next_outer_u += outer_delta_u<<3;
		next_outer_v += outer_delta_v<<3;
		next_outer_z += delta_z<<3;

		if((next_outer_z>>14)!=0) 
	 		z = 0xFFFFFF/(next_outer_z>>14);
		else
			z = 0;
		next_inner_u = (next_outer_u*z)>>6;
		next_inner_v = (next_outer_v*z)>>6;
		inner_delta_u = (next_inner_u-inner_u)>>3;
		inner_delta_v = (next_inner_v-inner_v)>>3;

		cl = 8;
	}
}

void draw_perfect_texture_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], unsigned int *texture ){
	poly_edge *current_edge = edges;
	unsigned int *zbuffer_shit = zbuffer;
	int total_len = 0;

	float x1, x2;
	float z1, z2;
	float u1, u2;
	float v1, v2;
	float xdelta1, xdelta2;
	float zdelta1, zdelta2;
	float udelta1, udelta2;
	float vdelta1, vdelta2;

	float ylen1, ylen2;
	float xlen1, xlen2;
	float zlen1, zlen2;
	float ulen1, ulen2;
	float vlen1, vlen2;
	float ex, ey;
	float zdiv[3];
	float udiv[3];
	float vdiv[3];

	float ymul1;
	float ymul2;

	int counter;
	int ay, by;

	int fxdelta1, fxdelta2;
	int fzdelta1, fzdelta2;
	int fudelta1, fudelta2;
	int fvdelta1, fvdelta2;
	int fx1, fx2;
	int fz1, fz2;
	int fu1, fu2;
	int fv1, fv2;
	int y;

	if(v[1].y > v[2].y){
		xchg_vertex2d(&v[1], &v[2]);
		xchg_texcoord(&uv[1], &uv[2]);
	}
	if(v[0].y > v[2].y){
		xchg_vertex2d(&v[0], &v[2]);
		xchg_texcoord(&uv[0], &uv[2]);
	}
	if(v[0].y > v[1].y){
		xchg_vertex2d(&v[0], &v[1]);
		xchg_texcoord(&uv[0], &uv[1]);
	}

	ylen1 = v[1].y-v[0].y;
	ylen2 = v[2].y-v[1].y;

	// kalkuler lengste scanline (den ved v[1]) osv,
	// for å slippe divisjoner i hline
	zdiv[0] = 1.0f/v[0].z;
	zdiv[1] = 1.0f/v[1].z;
	zdiv[2] = 1.0f/v[2].z;

	udiv[0] = uv[0].u*zdiv[0];
	udiv[1] = uv[1].u*zdiv[1];
	udiv[2] = uv[2].u*zdiv[2];

	vdiv[0] = uv[0].v*zdiv[0];
	vdiv[1] = uv[1].v*zdiv[1];
	vdiv[2] = uv[2].v*zdiv[2];

	xlen1 = v[1].x-v[0].x;
	xlen2 = v[2].x-v[0].x;
	zlen1 = zdiv[1]-zdiv[0];
	zlen2 = zdiv[2]-zdiv[0];
	ulen1 = udiv[1]-udiv[0];
	ulen2 = udiv[2]-udiv[0];
	vlen1 = vdiv[1]-vdiv[0];
	vlen2 = vdiv[2]-vdiv[0];

	ymul1 = 1.f/(ylen1+1);
	ymul2 = 1.f/(ylen1+ylen2+1);

	xdelta1 = xlen1*ymul1;
	xdelta2 = xlen2*ymul2;
	zdelta1 = zlen1*ymul1;
	zdelta2 = zlen2*ymul2;
	udelta1 = ulen1*ymul1;
	udelta2 = ulen2*ymul2;
	vdelta1 = vlen1*ymul1;
	vdelta2 = vlen2*ymul2;

	ey = ((int)(v[0].y+1.f))-v[0].y;
	ex = xdelta1*ey;
	x1 = v[0].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[0]+ex;
	ex = udelta1*ey;
	u1 = udiv[0]+ex;
	ex = vdelta1*ey;
	v1 = vdiv[0]+ex;

	ex = xdelta2*ey;
	x2 = v[0].x+ex;
	ex = zdelta2*ey;
	z2 = zdiv[0]+ex;
	ex = udelta2*ey;
	u2 = udiv[0]+ex;
	ex = vdelta2*ey;
	v2 = vdiv[0]+ex;

	ay = (int)v[0].y;
	by = (int)v[1].y;

	fxdelta1 = (int)(xdelta1*65536.f);
	fx1 = (int)(x1*65536.f);
	fxdelta2 = (int)(xdelta2*65536.f);
	fx2 = (int)(x2*65536.f);

	fzdelta1 = (int)(zdelta1*4294967296.f);
	fz1 = (int)(z1*4294967296.f);
	fzdelta2 = (int)(zdelta2*4294967296.f);
	fz2 = (int)(z2*4294967296.f);

	fudelta1 = (int)(udelta1*65536.f);
	fu1 = (int)(u1*65536.f);
	fudelta2 = (int)(udelta2*65536.f);
	fu2 = (int)(u2*65536.f);

	fvdelta1 = (int)(vdelta1*65536.f);
	fv1 = (int)(v1*65536.f);
	fvdelta2 = (int)(vdelta2*65536.f);
	fv2 = (int)(v2*65536.f);


	if((by-ay)>0){
		for(counter=(by-ay);counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			current_edge++;
			total_len++;
		}
	}

	xlen1 = v[2].x-v[1].x;
	zlen1 = zdiv[2]-zdiv[1];
	ulen1 = udiv[2]-udiv[1];
	vlen1 = vdiv[2]-vdiv[1];

	ymul1 = 1/(ylen2+1);
	xdelta1 = xlen1*ymul1;
	zdelta1 = zlen1*ymul1;
	udelta1 = ulen1*ymul1;
	vdelta1 = vlen1*ymul1;

	ey = ((int)(v[1].y+1.f))-v[1].y;

	ex = xdelta1*ey;
	x1 = v[1].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[1]+ex;
	ex = udelta1*ey;
	u1 = udiv[1]+ex;
	ex = vdelta1*ey;
	v1 = vdiv[1]+ex;

	ay = (int)v[1].y;
	by = (int)v[2].y;

	if((by-ay)>0){
		fxdelta1 = (int)(xdelta1*65536.f);
		fx1 = (int)(x1*65536.f);
		fzdelta1 = (int)(zdelta1*4294967296.f);
		fz1 = (int)(z1*4294967296.f);
		fudelta1 = (int)(udelta1*65536.f);
		fu1 = (int)(u1*65536.f);
		fvdelta1 = (int)(vdelta1*65536.f);
		fv1 = (int)(v1*65536.f);
		for(counter=by-ay;counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			current_edge++;
			total_len++;
		}
	}

	current_edge = edges;
	y = (int)v[0].y;
	buffer += (int)v[0].y*WIDTH;
	zbuffer_shit += (int)v[0].y*WIDTH;
	for(;total_len;total_len--){
		perfect_texture_hline( buffer, zbuffer_shit, current_edge, y, texture );
		buffer += WIDTH;
		zbuffer_shit += WIDTH;
		current_edge++;
		y++;
	}
}

__inline void perfect_multi_texture_hline( unsigned int* target, unsigned int* ztarget, poly_edge *edge, int y, unsigned int *texture1, unsigned int *texture2 ){
	unsigned int *bufferp;
	unsigned int *zbufferp;

	int x1 = edge->start;
	int x2 = edge->stop;
	int z1 = edge->zstart;
	int z2 = edge->zstop;
	int u1 = edge->ustart;
	int u2 = edge->ustop;
	int v1 = edge->vstart;
	int v2 = edge->vstop;
	int eu1 = edge->u2start;
	int eu2 = edge->u2stop;
	int ev1 = edge->v2start;
	int ev2 = edge->v2stop;
	int outer_delta_u;
	int outer_delta_v;
	int inner_delta_u;
	int inner_delta_v;
	int edelta_u;
	int edelta_v;

	int delta_z;
	int len;
	int segs;
	int z;
	int eu, ev;

	int next_outer_u;
	int next_outer_v;
	int next_outer_z;

	int next_inner_u;
	int next_inner_v;

	int outer_u;
	int outer_v;
	int inner_u;
	int inner_v;

	int cl, ch;

	if( x1==x2 ) return;
	if( x2 < x1 ){
		xchg_int( &x1, &x2 );
		xchg_int( &z1, &z2 );
		xchg_int( &u1, &u2 );
		xchg_int( &v1, &v2 );
		xchg_int( &eu1, &eu2 );
		xchg_int( &ev1, &ev2 );
	}

	delta_z = (z2-z1)/(x2-x1);
	outer_delta_u = (u2-u1)/(x2-x1);
	outer_delta_v = (v2-v1)/(x2-x1);
	outer_u = u1;
	outer_v = v1;

	edelta_u = (eu2-eu1)/(x2-x1);
	edelta_v = (ev2-ev1)/(x2-x1);

	eu = eu1;
	ev = ev1;

	bufferp = target+x1;
	zbufferp = ztarget+x1;

	len = x2-x1;
	segs = len>>3;
	cl = len-(segs<<3);

	if(z1 != 0)
 		z = 0xFFFFFF/(z1>>14);
	else
		z = 0;
	inner_u = (outer_u*z)>>6;
	inner_v = (outer_v*z)>>6;
	next_outer_u = u1+(outer_delta_u*cl);
	next_outer_v = v1+(outer_delta_v*cl);
	next_outer_z = z1+(delta_z*cl);

	if((next_outer_z>>14)!=0) 
	 	z = 0xFFFFFF/(next_outer_z>>14);
	else
		z = 0;
	next_inner_u = (next_outer_u*z)>>6;
	next_inner_v = (next_outer_v*z)>>6;

	if(cl>0){
		inner_delta_u = (next_inner_u-inner_u)/cl;
		inner_delta_v = (next_inner_v-inner_v)/cl;
	}

	for( ch=1+segs; ch; ch-- ){
		for(;cl;cl--){
			if(*zbufferp<z1){
				unsigned int color1 = texture1[((inner_u>>16)&0xFF)|((inner_v>>8)&0xFF00)];
				unsigned int color2 = texture2[((eu>>12)&0xFF)|((ev>>4)&0xFF00)];

				_asm{
					movd mm0, color1
					movd mm1, color2
					paddusb mm0, mm1
					movd color1, mm0
				}

				*bufferp = color1;
				*zbufferp = z1;
			}
			z1 += delta_z;
			inner_u += inner_delta_u;
			inner_v += inner_delta_v;
			eu += edelta_u;
			ev += edelta_v;
			bufferp++;
			zbufferp++;
		}
		outer_u = next_outer_u;
		outer_v = next_outer_v;
		next_outer_u += outer_delta_u<<3;
		next_outer_v += outer_delta_v<<3;
		next_outer_z += delta_z<<3;

		if((next_outer_z>>14)!=0) 
	 		z = 0xFFFFFF/(next_outer_z>>14);
		else
			z = 0;
		next_inner_u = (next_outer_u*z)>>6;
		next_inner_v = (next_outer_v*z)>>6;
		inner_delta_u = (next_inner_u-inner_u)>>3;
		inner_delta_v = (next_inner_v-inner_v)>>3;

		cl = 8;
	}
}


void draw_perfect_multi_texture_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], texcoord euv[3], unsigned int *texture1, unsigned int *texture2 ){
	poly_edge *current_edge = edges;
	unsigned int *zbuffer_shit = zbuffer;
	int total_len = 0;

	float x1, x2;
	float z1, z2;
	float u1, u2;
	float v1, v2;
	float eu1, eu2;
	float ev1, ev2;
	float xdelta1, xdelta2;
	float zdelta1, zdelta2;
	float udelta1, udelta2;
	float vdelta1, vdelta2;
	float eudelta1, eudelta2;
	float evdelta1, evdelta2;

	float ylen1, ylen2;
	float xlen1, xlen2;
	float zlen1, zlen2;
	float ulen1, ulen2;
	float vlen1, vlen2;
	float eulen1, eulen2;
	float evlen1, evlen2;
	float ex, ey;
	float zdiv[3];
	float udiv[3];
	float vdiv[3];
	float eudiv[3];
	float evdiv[3];

	float ymul1;
	float ymul2;

	int counter;
	int ay, by;

	int fxdelta1, fxdelta2;
	int fzdelta1, fzdelta2;
	int fudelta1, fudelta2;
	int fvdelta1, fvdelta2;
	int feudelta1, feudelta2;
	int fevdelta1, fevdelta2;
	int fx1, fx2;
	int fz1, fz2;
	int fu1, fu2;
	int fv1, fv2;
	int feu1, feu2;
	int fev1, fev2;
	int y;

	if(v[1].y > v[2].y){
		xchg_vertex2d(&v[1], &v[2]);
		xchg_texcoord(&uv[1], &uv[2]);
		xchg_texcoord(&euv[1], &euv[2]);
	}
	if(v[0].y > v[2].y){
		xchg_vertex2d(&v[0], &v[2]);
		xchg_texcoord(&uv[0], &uv[2]);
		xchg_texcoord(&euv[0], &euv[2]);
	}
	if(v[0].y > v[1].y){
		xchg_vertex2d(&v[0], &v[1]);
		xchg_texcoord(&uv[0], &uv[1]);
		xchg_texcoord(&euv[0], &euv[1]);
	}

	ylen1 = v[1].y-v[0].y;
	ylen2 = v[2].y-v[1].y;

	// kalkuler lengste scanline (den ved v[1]) osv,
	// for å slippe divisjoner i hline
	zdiv[0] = 1.0f/v[0].z;
	zdiv[1] = 1.0f/v[1].z;
	zdiv[2] = 1.0f/v[2].z;

	udiv[0] = uv[0].u*zdiv[0];
	udiv[1] = uv[1].u*zdiv[1];
	udiv[2] = uv[2].u*zdiv[2];

	vdiv[0] = uv[0].v*zdiv[0];
	vdiv[1] = uv[1].v*zdiv[1];
	vdiv[2] = uv[2].v*zdiv[2];

	eudiv[0] = euv[0].u*zdiv[0];
	eudiv[1] = euv[1].u*zdiv[1];
	eudiv[2] = euv[2].u*zdiv[2];

	evdiv[0] = euv[0].v*zdiv[0];
	evdiv[1] = euv[1].v*zdiv[1];
	evdiv[2] = euv[2].v*zdiv[2];

	xlen1 = v[1].x-v[0].x;
	xlen2 = v[2].x-v[0].x;
	zlen1 = zdiv[1]-zdiv[0];
	zlen2 = zdiv[2]-zdiv[0];
	ulen1 = udiv[1]-udiv[0];
	ulen2 = udiv[2]-udiv[0];
	vlen1 = vdiv[1]-vdiv[0];
	vlen2 = vdiv[2]-vdiv[0];
	eulen1 = eudiv[1]-eudiv[0];
	eulen2 = eudiv[2]-eudiv[0];
	evlen1 = evdiv[1]-evdiv[0];
	evlen2 = evdiv[2]-evdiv[0];

	ymul1 = 1.f/(ylen1+1);
	ymul2 = 1.f/(ylen1+ylen2+1);

	xdelta1 = xlen1*ymul1;
	xdelta2 = xlen2*ymul2;
	zdelta1 = zlen1*ymul1;
	zdelta2 = zlen2*ymul2;
	udelta1 = ulen1*ymul1;
	udelta2 = ulen2*ymul2;
	vdelta1 = vlen1*ymul1;
	vdelta2 = vlen2*ymul2;
	eudelta1 = eulen1*ymul1;
	eudelta2 = eulen2*ymul2;
	evdelta1 = evlen1*ymul1;
	evdelta2 = evlen2*ymul2;

	ey = ((int)(v[0].y+1.f))-v[0].y;
	ex = xdelta1*ey;
	x1 = v[0].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[0]+ex;
	ex = udelta1*ey;
	u1 = udiv[0]+ex;
	ex = vdelta1*ey;
	v1 = vdiv[0]+ex;
	ex = eudelta1*ey;
	eu1 = eudiv[0]+ex;
	ex = evdelta1*ey;
	ev1 = evdiv[0]+ex;

	ex = xdelta2*ey;
	x2 = v[0].x+ex;
	ex = zdelta2*ey;
	z2 = zdiv[0]+ex;
	ex = udelta2*ey;
	u2 = udiv[0]+ex;
	ex = vdelta2*ey;
	v2 = vdiv[0]+ex;
	ex = eudelta2*ey;
	eu2 = eudiv[0]+ex;
	ex = evdelta2*ey;
	ev2 = evdiv[0]+ex;

	ay = (int)v[0].y;
	by = (int)v[1].y;

	fxdelta1 = (int)(xdelta1*65536.f);
	fx1 = (int)(x1*65536.f);
	fxdelta2 = (int)(xdelta2*65536.f);
	fx2 = (int)(x2*65536.f);

	fzdelta1 = (int)(zdelta1*4294967296.f);
	fz1 = (int)(z1*4294967296.f);
	fzdelta2 = (int)(zdelta2*4294967296.f);
	fz2 = (int)(z2*4294967296.f);

	fudelta1 = (int)(udelta1*65536.f);
	fu1 = (int)(u1*65536.f);
	fudelta2 = (int)(udelta2*65536.f);
	fu2 = (int)(u2*65536.f);
	feudelta1 = (int)(eudelta1*65536.f);
	feu1 = (int)(eu1*65536.f);
	feudelta2 = (int)(eudelta2*65536.f);
	feu2 = (int)(eu2*65536.f);

	fvdelta1 = (int)(vdelta1*65536.f);
	fv1 = (int)(v1*65536.f);
	fvdelta2 = (int)(vdelta2*65536.f);
	fv2 = (int)(v2*65536.f);
	fevdelta1 = (int)(evdelta1*65536.f);
	fev1 = (int)(ev1*65536.f);
	fevdelta2 = (int)(evdelta2*65536.f);
	fev2 = (int)(ev2*65536.f);


	if((by-ay)>0){
		for(counter=(by-ay);counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			current_edge->u2start = feu1;
			current_edge->u2stop = feu2;
			current_edge->v2start = fev1;
			current_edge->v2stop = fev2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			feu1 += feudelta1;
			feu2 += feudelta2;
			fev1 += fevdelta1;
			fev2 += fevdelta2;
			current_edge++;
			total_len++;
		}
	}

	xlen1 = v[2].x-v[1].x;
	zlen1 = zdiv[2]-zdiv[1];
	ulen1 = udiv[2]-udiv[1];
	vlen1 = vdiv[2]-vdiv[1];
	eulen1 = eudiv[2]-eudiv[1];
	evlen1 = evdiv[2]-evdiv[1];

	ymul1 = 1/(ylen2+1);
	xdelta1 = xlen1*ymul1;
	zdelta1 = zlen1*ymul1;
	udelta1 = ulen1*ymul1;
	vdelta1 = vlen1*ymul1;
	eudelta1 = eulen1*ymul1;
	evdelta1 = evlen1*ymul1;

	ey = ((int)(v[1].y+1.f))-v[1].y;

	ex = xdelta1*ey;
	x1 = v[1].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[1]+ex;
	ex = udelta1*ey;
	u1 = udiv[1]+ex;
	ex = vdelta1*ey;
	v1 = vdiv[1]+ex;
	ex = eudelta1*ey;
	eu1 = eudiv[1]+ex;
	ex = evdelta1*ey;
	ev1 = evdiv[1]+ex;

	ay = (int)v[1].y;
	by = (int)v[2].y;

	if((by-ay)>0){
		fxdelta1 = (int)(xdelta1*65536.f);
		fx1 = (int)(x1*65536.f);
		fzdelta1 = (int)(zdelta1*4294967296.f);
		fz1 = (int)(z1*4294967296.f);
		fudelta1 = (int)(udelta1*65536.f);
		fu1 = (int)(u1*65536.f);
		fvdelta1 = (int)(vdelta1*65536.f);
		fv1 = (int)(v1*65536.f);
		feudelta1 = (int)(eudelta1*65536.f);
		feu1 = (int)(eu1*65536.f);
		fevdelta1 = (int)(evdelta1*65536.f);
		fev1 = (int)(ev1*65536.f);
		for(counter=by-ay;counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			current_edge->u2start = feu1;
			current_edge->u2stop = feu2;
			current_edge->v2start = fev1;
			current_edge->v2stop = fev2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			feu1 += feudelta1;
			feu2 += feudelta2;
			fev1 += fevdelta1;
			fev2 += fevdelta2;
			current_edge++;
			total_len++;
		}
	}

	current_edge = edges;
	y = (int)v[0].y;
	buffer += (int)v[0].y*WIDTH;
	zbuffer_shit += (int)v[0].y*WIDTH;

	_asm pxor mm5, mm5

	for(;total_len;total_len--){
		perfect_multi_texture_hline( buffer, zbuffer_shit, current_edge, y, texture1, texture2 );
		buffer += WIDTH;
		zbuffer_shit += WIDTH;
		current_edge++;
		y++;
	}

	_asm emms;
}

__inline void perfect_bump_texture_hline( unsigned int* target, unsigned int* ztarget, poly_edge *edge, int y, unsigned int *texture1, unsigned int *texture2 ){
	unsigned int *bufferp;
	unsigned int *zbufferp;

	int x1 = edge->start;
	int x2 = edge->stop;
	int z1 = edge->zstart;
	int z2 = edge->zstop;
	int u1 = edge->ustart;
	int u2 = edge->ustop;
	int v1 = edge->vstart;
	int v2 = edge->vstop;
	int eu1 = edge->u2start;
	int eu2 = edge->u2stop;
	int ev1 = edge->v2start;
	int ev2 = edge->v2stop;
	int outer_delta_u;
	int outer_delta_v;
	int inner_delta_u;
	int inner_delta_v;
	int edelta_u;
	int edelta_v;

	int delta_z;
	int len;
	int segs;
	int z;
	int eu, ev, bx, by;

	int next_outer_u;
	int next_outer_v;
	int next_outer_z;

	int next_inner_u;
	int next_inner_v;

	int outer_u;
	int outer_v;
	int inner_u;
	int inner_v;

	int cl, ch;

	if( x1==x2 ) return;
	if( x2 < x1 ){
		xchg_int( &x1, &x2 );
		xchg_int( &z1, &z2 );
		xchg_int( &u1, &u2 );
		xchg_int( &v1, &v2 );
		xchg_int( &eu1, &eu2 );
		xchg_int( &ev1, &ev2 );
	}

	delta_z = (z2-z1)/(x2-x1);
	outer_delta_u = (u2-u1)/(x2-x1);
	outer_delta_v = (v2-v1)/(x2-x1);
	outer_u = u1;
	outer_v = v1;

	edelta_u = (eu2-eu1)/(x2-x1);
	edelta_v = (ev2-ev1)/(x2-x1);

	eu = eu1;
	ev = ev1;

	bufferp = target+x1;
	zbufferp = ztarget+x1;

	len = x2-x1;
	segs = len>>3;
	cl = len-(segs<<3);

	if(z1 != 0)
 		z = 0xFFFFFF/(z1>>14);
	else
		z = 0;
	inner_u = (outer_u*z)>>6;
	inner_v = (outer_v*z)>>6;
	next_outer_u = u1+(outer_delta_u*cl);
	next_outer_v = v1+(outer_delta_v*cl);
	next_outer_z = z1+(delta_z*cl);

	if((next_outer_z>>14)!=0) 
	 	z = 0xFFFFFF/(next_outer_z>>14);
	else
		z = 0;
	next_inner_u = (next_outer_u*z)>>6;
	next_inner_v = (next_outer_v*z)>>6;

	if(cl>0){
		inner_delta_u = (next_inner_u-inner_u)/cl;
		inner_delta_v = (next_inner_v-inner_v)/cl;
	}

	for( ch=1+segs; ch; ch-- ){
		for(;cl;cl--){
			if(*zbufferp<z1){
				unsigned int color2;
				unsigned int color1 = texture1[((inner_u>>16)&0xFF)|((inner_v>>8)&0xFF00)];

				bx = (texture1[(((inner_u>>16)+1)&0xFF)|((inner_v>>8)&0xFF00)]>>24) - (texture1[(((inner_u>>16)-1)&0xFF)|((inner_v>>8)&0xFF00)]>>24);
                by = (texture1[((inner_u>>16)&0xFF)|(((inner_v>>8)+1)&0xFF00)]>>24) - (texture1[((inner_u>>16)&0xFF)|(((inner_v>>8)-1)&0xFF00)]>>24);

				bx += (eu>>12);
				by += (ev>>12);

				color2 = texture2[(bx&0xFF)|((by<<8)&0xFF00)];

				_asm{
					movd mm0, color1
					movd mm1, color2
					punpcklbw mm0, mm5
					punpcklbw mm1, mm5
					pmullw mm0, mm1
					psrlw mm0, 8
					packuswb mm0, mm0
					movd color1, mm0
				}
				*bufferp = color1;
				*zbufferp = z1;
			}
			z1 += delta_z;
			inner_u += inner_delta_u;
			inner_v += inner_delta_v;
			eu += edelta_u;
			ev += edelta_v;
			bufferp++;
			zbufferp++;
		}
		outer_u = next_outer_u;
		outer_v = next_outer_v;
		next_outer_u += outer_delta_u<<3;
		next_outer_v += outer_delta_v<<3;
		next_outer_z += delta_z<<3;

		if((next_outer_z>>14)!=0) 
	 		z = 0xFFFFFF/(next_outer_z>>14);
		else
			z = 0;
		next_inner_u = (next_outer_u*z)>>6;
		next_inner_v = (next_outer_v*z)>>6;
		inner_delta_u = (next_inner_u-inner_u)>>3;
		inner_delta_v = (next_inner_v-inner_v)>>3;

		cl = 8;
	}
}

void draw_perfect_bump_texture_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], texcoord euv[3], unsigned int *texture1, unsigned int *texture2 ){
	poly_edge *current_edge = edges;
	unsigned int *zbuffer_shit = zbuffer;
	int total_len = 0;

	float x1, x2;
	float z1, z2;
	float u1, u2;
	float v1, v2;
	float eu1, eu2;
	float ev1, ev2;
	float xdelta1, xdelta2;
	float zdelta1, zdelta2;
	float udelta1, udelta2;
	float vdelta1, vdelta2;
	float eudelta1, eudelta2;
	float evdelta1, evdelta2;

	float ylen1, ylen2;
	float xlen1, xlen2;
	float zlen1, zlen2;
	float ulen1, ulen2;
	float vlen1, vlen2;
	float eulen1, eulen2;
	float evlen1, evlen2;
	float ex, ey;
	float zdiv[3];
	float udiv[3];
	float vdiv[3];
	float eudiv[3];
	float evdiv[3];

	float ymul1;
	float ymul2;

	int counter;
	int ay, by;

	int fxdelta1, fxdelta2;
	int fzdelta1, fzdelta2;
	int fudelta1, fudelta2;
	int fvdelta1, fvdelta2;
	int feudelta1, feudelta2;
	int fevdelta1, fevdelta2;
	int fx1, fx2;
	int fz1, fz2;
	int fu1, fu2;
	int fv1, fv2;
	int feu1, feu2;
	int fev1, fev2;
	int y;

	if(v[1].y > v[2].y){
		xchg_vertex2d(&v[1], &v[2]);
		xchg_texcoord(&uv[1], &uv[2]);
		xchg_texcoord(&euv[1], &euv[2]);
	}
	if(v[0].y > v[2].y){
		xchg_vertex2d(&v[0], &v[2]);
		xchg_texcoord(&uv[0], &uv[2]);
		xchg_texcoord(&euv[0], &euv[2]);
	}
	if(v[0].y > v[1].y){
		xchg_vertex2d(&v[0], &v[1]);
		xchg_texcoord(&uv[0], &uv[1]);
		xchg_texcoord(&euv[0], &euv[1]);
	}

	ylen1 = v[1].y-v[0].y;
	ylen2 = v[2].y-v[1].y;

	// kalkuler lengste scanline (den ved v[1]) osv,
	// for å slippe divisjoner i hline
	zdiv[0] = 1.0f/v[0].z;
	zdiv[1] = 1.0f/v[1].z;
	zdiv[2] = 1.0f/v[2].z;

	udiv[0] = uv[0].u*zdiv[0];
	udiv[1] = uv[1].u*zdiv[1];
	udiv[2] = uv[2].u*zdiv[2];

	vdiv[0] = uv[0].v*zdiv[0];
	vdiv[1] = uv[1].v*zdiv[1];
	vdiv[2] = uv[2].v*zdiv[2];

	eudiv[0] = euv[0].u*zdiv[0];
	eudiv[1] = euv[1].u*zdiv[1];
	eudiv[2] = euv[2].u*zdiv[2];

	evdiv[0] = euv[0].v*zdiv[0];
	evdiv[1] = euv[1].v*zdiv[1];
	evdiv[2] = euv[2].v*zdiv[2];

	xlen1 = v[1].x-v[0].x;
	xlen2 = v[2].x-v[0].x;
	zlen1 = zdiv[1]-zdiv[0];
	zlen2 = zdiv[2]-zdiv[0];
	ulen1 = udiv[1]-udiv[0];
	ulen2 = udiv[2]-udiv[0];
	vlen1 = vdiv[1]-vdiv[0];
	vlen2 = vdiv[2]-vdiv[0];
	eulen1 = eudiv[1]-eudiv[0];
	eulen2 = eudiv[2]-eudiv[0];
	evlen1 = evdiv[1]-evdiv[0];
	evlen2 = evdiv[2]-evdiv[0];

	ymul1 = 1.f/(ylen1+1);
	ymul2 = 1.f/(ylen1+ylen2+1);

	xdelta1 = xlen1*ymul1;
	xdelta2 = xlen2*ymul2;
	zdelta1 = zlen1*ymul1;
	zdelta2 = zlen2*ymul2;
	udelta1 = ulen1*ymul1;
	udelta2 = ulen2*ymul2;
	vdelta1 = vlen1*ymul1;
	vdelta2 = vlen2*ymul2;
	eudelta1 = eulen1*ymul1;
	eudelta2 = eulen2*ymul2;
	evdelta1 = evlen1*ymul1;
	evdelta2 = evlen2*ymul2;

	ey = ((int)(v[0].y+1.f))-v[0].y;
	ex = xdelta1*ey;
	x1 = v[0].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[0]+ex;
	ex = udelta1*ey;
	u1 = udiv[0]+ex;
	ex = vdelta1*ey;
	v1 = vdiv[0]+ex;
	ex = eudelta1*ey;
	eu1 = eudiv[0]+ex;
	ex = evdelta1*ey;
	ev1 = evdiv[0]+ex;

	ex = xdelta2*ey;
	x2 = v[0].x+ex;
	ex = zdelta2*ey;
	z2 = zdiv[0]+ex;
	ex = udelta2*ey;
	u2 = udiv[0]+ex;
	ex = vdelta2*ey;
	v2 = vdiv[0]+ex;
	ex = eudelta2*ey;
	eu2 = eudiv[0]+ex;
	ex = evdelta2*ey;
	ev2 = evdiv[0]+ex;

	ay = (int)v[0].y;
	by = (int)v[1].y;

	fxdelta1 = (int)(xdelta1*65536.f);
	fx1 = (int)(x1*65536.f);
	fxdelta2 = (int)(xdelta2*65536.f);
	fx2 = (int)(x2*65536.f);

	fzdelta1 = (int)(zdelta1*4294967296.f);
	fz1 = (int)(z1*4294967296.f);
	fzdelta2 = (int)(zdelta2*4294967296.f);
	fz2 = (int)(z2*4294967296.f);

	fudelta1 = (int)(udelta1*65536.f);
	fu1 = (int)(u1*65536.f);
	fudelta2 = (int)(udelta2*65536.f);
	fu2 = (int)(u2*65536.f);
	feudelta1 = (int)(eudelta1*65536.f);
	feu1 = (int)(eu1*65536.f);
	feudelta2 = (int)(eudelta2*65536.f);
	feu2 = (int)(eu2*65536.f);

	fvdelta1 = (int)(vdelta1*65536.f);
	fv1 = (int)(v1*65536.f);
	fvdelta2 = (int)(vdelta2*65536.f);
	fv2 = (int)(v2*65536.f);
	fevdelta1 = (int)(evdelta1*65536.f);
	fev1 = (int)(ev1*65536.f);
	fevdelta2 = (int)(evdelta2*65536.f);
	fev2 = (int)(ev2*65536.f);


	if((by-ay)>0){
		for(counter=(by-ay);counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			current_edge->u2start = feu1;
			current_edge->u2stop = feu2;
			current_edge->v2start = fev1;
			current_edge->v2stop = fev2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			feu1 += feudelta1;
			feu2 += feudelta2;
			fev1 += fevdelta1;
			fev2 += fevdelta2;
			current_edge++;
			total_len++;
		}
	}

	xlen1 = v[2].x-v[1].x;
	zlen1 = zdiv[2]-zdiv[1];
	ulen1 = udiv[2]-udiv[1];
	vlen1 = vdiv[2]-vdiv[1];
	eulen1 = eudiv[2]-eudiv[1];
	evlen1 = evdiv[2]-evdiv[1];

	ymul1 = 1/(ylen2+1);
	xdelta1 = xlen1*ymul1;
	zdelta1 = zlen1*ymul1;
	udelta1 = ulen1*ymul1;
	vdelta1 = vlen1*ymul1;
	eudelta1 = eulen1*ymul1;
	evdelta1 = evlen1*ymul1;

	ey = ((int)(v[1].y+1.f))-v[1].y;

	ex = xdelta1*ey;
	x1 = v[1].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[1]+ex;
	ex = udelta1*ey;
	u1 = udiv[1]+ex;
	ex = vdelta1*ey;
	v1 = vdiv[1]+ex;
	ex = eudelta1*ey;
	eu1 = eudiv[1]+ex;
	ex = evdelta1*ey;
	ev1 = evdiv[1]+ex;

	ay = (int)v[1].y;
	by = (int)v[2].y;

	if((by-ay)>0){
		fxdelta1 = (int)(xdelta1*65536.f);
		fx1 = (int)(x1*65536.f);
		fzdelta1 = (int)(zdelta1*4294967296.f);
		fz1 = (int)(z1*4294967296.f);
		fudelta1 = (int)(udelta1*65536.f);
		fu1 = (int)(u1*65536.f);
		fvdelta1 = (int)(vdelta1*65536.f);
		fv1 = (int)(v1*65536.f);
		feudelta1 = (int)(eudelta1*65536.f);
		feu1 = (int)(eu1*65536.f);
		fevdelta1 = (int)(evdelta1*65536.f);
		fev1 = (int)(ev1*65536.f);
		for(counter=by-ay;counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			current_edge->u2start = feu1;
			current_edge->u2stop = feu2;
			current_edge->v2start = fev1;
			current_edge->v2stop = fev2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			feu1 += feudelta1;
			feu2 += feudelta2;
			fev1 += fevdelta1;
			fev2 += fevdelta2;
			current_edge++;
			total_len++;
		}
	}

	current_edge = edges;
	y = (int)v[0].y;
	buffer += (int)v[0].y*WIDTH;
	zbuffer_shit += (int)v[0].y*WIDTH;

	_asm pxor mm5, mm5

	for(;total_len;total_len--){
		perfect_bump_texture_hline( buffer, zbuffer_shit, current_edge, y, texture1, texture2 );
		buffer += WIDTH;
		zbuffer_shit += WIDTH;
		current_edge++;
		y++;
	}

	_asm emms;
}

__inline void perfect_envbump_texture_hline( unsigned int* target, unsigned int* ztarget, poly_edge *edge, int y, unsigned int *texture1, unsigned int *texture2 ){
	unsigned int *bufferp;
	unsigned int *zbufferp;

	int x1 = edge->start;
	int x2 = edge->stop;
	int z1 = edge->zstart;
	int z2 = edge->zstop;
	int u1 = edge->ustart;
	int u2 = edge->ustop;
	int v1 = edge->vstart;
	int v2 = edge->vstop;
	int eu1 = edge->u2start;
	int eu2 = edge->u2stop;
	int ev1 = edge->v2start;
	int ev2 = edge->v2stop;
	int outer_delta_u;
	int outer_delta_v;
	int inner_delta_u;
	int inner_delta_v;
	int edelta_u;
	int edelta_v;

	int delta_z;
	int len;
	int segs;
	int z;
	int eu, ev, bx, by;

	int next_outer_u;
	int next_outer_v;
	int next_outer_z;

	int next_inner_u;
	int next_inner_v;

	int outer_u;
	int outer_v;
	int inner_u;
	int inner_v;

	int cl, ch;

	if( x1==x2 ) return;
	if( x2 < x1 ){
		xchg_int( &x1, &x2 );
		xchg_int( &z1, &z2 );
		xchg_int( &u1, &u2 );
		xchg_int( &v1, &v2 );
		xchg_int( &eu1, &eu2 );
		xchg_int( &ev1, &ev2 );
	}

	delta_z = (z2-z1)/(x2-x1);
	outer_delta_u = (u2-u1)/(x2-x1);
	outer_delta_v = (v2-v1)/(x2-x1);
	outer_u = u1;
	outer_v = v1;

	edelta_u = (eu2-eu1)/(x2-x1);
	edelta_v = (ev2-ev1)/(x2-x1);

	eu = eu1;
	ev = ev1;

	bufferp = target+x1;
	zbufferp = ztarget+x1;

	len = x2-x1;
	segs = len>>3;
	cl = len-(segs<<3);

	if(z1 != 0)
 		z = 0xFFFFFF/(z1>>14);
	else
		z = 0;
	inner_u = (outer_u*z)>>6;
	inner_v = (outer_v*z)>>6;
	next_outer_u = u1+(outer_delta_u*cl);
	next_outer_v = v1+(outer_delta_v*cl);
	next_outer_z = z1+(delta_z*cl);

	if((next_outer_z>>14)!=0) 
	 	z = 0xFFFFFF/(next_outer_z>>14);
	else
		z = 0;
	next_inner_u = (next_outer_u*z)>>6;
	next_inner_v = (next_outer_v*z)>>6;

	if(cl>0){
		inner_delta_u = (next_inner_u-inner_u)/cl;
		inner_delta_v = (next_inner_v-inner_v)/cl;
	}

	for( ch=1+segs; ch; ch-- ){
		for(;cl;cl--){
			if(*zbufferp<z1){
				bx = (texture1[(((inner_u>>16)+1)&0xFF)|((inner_v>>8)&0xFF00)]>>24) - (texture1[(((inner_u>>16)-1)&0xFF)|((inner_v>>8)&0xFF00)]>>24);
                by = (texture1[((inner_u>>16)&0xFF)|(((inner_v>>8)+1)&0xFF00)]>>24) - (texture1[((inner_u>>16)&0xFF)|(((inner_v>>8)-1)&0xFF00)]>>24);
				bx += (eu>>12);
				by += (ev>>12);
				*bufferp = texture2[(bx&0xFF)|((by<<8)&0xFF00)];;
				*zbufferp = z1;
			}
			z1 += delta_z;
			inner_u += inner_delta_u;
			inner_v += inner_delta_v;
			eu += edelta_u;
			ev += edelta_v;
			bufferp++;
			zbufferp++;
		}
		outer_u = next_outer_u;
		outer_v = next_outer_v;
		next_outer_u += outer_delta_u<<3;
		next_outer_v += outer_delta_v<<3;
		next_outer_z += delta_z<<3;

		if((next_outer_z>>14)!=0) 
	 		z = 0xFFFFFF/(next_outer_z>>14);
		else
			z = 0;
		next_inner_u = (next_outer_u*z)>>6;
		next_inner_v = (next_outer_v*z)>>6;
		inner_delta_u = (next_inner_u-inner_u)>>3;
		inner_delta_v = (next_inner_v-inner_v)>>3;

		cl = 8;
	}
}

void draw_perfect_envbump_texture_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], texcoord euv[3], unsigned int *texture1, unsigned int *texture2 ){
	poly_edge *current_edge = edges;
	unsigned int *zbuffer_shit = zbuffer;
	int total_len = 0;

	float x1, x2;
	float z1, z2;
	float u1, u2;
	float v1, v2;
	float eu1, eu2;
	float ev1, ev2;
	float xdelta1, xdelta2;
	float zdelta1, zdelta2;
	float udelta1, udelta2;
	float vdelta1, vdelta2;
	float eudelta1, eudelta2;
	float evdelta1, evdelta2;

	float ylen1, ylen2;
	float xlen1, xlen2;
	float zlen1, zlen2;
	float ulen1, ulen2;
	float vlen1, vlen2;
	float eulen1, eulen2;
	float evlen1, evlen2;
	float ex, ey;
	float zdiv[3];
	float udiv[3];
	float vdiv[3];
	float eudiv[3];
	float evdiv[3];

	float ymul1;
	float ymul2;

	int counter;
	int ay, by;

	int fxdelta1, fxdelta2;
	int fzdelta1, fzdelta2;
	int fudelta1, fudelta2;
	int fvdelta1, fvdelta2;
	int feudelta1, feudelta2;
	int fevdelta1, fevdelta2;
	int fx1, fx2;
	int fz1, fz2;
	int fu1, fu2;
	int fv1, fv2;
	int feu1, feu2;
	int fev1, fev2;
	int y;

	if(v[1].y > v[2].y){
		xchg_vertex2d(&v[1], &v[2]);
		xchg_texcoord(&uv[1], &uv[2]);
		xchg_texcoord(&euv[1], &euv[2]);
	}
	if(v[0].y > v[2].y){
		xchg_vertex2d(&v[0], &v[2]);
		xchg_texcoord(&uv[0], &uv[2]);
		xchg_texcoord(&euv[0], &euv[2]);
	}
	if(v[0].y > v[1].y){
		xchg_vertex2d(&v[0], &v[1]);
		xchg_texcoord(&uv[0], &uv[1]);
		xchg_texcoord(&euv[0], &euv[1]);
	}

	ylen1 = v[1].y-v[0].y;
	ylen2 = v[2].y-v[1].y;

	// kalkuler lengste scanline (den ved v[1]) osv,
	// for å slippe divisjoner i hline
	zdiv[0] = 1.0f/v[0].z;
	zdiv[1] = 1.0f/v[1].z;
	zdiv[2] = 1.0f/v[2].z;

	udiv[0] = uv[0].u*zdiv[0];
	udiv[1] = uv[1].u*zdiv[1];
	udiv[2] = uv[2].u*zdiv[2];

	vdiv[0] = uv[0].v*zdiv[0];
	vdiv[1] = uv[1].v*zdiv[1];
	vdiv[2] = uv[2].v*zdiv[2];

	eudiv[0] = euv[0].u*zdiv[0];
	eudiv[1] = euv[1].u*zdiv[1];
	eudiv[2] = euv[2].u*zdiv[2];

	evdiv[0] = euv[0].v*zdiv[0];
	evdiv[1] = euv[1].v*zdiv[1];
	evdiv[2] = euv[2].v*zdiv[2];

	xlen1 = v[1].x-v[0].x;
	xlen2 = v[2].x-v[0].x;
	zlen1 = zdiv[1]-zdiv[0];
	zlen2 = zdiv[2]-zdiv[0];
	ulen1 = udiv[1]-udiv[0];
	ulen2 = udiv[2]-udiv[0];
	vlen1 = vdiv[1]-vdiv[0];
	vlen2 = vdiv[2]-vdiv[0];
	eulen1 = eudiv[1]-eudiv[0];
	eulen2 = eudiv[2]-eudiv[0];
	evlen1 = evdiv[1]-evdiv[0];
	evlen2 = evdiv[2]-evdiv[0];

	ymul1 = 1.f/(ylen1+1);
	ymul2 = 1.f/(ylen1+ylen2+1);

	xdelta1 = xlen1*ymul1;
	xdelta2 = xlen2*ymul2;
	zdelta1 = zlen1*ymul1;
	zdelta2 = zlen2*ymul2;
	udelta1 = ulen1*ymul1;
	udelta2 = ulen2*ymul2;
	vdelta1 = vlen1*ymul1;
	vdelta2 = vlen2*ymul2;
	eudelta1 = eulen1*ymul1;
	eudelta2 = eulen2*ymul2;
	evdelta1 = evlen1*ymul1;
	evdelta2 = evlen2*ymul2;

	ey = ((int)(v[0].y+1.f))-v[0].y;
	ex = xdelta1*ey;
	x1 = v[0].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[0]+ex;
	ex = udelta1*ey;
	u1 = udiv[0]+ex;
	ex = vdelta1*ey;
	v1 = vdiv[0]+ex;
	ex = eudelta1*ey;
	eu1 = eudiv[0]+ex;
	ex = evdelta1*ey;
	ev1 = evdiv[0]+ex;

	ex = xdelta2*ey;
	x2 = v[0].x+ex;
	ex = zdelta2*ey;
	z2 = zdiv[0]+ex;
	ex = udelta2*ey;
	u2 = udiv[0]+ex;
	ex = vdelta2*ey;
	v2 = vdiv[0]+ex;
	ex = eudelta2*ey;
	eu2 = eudiv[0]+ex;
	ex = evdelta2*ey;
	ev2 = evdiv[0]+ex;

	ay = (int)v[0].y;
	by = (int)v[1].y;

	fxdelta1 = (int)(xdelta1*65536.f);
	fx1 = (int)(x1*65536.f);
	fxdelta2 = (int)(xdelta2*65536.f);
	fx2 = (int)(x2*65536.f);

	fzdelta1 = (int)(zdelta1*4294967296.f);
	fz1 = (int)(z1*4294967296.f);
	fzdelta2 = (int)(zdelta2*4294967296.f);
	fz2 = (int)(z2*4294967296.f);

	fudelta1 = (int)(udelta1*65536.f);
	fu1 = (int)(u1*65536.f);
	fudelta2 = (int)(udelta2*65536.f);
	fu2 = (int)(u2*65536.f);
	feudelta1 = (int)(eudelta1*65536.f);
	feu1 = (int)(eu1*65536.f);
	feudelta2 = (int)(eudelta2*65536.f);
	feu2 = (int)(eu2*65536.f);

	fvdelta1 = (int)(vdelta1*65536.f);
	fv1 = (int)(v1*65536.f);
	fvdelta2 = (int)(vdelta2*65536.f);
	fv2 = (int)(v2*65536.f);
	fevdelta1 = (int)(evdelta1*65536.f);
	fev1 = (int)(ev1*65536.f);
	fevdelta2 = (int)(evdelta2*65536.f);
	fev2 = (int)(ev2*65536.f);


	if((by-ay)>0){
		for(counter=(by-ay);counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			current_edge->u2start = feu1;
			current_edge->u2stop = feu2;
			current_edge->v2start = fev1;
			current_edge->v2stop = fev2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			feu1 += feudelta1;
			feu2 += feudelta2;
			fev1 += fevdelta1;
			fev2 += fevdelta2;
			current_edge++;
			total_len++;
		}
	}

	xlen1 = v[2].x-v[1].x;
	zlen1 = zdiv[2]-zdiv[1];
	ulen1 = udiv[2]-udiv[1];
	vlen1 = vdiv[2]-vdiv[1];
	eulen1 = eudiv[2]-eudiv[1];
	evlen1 = evdiv[2]-evdiv[1];

	ymul1 = 1/(ylen2+1);
	xdelta1 = xlen1*ymul1;
	zdelta1 = zlen1*ymul1;
	udelta1 = ulen1*ymul1;
	vdelta1 = vlen1*ymul1;
	eudelta1 = eulen1*ymul1;
	evdelta1 = evlen1*ymul1;

	ey = ((int)(v[1].y+1.f))-v[1].y;

	ex = xdelta1*ey;
	x1 = v[1].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[1]+ex;
	ex = udelta1*ey;
	u1 = udiv[1]+ex;
	ex = vdelta1*ey;
	v1 = vdiv[1]+ex;
	ex = eudelta1*ey;
	eu1 = eudiv[1]+ex;
	ex = evdelta1*ey;
	ev1 = evdiv[1]+ex;

	ay = (int)v[1].y;
	by = (int)v[2].y;

	if((by-ay)>0){
		fxdelta1 = (int)(xdelta1*65536.f);
		fx1 = (int)(x1*65536.f);
		fzdelta1 = (int)(zdelta1*4294967296.f);
		fz1 = (int)(z1*4294967296.f);
		fudelta1 = (int)(udelta1*65536.f);
		fu1 = (int)(u1*65536.f);
		fvdelta1 = (int)(vdelta1*65536.f);
		fv1 = (int)(v1*65536.f);
		feudelta1 = (int)(eudelta1*65536.f);
		feu1 = (int)(eu1*65536.f);
		fevdelta1 = (int)(evdelta1*65536.f);
		fev1 = (int)(ev1*65536.f);
		for(counter=by-ay;counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			current_edge->u2start = feu1;
			current_edge->u2stop = feu2;
			current_edge->v2start = fev1;
			current_edge->v2stop = fev2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			feu1 += feudelta1;
			feu2 += feudelta2;
			fev1 += fevdelta1;
			fev2 += fevdelta2;
			current_edge++;
			total_len++;
		}
	}

	current_edge = edges;
	y = (int)v[0].y;
	buffer += (int)v[0].y*WIDTH;
	zbuffer_shit += (int)v[0].y*WIDTH;

	_asm pxor mm5, mm5

	for(;total_len;total_len--){
		perfect_envbump_texture_hline( buffer, zbuffer_shit, current_edge, y, texture1, texture2 );
		buffer += WIDTH;
		zbuffer_shit += WIDTH;
		current_edge++;
		y++;
	}

	_asm emms;
}
__inline void texture_hline( unsigned int* target, unsigned int* ztarget, poly_edge *edge, int y, unsigned int *texture ){
	unsigned int counter;
	unsigned int *bufferp;
	unsigned int *zbufferp;

	int x1 = edge->start;
	int x2 = edge->stop;
	int z1 = edge->zstart;
	int z2 = edge->zstop;
	int u1 = edge->ustart;
	int u2 = edge->ustop;
	int v1 = edge->vstart;
	int v2 = edge->vstop;
	int zdelta;
	int udelta;
	int vdelta;

	if( x1==x2 ) return;
	if( x2 < x1 ){
		xchg_int( &x1, &x2 );
		xchg_int( &z1, &z2 );
		xchg_int( &u1, &u2 );
		xchg_int( &v1, &v2 );
	}

	zdelta = (z2-z1)/(x2-x1);
	udelta = (u2-u1)/(x2-x1);
	vdelta = (v2-v1)/(x2-x1);

	z1 += zdelta>>1;
	u1 += udelta>>1;
	v1 += vdelta>>1;

	bufferp = target+x1;
	zbufferp = ztarget+x1;

	for( counter=x2-x1; counter; counter-- ){
		if(*zbufferp<z1){
			*bufferp = texture[((u1>>16)&0xFF)|((v1>>8)&0xFF00)];
			*zbufferp = z1;
		}
		z1 += zdelta;
		u1 += udelta;
		v1 += vdelta;
		bufferp++;
		zbufferp++;
	}
}

void draw_texture_triangle( unsigned int *buffer, vertex_2d v[3], texcoord uv[3], unsigned int *texture ){
	poly_edge *current_edge = edges;
	unsigned int *zbuffer_shit = zbuffer;
	int total_len = 0;

	float x1, x2;
	float z1, z2;
	float u1, u2;
	float v1, v2;
	float xdelta1, xdelta2;
	float zdelta1, zdelta2;
	float udelta1, udelta2;
	float vdelta1, vdelta2;

	float ylen1, ylen2;
	float xlen1, xlen2;
	float zlen1, zlen2;
	float ulen1, ulen2;
	float vlen1, vlen2;
	float ex, ey;
	float zdiv[3];

	float ymul1;
	float ymul2;

	int counter;
	int ay, by;

	int fxdelta1, fxdelta2;
	int fzdelta1, fzdelta2;
	int fudelta1, fudelta2;
	int fvdelta1, fvdelta2;
	int fx1, fx2;
	int fz1, fz2;
	int fu1, fu2;
	int fv1, fv2;
	int y;

	if(v[1].y > v[2].y){
		xchg_vertex2d(&v[1], &v[2]);
		xchg_texcoord(&uv[1], &uv[2]);
	}
	if(v[0].y > v[2].y){
		xchg_vertex2d(&v[0], &v[2]);
		xchg_texcoord(&uv[0], &uv[2]);
	}
	if(v[0].y > v[1].y){
		xchg_vertex2d(&v[0], &v[1]);
		xchg_texcoord(&uv[0], &uv[1]);
	}

	ylen1 = v[1].y-v[0].y;
	ylen2 = v[2].y-v[1].y;

	// kalkuler lengste scanline (den ved v[1]) osv,
	// for å slippe divisjoner i hline
	zdiv[0] = 1.0f/v[0].z;
	zdiv[1] = 1.0f/v[1].z;
	zdiv[2] = 1.0f/v[2].z;

	xlen1 = v[1].x-v[0].x;
	xlen2 = v[2].x-v[0].x;
	zlen1 = zdiv[1]-zdiv[0];
	zlen2 = zdiv[2]-zdiv[0];
	ulen1 = uv[1].u-uv[0].u;
	ulen2 = uv[2].u-uv[0].u;
	vlen1 = uv[1].v-uv[0].v;
	vlen2 = uv[2].v-uv[0].v;

	ymul1 = 1.f/(ylen1+1);
	ymul2 = 1.f/(ylen1+ylen2+1);

	xdelta1 = xlen1*ymul1;
	xdelta2 = xlen2*ymul2;
	zdelta1 = zlen1*ymul1;
	zdelta2 = zlen2*ymul2;
	udelta1 = ulen1*ymul1;
	udelta2 = ulen2*ymul2;
	vdelta1 = vlen1*ymul1;
	vdelta2 = vlen2*ymul2;

	ey = ((int)(v[0].y+1.f))-v[0].y;
	ex = xdelta1*ey;
	x1 = v[0].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[0]+ex;
	ex = udelta1*ey;
	u1 = uv[0].u+ex;
	ex = vdelta1*ey;
	v1 = uv[0].v+ex;

	ex = xdelta2*ey;
	x2 = v[0].x+ex;
	ex = zdelta2*ey;
	z2 = zdiv[0]+ex;
	ex = udelta2*ey;
	u2 = uv[0].u+ex;
	ex = vdelta2*ey;
	v2 = uv[0].v+ex;

	ay = (int)v[0].y;
	by = (int)v[1].y;

	fxdelta1 = (int)(xdelta1*65536.f);
	fx1 = (int)(x1*65536.f);
	fxdelta2 = (int)(xdelta2*65536.f);
	fx2 = (int)(x2*65536.f);

	fzdelta1 = (int)(zdelta1*4294967296.f);
	fz1 = (int)(z1*4294967296.f);
	fzdelta2 = (int)(zdelta2*4294967296.f);
	fz2 = (int)(z2*4294967296.f);

	fudelta1 = (int)(udelta1*65536.f);
	fu1 = (int)(u1*65536.f);
	fudelta2 = (int)(udelta2*65536.f);
	fu2 = (int)(u2*65536.f);

	fvdelta1 = (int)(vdelta1*65536.f);
	fv1 = (int)(v1*65536.f);
	fvdelta2 = (int)(vdelta2*65536.f);
	fv2 = (int)(v2*65536.f);


	if((by-ay)>0){
		for(counter=(by-ay);counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			current_edge++;
			total_len++;
		}
	}

	xlen1 = v[2].x-v[1].x;
	zlen1 = zdiv[2]-zdiv[1];
	ulen1 = uv[2].u-uv[1].u;
	vlen1 = uv[2].v-uv[1].v;

	ymul1 = 1/(ylen2+1);
	xdelta1 = xlen1*ymul1;
	zdelta1 = zlen1*ymul1;
	udelta1 = ulen1*ymul1;
	vdelta1 = vlen1*ymul1;

	ey = ((int)(v[1].y+1.f))-v[1].y;

	ex = xdelta1*ey;
	x1 = v[1].x+ex;
	ex = zdelta1*ey;
	z1 = zdiv[1]+ex;
	ex = udelta1*ey;
	u1 = uv[1].u+ex;
	ex = vdelta1*ey;
	v1 = uv[1].v+ex;

	ay = (int)v[1].y;
	by = (int)v[2].y;

	if((by-ay)>0){
		fxdelta1 = (int)(xdelta1*65536.f);
		fx1 = (int)(x1*65536.f);
		fzdelta1 = (int)(zdelta1*4294967296.f);
		fz1 = (int)(z1*4294967296.f);
		fudelta1 = (int)(udelta1*65536.f);
		fu1 = (int)(u1*65536.f);
		fvdelta1 = (int)(vdelta1*65536.f);
		fv1 = (int)(v1*65536.f);
		for(counter=by-ay;counter;counter--){
			current_edge->start = fx1>>16;
			current_edge->stop = fx2>>16;
			current_edge->zstart = fz1;
			current_edge->zstop = fz2;
			current_edge->ustart = fu1;
			current_edge->ustop = fu2;
			current_edge->vstart = fv1;
			current_edge->vstop = fv2;
			fx1 += fxdelta1;
			fx2 += fxdelta2;
			fz1 += fzdelta1;
			fz2 += fzdelta2;
			fu1 += fudelta1;
			fu2 += fudelta2;
			fv1 += fvdelta1;
			fv2 += fvdelta2;
			current_edge++;
			total_len++;
		}
	}

	current_edge = edges;
	y = (int)v[0].y;
	buffer += (int)v[0].y*WIDTH;
	zbuffer_shit += (int)v[0].y*WIDTH;
	for(;total_len;total_len--){
		texture_hline( buffer, zbuffer_shit, current_edge, y, texture );
		buffer += WIDTH;
		zbuffer_shit += WIDTH;
		current_edge++;
		y++;
	}
}


void draw_pixel( unsigned int *screen, vertex_2d v ){
	int offset;
	if(v.x<1.f) return;
	if(v.x>WIDTH) return;
	if(v.y<1.f) return;
	if(v.y>HEIGHT) return;

	offset = (int)v.x+(((int)v.y)*320);

	screen[offset] = 0xFFFFFF;
}
