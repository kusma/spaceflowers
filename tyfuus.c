#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "tyfuus.h"
#include "file.h"

#define WIDTH 320
#define HEIGHT 240

#include <math.h>
#pragma intrinsic( cos, sin )
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

#include <commdlg.h>
#include <ocidl.h>
#include <olectl.h>
#include <crtdbg.h>
#define HIMETRIC_INCH	2540

#ifdef _DEBUG
#include <stdio.h>
#endif

void tyfuus_multiply( tyfuus_bitmap *bitmap1, tyfuus_bitmap *bitmap2 ){
	unsigned int* buffer1 = bitmap1->data;
	unsigned int* buffer2 = bitmap2->data;
	int len = bitmap2->width*bitmap2->height;

	__asm{
		mov edi, buffer1
		mov esi, buffer2
		mov ecx, len
		pxor mm5, mm5

align 16
lup:
		movd mm0, [edi]
		movd mm1, [esi]
		punpcklbw mm0, mm5
		punpcklbw mm1, mm5

		pmullw mm0, mm1
		add esi, 4
		psrlw mm0, 8
		packuswb mm0, mm0
		movd eax, mm0

		mov [edi], eax
		add edi, 4

		dec ecx
		jnz lup

		emms
	}
}

void tyfuus_blend( tyfuus_bitmap *bitmap1, tyfuus_bitmap *bitmap2, unsigned char alpha ){
	unsigned int* buffer1 = bitmap1->data;
	unsigned int* buffer2 = bitmap2->data;
	int len = bitmap2->width*bitmap2->height;

	unsigned short blend[4]={alpha,alpha,alpha,alpha};

	__asm{
		movq mm6, [blend]
		pxor mm5, mm5

		mov edi, buffer1
		mov esi, buffer2
		mov ecx, len
		pxor mm5, mm5

align 16
lup:
		movd mm0, [edi]
		movd mm1, [esi]
		punpcklbw mm0, mm5
		punpcklbw mm1, mm5

		psubw mm1, mm0
		add esi, 4
		pmullw mm1, mm6
		psllw mm0, 8
		paddw mm0, mm1
		psrlw mm0, 8
		packuswb mm0, mm5

		movd [edi], mm0
		add edi, 4

		dec ecx
		jnz lup

		emms
	}
}


void tyfuus_alphablend( tyfuus_bitmap *bitmap1, tyfuus_bitmap *bitmap2, unsigned char alpha ){
	unsigned int* buffer1;
	unsigned int* buffer2;
	int len;

	unsigned short blend[4]={alpha,alpha,alpha,alpha};

	if(alpha==0) return;
	if(alpha==255){
		tyfuus_alphablend_opaque( bitmap1, bitmap2 );
		return;
	}

	buffer1 = bitmap1->data;
	buffer2 = bitmap2->data;
	len = bitmap2->width*bitmap2->height;

	__asm{
		movq mm7, [blend]
		pxor mm5, mm5
		mov edi, buffer1
		mov esi, buffer2
		mov ecx, len

align 16
lup:
		movd mm0, [edi]
		mov eax, [esi]
		movd mm1, eax

		shr eax, 24
		mov ah, al
		mov ebx, eax
		shl ebx, 16
		or eax, ebx

		movd mm6, eax
		punpcklbw mm0, mm5
		punpcklbw mm1, mm5
		punpcklbw mm6, mm5

		psubw mm1, mm0

		pmullw mm6, mm7
		psrlw mm6, 8
	
		add esi, 4
		pmullw mm1, mm6
		psllw mm0, 8
		paddw mm0, mm1
		psrlw mm0, 8
		packuswb mm0, mm5

		movd [edi], mm0
//		mov [edi], eax
		add edi, 4

		dec ecx
		jnz lup

		emms
	}

}

void tyfuus_alphablend_opaque( tyfuus_bitmap *bitmap1, tyfuus_bitmap *bitmap2 ){
	unsigned int* buffer1 = bitmap1->data;
	unsigned int* buffer2 = bitmap2->data;
	int len = bitmap2->width*bitmap2->height;

	__asm{
		pxor mm5, mm5
		mov edi, buffer1
		mov esi, buffer2
		mov ecx, len

align 16
lup:
		movd mm0, [edi]
		mov eax, [esi]
		movd mm1, eax

		shr eax, 24
		mov ah, al
		mov ebx, eax
		shl ebx, 16
		or eax, ebx

		movd mm6, eax
		punpcklbw mm0, mm5
		punpcklbw mm1, mm5
		punpcklbw mm6, mm5

		psubw mm1, mm0
		add esi, 4
		pmullw mm1, mm6
		psllw mm0, 8
		paddw mm0, mm1
		psrlw mm0, 8
		packuswb mm0, mm5

		movd [edi], mm0
		add edi, 4

		dec ecx
		jnz lup

		emms
	}
}

