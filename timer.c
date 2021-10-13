#define WIN32_LEAN_AND_MEAN
#include <windows.h>

BOOL present;
LARGE_INTEGER freq;
float scalar;

void init_timer(){
	if(QueryPerformanceFrequency(&freq)){
		present=TRUE;
		scalar = 1.f/freq.QuadPart;
	}else{
		present=FALSE;
		scalar = 1.f/1000;
	}
}

float get_time(){
	__int64 time;
	if(present){
		LARGE_INTEGER li_time;
		QueryPerformanceCounter(&li_time);
		time = li_time.QuadPart;
	}else{
		time = GetTickCount();
	}

	return (float)time*scalar;
}