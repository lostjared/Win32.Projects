#pragma once // include the header file only once
#define WIN32_LEAN_AND_MEAN // include only whats nessicary to make a windows program
#include<windows.h>// include windows.h
#include "mxsocket.h"// include the socket header file
#include "tokens.h"

// the pixel structure
struct Pixel {
	int x,y;// x,y variables
	COLORREF color;// color of the pixel
	bool on;// is this pixel drawn yet?
	Pixel() {
		memset(this,0,sizeof(this));// set all the data to 0
	}
};

Pixel pixels[640][480];// pixel array 640x480