void tyfuus_colorfade( tyfuus_bitmap *bitmap, unsigned int color, unsigned char alpha ){
	unsigned int *buffer = bitmap->data;
	int len = bitmap->width*bitmap->height;

	static short white[]= {0xff,0xff,0xff,0xff};
	__asm{

		xor eax, eax
		mov al, alpha
		mov ebx, eax
		shl ebx, 8
		add eax, ebx
		shl ebx, 8
		add eax, ebx
		shl ebx, 8
		add eax, ebx

		pxor mm5, mm5

		movd mm2, eax
		punpcklbw mm2, mm5

		movq mm3, white
		psubusw mm3, mm2
		
		mov edi, buffer

		movd mm1, color
		punpcklbw mm1, mm5

		pmullw mm1, mm2
		psrlw mm1, 8

		mov ecx, len
align 16
lup:
		movd mm0, [edi]
		punpcklbw mm0, mm5
		pmullw mm0, mm3
		psrlw mm0, 8
		paddusw mm0, mm1
		packuswb mm0, mm0
		movd eax, mm0
		stosd

		dec ecx
		jnz lup

		emms
	}
}

void tyfuus_gamma( tyfuus_bitmap *bitmap1, float gamma ){
	int counter;
	static unsigned char gammatable[256];
	unsigned char* src = (unsigned char*)bitmap1->data;

	for( counter=0; counter<256; counter++ ){
		int val = (int)pow(counter,gamma);
		if(val<0) val=0;
		if(val>255) val=255;
		gammatable[counter] = val;
	}

	for( counter=bitmap1->width*bitmap1->height; counter; counter-- ){
		*src++ = gammatable[*src];
		*src++ = gammatable[*src];
		*src++ = gammatable[*src];
		src++;
	}
}

void tyfuus_invert( tyfuus_bitmap *bitmap1, unsigned char invert ){
	int counter;
	unsigned char* src = (unsigned char*)bitmap1->data;
	unsigned char inv_invert = 255-invert;
//	static short white[]= {0xff,0xff,0xff,0xff};
//	unsigned short invert[] = {gamma, gamma, gamma, gamma};

	// bør skrives om i assembler med mmx og i 1-mul-variant
	// tarre når jeg kjeder meg... :P
	for( counter=bitmap1->width*bitmap1->height; counter; counter-- ){
		*src = (((255-*src)*invert)+(*src*(inv_invert)))>>8;
		src++;
		*src = (((255-*src)*invert)+(*src*(inv_invert)))>>8;
		src++;
		*src = (((255-*src)*invert)+(*src*(inv_invert)))>>8;
		src++;
		src++;
	}
}

/*
	(255-pixel)*invert+pixel*(255-invert)

*/


HFONT font;
HDC hdc_mem;
HBITMAP bitmap;
BITMAPINFO bi;

void tyfuus_init_font( int width, int height, char *fontname, int size ){
	LOGFONT lf;
	HDC hdc;

	hdc = GetDC( NULL );
	bitmap = CreateCompatibleBitmap(hdc, width, height);
	hdc_mem = CreateCompatibleDC(hdc);
	SelectObject(hdc_mem, bitmap);

	lf.lfHeight         = size;
	lf.lfWidth          = 0;
	lf.lfEscapement     = 0;
	lf.lfOrientation    = 0;
	lf.lfWeight         = 0;
	lf.lfItalic         = 0;
	lf.lfUnderline      = 0;
	lf.lfStrikeOut      = 0;
	lf.lfCharSet        = 0;
	lf.lfOutPrecision   = 0;
	lf.lfClipPrecision  = 0;
	lf.lfQuality        = ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily = 0;
	strcpy( lf.lfFaceName, fontname );
	font = CreateFontIndirect( &lf );

	memset( &bi, 0, sizeof(BITMAPINFO) );
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = width;
	bi.bmiHeader.biHeight = -height;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;

}

