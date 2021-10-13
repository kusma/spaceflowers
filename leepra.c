#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>

#include <stdio.h>

HWND win;
LPDIRECTDRAW ddraw;
LPDIRECTDRAWSURFACE primarybuffer;
LPDIRECTDRAWSURFACE backbuffer;
DDPIXELFORMAT pixelformat;
LPDIRECTDRAWCLIPPER clipper;

typedef HRESULT (WINAPI * DIRECTDRAWCREATE) (GUID FAR *lpGUID,LPDIRECTDRAW FAR *lplpDD,IUnknown FAR *pUnkOuter);
static HMODULE library = 0;

static LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

int width, height;
BOOL fullscreen = TRUE;
BOOL active=FALSE;
HINSTANCE inst;

// Converter-stuff
typedef void* (*CONVERTER) (void *dst,const void *src, unsigned int count);
CONVERTER converter;
CONVERTER get_converter( unsigned int bpp, unsigned int r_mask, unsigned int g_mask, unsigned int b_mask );

int leepra_open( char* title, int width_, int height_, BOOL fullscreen_){
	DIRECTDRAWCREATE DirectDrawCreate;
	HRESULT result;
	RECT rect;
	WNDCLASS wc;
	DDSURFACEDESC desc;
	DDSCAPS caps;
	unsigned int style;

	inst = GetModuleHandle(NULL);
	fullscreen = fullscreen_;
	width = width_;
	height = height_;

	library = (HMODULE) LoadLibrary("ddraw.dll");
	if (!library) return FALSE;
	
	DirectDrawCreate = (DIRECTDRAWCREATE) GetProcAddress(library,"DirectDrawCreate");
	if (!DirectDrawCreate) return FALSE;

	result = DirectDrawCreate( 0, &ddraw, 0 );
	if(FAILED(result)) return FALSE;

	if(fullscreen){
		result = IDirectDraw_SetDisplayMode( ddraw, width, height, 32 );
		if(FAILED(result)){
			result = IDirectDraw_SetDisplayMode( ddraw, width, height, 24 );
			if(FAILED(result)){
				result = IDirectDraw_SetDisplayMode( ddraw, width, height, 16 );
				if(FAILED(result)) return FALSE;
			}
		}
	}


	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	if(!fullscreen){
		style = WS_VISIBLE|WS_OVERLAPPEDWINDOW;//WS_VISIBLE|WS_OVERLAPPED|WS_THICKFRAME|WS_CAPTION;
		AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, FALSE );
	}else{
		style = WS_VISIBLE|WS_POPUP;
	}
	
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = 0;
	wc.hCursor = LoadCursor(0,IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = "leepra";
	RegisterClass(&wc);

	win = CreateWindowEx(0, "leepra", title, style, 0, 0, rect.right-rect.left, rect.bottom-rect.top, 0, 0, inst, 0);

	if( fullscreen ) result = IDirectDraw_SetCooperativeLevel( ddraw, win, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
	else result = IDirectDraw_SetCooperativeLevel( ddraw, win, DDSCL_NORMAL );
	if(FAILED(result)) return FALSE;

	memset( &desc, 0, sizeof(DDSURFACEDESC) );
	desc.dwSize = sizeof(DDSURFACEDESC);

	if(fullscreen){
		ShowCursor(0);
		desc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
		desc.dwBackBufferCount = 1;
	}else{
		desc.dwFlags = DDSD_CAPS;
		desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	}

	result = IDirectDraw_CreateSurface( ddraw, &desc, &primarybuffer, NULL );
	if( FAILED(result) ) return FALSE;

	if(fullscreen){
		int i;
		caps.dwCaps = DDSCAPS_BACKBUFFER;
		result = IDirectDrawSurface_GetAttachedSurface( primarybuffer, &caps, &backbuffer );
		if( FAILED(result) ) return FALSE;

		for(i=0; i<2; i++){
			DDSURFACEDESC descriptor;
			int pitch, y;
			char* dst;

			IDirectDrawSurface_Restore(primarybuffer);
			descriptor.dwSize = sizeof(DDSURFACEDESC);
			IDirectDrawSurface_Lock(backbuffer,0,&descriptor,DDLOCK_WAIT,0);
			pitch = descriptor.lPitch;

			dst = (char*)descriptor.lpSurface;
			for( y=height; y; y-- ){
				memset(dst, 0, width*4);
				dst += pitch;
			}
			IDirectDrawSurface_Unlock( backbuffer, descriptor.lpSurface);
			IDirectDrawSurface_Flip( primarybuffer, NULL, DDFLIP_WAIT );
		}
	}else{
		memset( &desc, 0, sizeof(DDSURFACEDESC) );
		desc.dwSize = sizeof(DDSURFACEDESC);
		desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		desc.dwWidth = width;
		desc.dwHeight = height;

		result = IDirectDraw_CreateSurface( ddraw, &desc, &backbuffer, NULL );
		if(FAILED(result)) return FALSE;

		IDirectDraw_CreateClipper( ddraw, 0, &clipper, 0 );
		IDirectDrawClipper_SetHWnd( clipper, 0,win);
		IDirectDrawSurface_SetClipper( primarybuffer, clipper );
	}

	pixelformat.dwSize = sizeof(DDPIXELFORMAT);
	IDirectDrawSurface_GetPixelFormat( primarybuffer, &pixelformat );
	if (!(pixelformat.dwFlags & DDPF_RGB)) return FALSE;

#ifdef _DEBUG
	printf("%u\n", pixelformat.dwRGBBitCount );
#endif

	converter = get_converter( pixelformat.dwRGBBitCount, pixelformat.dwRBitMask, pixelformat.dwGBitMask, pixelformat.dwBBitMask );
	if(converter == NULL) return FALSE;

#ifndef _DEBUG
//	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif

	active = TRUE;
	return TRUE;
}

void leepra_update( void* data ){
	DDSURFACEDESC descriptor;
	int pitch;
/*
	MSG message;
	while (PeekMessage(&message,win,0,0,PM_REMOVE)){
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
*/
	if(active==FALSE && fullscreen==TRUE) return;

	if(IDirectDrawSurface_IsLost(primarybuffer))IDirectDrawSurface_Restore(primarybuffer);
	if(!fullscreen&&IDirectDrawSurface_IsLost(backbuffer)) IDirectDrawSurface_Restore(backbuffer);

	descriptor.dwSize = sizeof(DDSURFACEDESC);

	IDirectDrawSurface_Lock( backbuffer, 0,&descriptor,DDLOCK_WAIT,0);

	pitch = descriptor.lPitch;

	if(pitch==width){
		converter(data, descriptor.lpSurface, width*height*4);
	}else{
		int y;
		char* dst = (char*)descriptor.lpSurface;
		char* src = (char*)data;
		for(y=height; y; y--){
			converter(dst, src, width*4);
			src += width*4;
			dst += pitch;
		}
	}

	IDirectDrawSurface_Unlock(backbuffer,descriptor.lpSurface);

	if(!fullscreen){
		RECT srcrect;
		RECT destrect;
		POINT point;

		point.x = 0;
		point.y = 0;
		ClientToScreen(win, &point);
		GetClientRect(win, &destrect);
		OffsetRect(&destrect, point.x, point.y);
		SetRect(&srcrect, 0, 0, width, height);
		IDirectDrawSurface_Blt( primarybuffer, &destrect, backbuffer, &srcrect, DDBLT_WAIT, NULL);
	}else{
		IDirectDrawSurface_Flip( primarybuffer, NULL, DDFLIP_WAIT );
	}
}

void leepra_close(){
	if(primarybuffer!=NULL) IDirectDrawSurface_Release(primarybuffer);
	IDirectDraw_Release(ddraw);
	ddraw = NULL;
	DestroyWindow( win );
	UnregisterClass("leepra", inst);

	ShowCursor( TRUE );
}

HWND leepra_get_window(){
	return win;
}

// pixelkonvertering
void* convert_argb32_argb888(void *dst, const void *src, unsigned int count);
void* convert_argb32_rgb888(void *dst, const void *src, unsigned int count);
void* convert_argb32_rgb565(void *dst, const void *src, unsigned int count);
void* convert_argb32_rgb555(void *dst, const void *src, unsigned int count);

CONVERTER get_converter( unsigned int bpp, unsigned int r_mask, unsigned int g_mask, unsigned int b_mask ){
	if( (bpp==32) ) return &memcpy;
	if( (bpp==24) ) return &convert_argb32_rgb888;
	if( (bpp==16) ) return &convert_argb32_rgb565;
	if( (bpp==15) ) return &convert_argb32_rgb555;

#ifdef _DEBUG
	printf("bpp:%u rmask:%x gmask:%x bmask:%x\n", bpp, r_mask, g_mask, b_mask);
#endif
	return NULL;
}

void* convert_argb32_argb888(void *dst, const void *src, unsigned int num){
	_asm{
		push ebp

		mov esi, src
		mov edi, dst
		mov ecx, num
		shr ecx, 3

	lup:
		movq mm0, [esi]
		movq [edi], mm0
		add esi, 8
		add edi, 8

		dec ecx
		jnz lup

		emms
		pop ebp
	}
	return NULL;
}

void* convert_argb32_rgb565(void *dst, const void *src, unsigned int num){
	_asm
	{
		push ebp
		mov esi,src
		mov edi,dst
		mov ecx,num
		xor edx,edx
	lup:
		mov eax,[esi+edx*4]
		mov ebx,eax
		shr ebx,8
		and ebx,1111100000000000b
		mov ebp,eax
		shr ebp,3
		and ebp,0000000000011111b
		add ebx,ebp
		shr eax,5
		and eax,0000011111100000b
		add ebx,eax
		mov [edi+edx*2],bx
		inc edx
		dec ecx
		jnz lup
		pop ebp
	}
	return NULL;
}

void* convert_argb32_rgb555(void *dst, const void *src, unsigned int num){
	_asm
	{
		push ebp
		mov esi,src
		mov edi,dst
		mov ecx,num
		xor edx,edx
	lup:
		mov eax,[esi+edx*4]
		mov ebx,eax
		shr ebx,9
		and ebx,0111110000000000b
		mov ebp,eax
		shr ebp,3
		and ebp,0000000000011111b
		add ebx,ebp
		shr eax,6
		and eax,0000001111100000b
		add ebx,eax
		mov [edi+edx*2],bx
		inc edx
		dec ecx
		jnz lup
		pop ebp
	}
	return NULL;
}


void* convert_argb32_rgb888(void *dst, const void *src, unsigned int num){
	_asm
	{
		push ebp
		mov esi,src
		mov edi,dst
		mov ecx,num
		xor edx,edx
	lup:
		mov eax,[esi]
		mov [edi],eax
		add esi,4
		add edi,3
		dec ecx
		jnz lup
		pop ebp
	}
	return NULL;
}

static LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam){
	switch (message){
	case WM_PAINT:
		if(!fullscreen){
			RECT srcrect;
			RECT destrect;
			POINT point;

			point.x = 0;
			point.y = 0;
			ClientToScreen(win, &point);
			GetClientRect(win, &destrect);
			OffsetRect(&destrect, point.x, point.y);
			SetRect(&srcrect, 0, 0, width, height);
			IDirectDrawSurface_Blt( primarybuffer, &destrect, backbuffer, &srcrect, DDBLT_WAIT, NULL);
		}else{
			IDirectDrawSurface_Flip( primarybuffer, NULL, DDFLIP_WAIT );
		}
	break;
	case WM_ACTIVATEAPP:
		active = (BOOL) wParam;
	break;
	
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_SYSCOMMAND:
		switch (wParam){
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		break;
		}
	break;

	}
    return DefWindowProc(hWnd,message,wParam,lParam);
}