// this header file, supports my cause
// that is used for when I want my about box

#include <windows.h>
#include "resource.h"

void SupportJared();
void InitHelp();
void ShowAbout(char* title,char* aboutstring);
void CloseAbout();
LRESULT APIENTRY AboutProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
HWND aboutwin;
void LOADG();
void KILLG();
inline HFONT MakeFont(char* ftype, int size);
void SetMyFont(HWND,HFONT);
void SupportLostSideDead();


HBITMAP abmp;
HFONT   afont = MakeFont("Arial",14);
HWND editwin;
HWND hok;

void SetMyFont(HWND hwnd,HFONT font)
{
	SendMessage(hwnd,WM_SETFONT,(WPARAM)(HFONT)font,0);
}


// shell in IE
void SupportJared()
{
	ShellExecute(NULL,"open","http://www.lostsidedead.com",NULL,NULL,SW_SHOW);
}
// shell in IE
void SupportLostSideDead()
{
	ShellExecute(NULL,"open","http://www.lostsidedead.com",NULL,NULL,SW_SHOW);
}


// init help
void InitHelp()
{
	LOADG();

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL,NULL);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.lpfnWndProc = (WNDPROC) AboutProc;
	wc.lpszClassName = "AboutMaster";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);// register it

	aboutwin = CreateWindow("AboutMaster","About This Program (Jared Bruni)",WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,CW_USEDEFAULT,CW_USEDEFAULT,300,300-10,0,0,GetModuleHandle(NULL),0);
}


// about processs
LRESULT APIENTRY AboutProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		ShowWindow(hwnd,SW_HIDE);
		break;
	case WM_DESTROY:
		KILLG();
		break;
	case WM_CREATE:
		{
			HWND st;
			st = CreateWindow("Static","Please Support me, by Visitng my Sponsor",WS_CHILD |WS_VISIBLE,5,5,300,25,hwnd,0,GetModuleHandle(NULL),0);
			SetMyFont(st,afont);
			HWND st2;
			st2 = CreateWindow("Static",NULL,WS_CHILD | WS_VISIBLE | SS_BITMAP,5,25,0,0,hwnd,0,GetModuleHandle(NULL),0);
			SendMessage(st2,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(HBITMAP)abmp);
			HWND st3;
			st3 = CreateWindow("Static","Written by Jared Bruni",WS_CHILD | WS_VISIBLE,120,35,200,20,hwnd,0,GetModuleHandle(NULL),0);
			editwin = CreateWindowEx(WS_EX_STATICEDGE,"Edit",NULL,WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL | ES_READONLY ,5,80,290-5,150,hwnd,0,GetModuleHandle(NULL),0);
			hok = CreateWindowEx(WS_EX_STATICEDGE,"Button","Ok!",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,285-75,80+155,75,25,hwnd,0,GetModuleHandle(NULL),0);
			SetMyFont(hok,afont);
			SetMyFont(editwin,afont);
			HWND st4;
			st4 = CreateWindow("Static","www.lostsidedead.com",WS_CHILD | WS_VISIBLE,5,80+155,200,25,hwnd,0,GetModuleHandle(NULL),0);

		}
		break;
	case WM_COMMAND:
		{
			switch(HIWORD(wParam))
			{
			case BN_CLICKED:
				ShowWindow(hwnd,SW_HIDE);
				break;
			}

		}
		break;
	case WM_LBUTTONDOWN:
		{
			int mx;
			int my;
			mx = LOWORD(lParam);
			my = HIWORD(lParam);

			if(mx > 5 && mx < 5+101 && my > 25 && my < 75)
			{
				SupportJared();
			}

			if(mx > 5 && mx < 100 && my > 80+155 && my < 80+155+25)
			{
				SupportLostSideDead();
			}
		}
		break;
	default: return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	return 0;
}

struct AutoInit
{
	inline AutoInit()
	{
		InitHelp();
	}
};

static AutoInit autoit;

void ShowAbout(char* title,char* aboutstring)
{
	char tbuff[100];
	strcpy(tbuff,"About ");
	strcat(tbuff,title);

	SendMessage(aboutwin,WM_SETTEXT,strlen(title),(LPARAM)(LPCSTR)tbuff);
	SendMessage(editwin,WM_SETTEXT,strlen(aboutstring),(LPARAM)(LPCSTR)aboutstring);
	ShowWindow(aboutwin,SW_SHOW);
}

void CloseAbout()
{
	ShowWindow(aboutwin,SW_HIDE);
}

void LOADG()
{
	abmp = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP2));
}

void KILLG()
{
	DeleteObject(abmp);
}

// make a font
inline HFONT MakeFont(char* ftype, int size)
{
	return CreateFont(size,0,0,0,FW_DONTCARE,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,ftype);
}