void tyfuus_free_font(){
	DeleteObject(font);
	DeleteDC(hdc_mem);
	DeleteObject(bitmap);
}

void tyfuus_print( tyfuus_bitmap *target, int xpos, int ypos, char* text, unsigned int color ){
	unsigned int *buffer = target->data;
	int width = target->width;
	int height = target->height;

	SelectObject(hdc_mem, bitmap);
	SetDIBits( hdc_mem, bitmap, 0, height, buffer, &bi, DIB_RGB_COLORS );

	SetBkMode( hdc_mem, TRANSPARENT );
	SelectObject( hdc_mem, font );
	SetTextColor( hdc_mem,	((color&0xFF0000)>>16)|(color&0xFF00)|((color&0xFF)<<16) );
	TextOut(hdc_mem, xpos, ypos, text, strlen(text));

	GetDIBits( hdc_mem, bitmap, 0, height, buffer, &bi, DIB_RGB_COLORS );
}

tyfuus_bitmap* tyfuus_make_bitmap( int width, int height ){
	tyfuus_bitmap *image = malloc( sizeof( tyfuus_bitmap ) );
	if (image==NULL) return NULL;

	image->width = width;
	image->height = height;
	image->data = malloc( sizeof(unsigned int)*width*height );
	if(image->data==NULL) return NULL;
	
	memset( image->data, 0, sizeof(unsigned int)*width*height );

	return image;
}

void tyfuus_free_bitmap( tyfuus_bitmap *bitmap ){
	free(bitmap->data);
	bitmap->data = NULL;
	free(bitmap);
	bitmap = NULL;
}


tyfuus_bitmap *tyfuus_load_bitmap( char *filename ){
	tyfuus_bitmap *image;
	BITMAPINFO bi;
	HGLOBAL global;
	void* temp_data;
	LPSTREAM stream;
	HRESULT hr;
	LPPICTURE picture;
	long width, height;
	int width_, height_;

	HBITMAP bitmap;
	HDC hdc, hdc_mem;
	unsigned int* image_data;

	file* file = file_open(filename);
	if(file==NULL){
		return NULL;
	}

	image = malloc( sizeof(tyfuus_bitmap) );
	global = GlobalAlloc( GMEM_MOVEABLE, file->size );
	temp_data = GlobalLock( global );
	memcpy( temp_data, file->data, file->size );
	GlobalUnlock( global );
	stream = NULL;
	hr = CreateStreamOnHGlobal( global, TRUE, &stream );
	if(FAILED(hr)) return NULL;

	OleLoadPicture(
		stream,
		file->size,
		FALSE,
		&IID_IPicture,
		(LPVOID *)&picture
	);

	stream->lpVtbl->Release(stream);
	file_close( file );
	
	picture->lpVtbl->get_Width( picture, &width );
	picture->lpVtbl->get_Height( picture, &height );

	hdc = GetDC( NULL );
	width_ = MulDiv(width, GetDeviceCaps(hdc, LOGPIXELSX), HIMETRIC_INCH);
	height_ = MulDiv(height, GetDeviceCaps(hdc, LOGPIXELSY), HIMETRIC_INCH);

	bitmap = CreateCompatibleBitmap(hdc, width_, height_ );
	hdc_mem = CreateCompatibleDC(hdc);
	SelectObject(hdc_mem, bitmap);

	picture->lpVtbl->Render( picture, hdc_mem, 0, 0, width_, height_, 0, height, width, -height, NULL );
	image_data = malloc(sizeof(unsigned int)*width_*height_);

	memset( &bi, 0, sizeof(BITMAPINFO) );
	bi.bmiHeader.biSize=sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = width_;
	bi.bmiHeader.biHeight = -height_;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	GetDIBits( hdc_mem, bitmap, 0, height_, image_data, &bi, DIB_RGB_COLORS );
	DeleteDC(hdc_mem);
	DeleteObject(bitmap);

	image->width = width_;
	image->height = height_;
	image->data = image_data;
	return image;
}

tyfuus_bitmap *tyfuus_load_bitmap_alpha( char *filename, char * alphaname ){
	int len;
	unsigned int *src, *dst;
	tyfuus_bitmap *temp1, *temp2;

	temp1 = tyfuus_load_bitmap( filename );
	if( temp1==NULL ) return NULL;
	temp2 = tyfuus_load_bitmap( alphaname );
	if( temp2==NULL ) return NULL;

	dst = temp1->data;
	src = temp2->data;

	for( len=temp1->height*temp1->width; len; len--){
		unsigned int alpha = ((((*src&0xFF)+(*src&0xFF00>>8)+(*src&0xFF0000>>16))/3)&0xFF)<<24;
		*dst++ |= alpha;
		src++;
	}
	
	tyfuus_free_bitmap( temp2 );
	return temp1;
}



tyfuus_offsetmap *tyfuus_make_radialmap( int width, int height ){
	int x,y;
	unsigned int *table;
	tyfuus_offsetmap *temp = (tyfuus_offsetmap*)malloc( sizeof( tyfuus_offsetmap ) );
	temp->data = malloc( sizeof(unsigned int)*width*height );
	temp->width = width;
	temp->height = height;
	table = temp->data;

	for (y=0; y<height; y++){
		for (x=0; x<width; x++){
			unsigned int u,v;
			int x2, y2;
			double deg, rad;
			x2 = x-(width>>1);
			y2 = y-(height>>1);
			deg = atan2( x2,y2 );
			rad = sqrt(x2*x2+y2*y2);
			u = (int)(deg*(128.f/M_PI));
			v = (int)rad;
			*table++ = ((u&0xFF)+((v&0xFF)<<8));
		}
	}
	return temp;
}

tyfuus_offsetmap *tyfuus_make_zoommap( int width, int height ){
	int x,y;
	unsigned int *table;
	tyfuus_offsetmap *temp = (tyfuus_offsetmap*)malloc( sizeof( tyfuus_offsetmap ) );
	temp->data = malloc( sizeof(unsigned int)*width*height );
	temp->width = width;
	temp->height = height;
	table = temp->data;

	for (y=0; y<height; y++){
		for (x=0; x<width; x++){
			unsigned int u,v;
			int x2, y2;
			double deg, rad;
			x2 = x-(width>>1);
			y2 = y-(height>>1);
			deg = atan2( x2,y2 );
			rad = sqrt(x2*x2+y2*y2);
			u = (int)(deg*(128.f/M_PI));
			v = (int)(log(rad)*50);
			*table++ = ((u&0xFF)+((v&0xFF)<<8));
		}
	}
	return temp;
}

tyfuus_offsetmap *tyfuus_make_wierdmap( int width, int height ){
	int x,y;
	unsigned int *table;
	tyfuus_offsetmap *temp = (tyfuus_offsetmap*)malloc( sizeof( tyfuus_offsetmap ) );
	temp->data = malloc( sizeof(unsigned int)*width*height );
	temp->width = width;
	temp->height = height;
	table = temp->data;

	for (y=0; y<height; y++){
		for (x=0; x<width; x++){
			unsigned int u,v;
			int x2, y2;
			double deg, rad;
			x2 = x-(width>>1);
			y2 = y-(height>>1);
			rad = sqrt(x2*x2+y2*y2);
			deg = sin( x/(rad+0.001f) );
			u = (int)(deg*(128.f/M_PI));
			v = (int)(log(rad)*50);
			*table++ = ((u&0xFF)+((v&0xFF)<<8));
		}
	}
	return temp;
}


void tyfuus_free_offsetmap( tyfuus_offsetmap *offsetmap ){
	free( offsetmap->data );
	offsetmap->data = NULL;
	offsetmap->width = 0;
	offsetmap->height = 0;
	free( offsetmap );
	offsetmap = NULL;
}

void tyfuus_draw_offsetmap(tyfuus_bitmap *bitmap, tyfuus_offsetmap *offset_table, int xoffs, int yoffs, int x1, int y1, int x2, int y2, int x3, int y3, tyfuus_bitmap *texture ){
	int x,y;
	unsigned int *node1, *node2, *node3;
	unsigned int offset = (xoffs&0xFF)+((yoffs&0xFF)<<8);
	int width = offset_table->width;
	int height = offset_table->height;
	int bitmap_width = bitmap->width;
	int bitmap_height = bitmap->height;
	unsigned int *buffer = bitmap->data;
	unsigned int *texture_data = texture->data;
	int add_me = width-bitmap_width;
	node1 = offset_table->data+((width>>2)+x1)+((height>>2)+y1)*width;
	node2 = offset_table->data+((width>>2)+x2)+((height>>2)+y2)*width;
	node3 = offset_table->data+((width>>2)+x3)+((height>>2)+y3)*width;
	for(y=bitmap_height; y; y--){
		for(x=bitmap_width; x; x--){
			*buffer++ =	texture_data[(
				 *(node1)
				+*(node2)
				+*(node3)
				+offset
				)&0xFFFF];
			node1++;
			node2++;
			node3++;
		}
		node1+=add_me;
		node2+=add_me;
		node3+=add_me;
	}
}

void tyfuus_draw_quad_8x8( tyfuus_bitmap *target, int x, int y, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4, float s1, float s2, float s3, float s4, tyfuus_bitmap *texture){
	unsigned int *dst = target->data+x+(y*320);
	unsigned int *src = texture->data;

	int fu1 = (int)(u1*256.f);
	int fu2 = (int)(u2*256.f);
	int fv1 = (int)(v1*256.f);
	int fv2 = (int)(v2*256.f);
	int fs1 = (int)(s1*256.f);
	int fs2 = (int)(s2*256.f);

	int u_delta1 = (int)((u3-u1)*256.f)>>3;
	int u_delta2 = (int)((u4-u2)*256.f)>>3;
	int v_delta1 = (int)((v3-v1)*256.f)>>3;
	int v_delta2 = (int)((v4-v2)*256.f)>>3;
	int s_delta1 = (int)((s3-s1)*256.f)>>3;
	int s_delta2 = (int)((s4-s2)*256.f)>>3;

	_asm{
		mov edi, dst
		mov esi, src
		push esi
		push edi
	}
	for(y=8;y;y--){
		int s = fs1;
		int u_delta = (fu2-fu1)>>3;
		int v_delta = (fv2-fv1)>>3;
		int s_delta = (fs2-fs1)>>3;

		_asm{
			pop edi
			pop esi

			mov eax, fu1
			shl eax, 16
			add eax, fv1
			movd mm0, eax

			mov eax, u_delta
			shl eax, 16
			add eax, v_delta
			movd mm1, eax

			mov cl, 8
align 16
xlup:
			movq mm2, mm0

			psrlw mm2, 8
			packuswb mm2, mm5

			movd ebx, mm2
			and ebx, 0xFFFF
			mov eax, [esi+ebx*4]

			mov [edi], eax
			paddw mm0, mm1
			add edi, 4
			dec cl
			jnz xlup

			add edi, (320-8)*4
			push esi
			push edi
		}
		dst += 320-8;
		fv1 += v_delta1;
		fu1 += u_delta1;
		fv2 += v_delta2;
		fu2 += u_delta2;
//		s1 += s_delta1;
//		s2 += s_delta2;
	}
	_asm{
		pop edi
		pop esi
		emms
	}
}

tyfuus_grid *tyfuus_make_grid(int width, int height){
	tyfuus_grid *temp = (tyfuus_grid*)malloc(sizeof(tyfuus_grid));
	if( temp==NULL ) return NULL;

	width = width>>3;
	height = height>>3;

	temp->width = width+1;
	temp->height = height+1;
	temp->data = (tyfuus_grid_node*)malloc( sizeof(tyfuus_grid_node)*(width+1)*(height+1) );
	if(temp->data==NULL){
		free(temp);
		temp = NULL;
		return NULL;
	}

	return temp;
}

void tyfuus_free_grid( tyfuus_grid *grid ){
	free( grid->data );
	grid->data = NULL;
	grid->width = 0;
	grid->height = 0;
	free( grid );
}

void tyfuus_expand_grid( tyfuus_bitmap *bitmap, tyfuus_grid *grid, tyfuus_bitmap *texture ){
	int x,y;
	int w = grid->width-1;
	int h = grid->height-1;
	tyfuus_grid_node *node = grid->data;

	printf("%i %i\r", w, h);
	
	for(y=0;y<h;y++){
		for(x=0;x<w;x++){
			tyfuus_draw_quad_8x8( bitmap,
				x<<3,y<<3,

				node->u, node->v,
				(node+1)->u, (node+1)->v,
				(node+w+1)->u, (node+w+1)->v,
				(node+w+2)->u, (node+w+2)->v,

				node->shade,
				(node+1)->shade,
				(node+w+1)->shade,
				(node+w+2)->shade,

				texture );
			node++;
		}
		node++;
	}


}
