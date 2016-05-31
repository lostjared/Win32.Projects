/* SuperMaster LE ( Level Editor )
   written by Jared Bruni
   www.LostSideDead.com

  "Open Source, Open Mind"

*/

#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include<commdlg.h>
#include<stdio.h>
#include<fstream.h>
#include"resource.h"


const int MAX_TILE = 8000;
const int ID_COMBO = 4;
const int ID_COMBO2 = 5;
const int ID_COMBO3 = 6;

/***************************** game Level structure *********************************/
struct Level
{
	struct Tile
	{
		int block;
		bool solid;
	};


	char level_name[255];
	int grandma[50]; 
	int start_pos;
	Tile tiles[MAX_TILE];

	inline Level()
	{
		strcpy(level_name,"null");
		for(int i = 0; i < MAX_TILE; i++)
		{
			tiles[i].block = 0;
			tiles[i].solid = false;
		}
	}
};

Level level;
   
/******************* function prototypes ****************************************/
LRESULT APIENTRY MainProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
LRESULT APIENTRY AboutProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
LRESULT APIENTRY HelpProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
LRESULT APIENTRY MapProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);


inline void Init();
inline WPARAM Loop();
inline void LoadGraphics();
inline void DestroyGraphics();
void SetImage(HWND hwnd, int sprite);
void ReloadGrid();
void UpdateTitle();
void MenuProc(WPARAM wParam);
void MapClear();
bool openfile(char*);
bool savefile(char*);
int  getblockbypos(int x, int y);
void ScrollLeft();
void ScrollRight();
int getoffgrandma();
void ShowMap();
void HideMap();


/************** global variables *************************************************/
int off_set = 0,i,z;
HWND main, grid[1240],bleft,bright,hcombo,hcombo2,hcombo3,hstatic,about,help,map,mapedit,mapok;
HBITMAP images[15],hero,grandma;
char filename[255];
int cur_block = 0;
bool cur_solid = false;
int cur_tool = 0,cur_object = 0; 
HBITMAP sidelogo,aboutlogo;


/************************ function bodys *****************************************/
int APIENTRY WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR line,int CmdShow)
{
	Init();
	return (Loop());
}


inline void Init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra  = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(GetSysColor(COLOR_BTNFACE)); 
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1));
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = (WNDPROC) MainProc;
	wc.lpszClassName = "SuperMasterLE";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wc);

	wc.lpszMenuName = NULL;
	wc.lpszClassName = "MasterAbout";
	wc.lpfnWndProc = (WNDPROC) AboutProc;

	RegisterClassEx(&wc);

	wc.lpszClassName = "MasterHelp";
	wc.lpfnWndProc = (WNDPROC) HelpProc;

	RegisterClassEx(&wc);

	wc.lpszClassName = "MasterMap";
	wc.lpfnWndProc = (WNDPROC) MapProc;

	RegisterClassEx(&wc);

	LoadGraphics();

	main = CreateWindowEx(WS_EX_TOPMOST,"SuperMasterLE","Super MasterLE",WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,0,0,715,440,0,0,GetModuleHandle(NULL),0);
	about = CreateWindowEx(WS_EX_TOPMOST,"MasterAbout","About SuperMasterLE",WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,CW_USEDEFAULT,CW_USEDEFAULT,290,290,0,0,GetModuleHandle(NULL),0);
	help = CreateWindowEx(WS_EX_TOPMOST,"MasterHelp", "How to use",WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,CW_USEDEFAULT,CW_USEDEFAULT,300,300,0,0,GetModuleHandle(NULL),0);
	map = CreateWindowEx(WS_EX_TOPMOST,"MasterMap","Map Properties",WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,CW_USEDEFAULT,CW_USEDEFAULT,300,110,0,0,GetModuleHandle(NULL),0);


	MapClear();
 
	ShowWindow(main,SW_SHOW);
	UpdateWindow(main);
	ReloadGrid();
	UpdateTitle();
}

inline WPARAM Loop()
{
	MSG msg;
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

inline void LoadGraphics()
{
	images[0] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP1));
	images[1] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP2));
	images[2] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP3));
	images[3] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP4));
	images[4] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP5));
	images[5] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP6));
	images[6] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP7));
	images[7] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP8));
	images[8] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP9));
	images[9] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP10));
	images[10] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP11));
	images[11] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP12));
	images[12] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP13));
	images[13] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP14));
	images[14] = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP15));
	hero = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP16));
	grandma = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP17));
	sidelogo = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP18));
	aboutlogo = LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDB_BITMAP19));

}

inline void DestroyGraphics()
{
	for(int i = 0; i < 14; i++)
	{
		DeleteObject(images[i]);
	}

	DeleteObject(hero);
	DeleteObject(grandma);
	DeleteObject(sidelogo);
	DeleteObject(aboutlogo);
}

void SetImage(HWND hwnd, int sprite)
{
	if(sprite < 15)
	{
		SendMessage(hwnd,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(HBITMAP)images[sprite]);
	}
}

void ReloadGrid()
{
	for(i = 0; i < 700-4+24; i++)
	{
		if(level.start_pos == i+off_set)
		{
			SendMessage(grid[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(HBITMAP)hero);
			continue;
		}

		bool ok = true;

		for(z = 0; z < 50; z++)
		{
			if(level.grandma[z] == i+off_set && level.grandma[z] != 0)
			{
				SendMessage(grid[i],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(HBITMAP)grandma);
				ok = false;
				break;
			}
		}
	
		if(ok == true)
		{

		SetImage(grid[i],level.tiles[i+off_set].block);

		}
		
	}


}

void MapClear()
{
	for(i = 0; i < MAX_TILE; i++)
	{
		level.tiles[i].block = 0;
		level.tiles[i].solid = false;
	}

	for(i = 0; i < 50; i++)
	{
		level.grandma[i] = 0;
	}

	level.start_pos = 0;

	strcpy(filename,"Untitled.sml");
	strcpy(level.level_name,"Unknown Level");
}


void UpdateTitle()
{
	char title[355];
	sprintf(title,"SuperMasterLE - %s",filename);
	SendMessage(main,WM_SETTEXT,strlen(title),(LPARAM)(LPCSTR)title);
}


int  getblockbypos(int x, int y)
{
	int bx,by;
	bx = 5; by = 5;
	int gcount = 0;
	
	for(i = 0; i < 700-4+24; i++)
	{
		
 	if(x > bx && x < bx+16 && y > by && y < by + 16)
	{
		return i;
	}
		
		by = by + 16;
		gcount++;
		if(gcount > 23)
		{
			gcount = 0;
			by = 5;
			bx = bx + 16;
		}		
	}
	
	return -1;
}

void ScrollLeft()
{
	if(off_set > 0)
	{
		off_set -= 24;
		ReloadGrid();
	}


}

void ScrollRight()
{
	if(off_set < MAX_TILE)
	{
		off_set += 24;
		ReloadGrid();
	}
}


int getoffgrandma()
{
	for(i = 0; i < MAX_TILE; i++)
	{
		if(level.grandma[i] == 0)
		{
			return i;
		}
	}
	return -1; // fail
}

void ShowMap()
{
	SendMessage(mapedit,WM_SETTEXT,strlen(level.level_name),(LPARAM)(LPCSTR)level.level_name);
	ShowWindow(map,SW_SHOW);
}

void HideMap()
{
	SendMessage(mapedit,WM_GETTEXT,255,(LPARAM)(LPCSTR)level.level_name);
	ShowWindow(map,SW_HIDE);
}


LRESULT APIENTRY MainProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		DestroyGraphics();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		{
			int bx,by;
			bx = 5; by = 5;
			int gcount = 0;
			
			for(i = 0; i < 700-4+24; i++)
			{
				
				grid[i] = CreateWindow("Static",NULL, WS_CHILD | WS_VISIBLE |SS_BITMAP,bx,by,16,16,hwnd,0,GetModuleHandle(NULL),0);			
				by = by + 16;
				gcount++;
				if(gcount > 23)
				{
					gcount = 0;
					by = 5;
					bx = bx + 16;
				}
				
			}

			bleft = CreateWindow("Button","Scroll Left",WS_CHILD | WS_VISIBLE | WS_BORDER ,495,5,100,25,hwnd,(HMENU)1,GetModuleHandle(NULL),0);
			bright = CreateWindow("Button","Scroll Right",WS_CHILD | WS_VISIBLE | WS_BORDER ,495+100+5,5,100,25,hwnd,(HMENU)2,GetModuleHandle(NULL),0);
			hcombo = CreateWindow("ComboBox",NULL,WS_VSCROLL | WS_BORDER|CBS_DISABLENOSCROLL|WS_CHILD|CBS_DROPDOWNLIST|WS_VISIBLE,495,35,150,300,hwnd,(HMENU)ID_COMBO,GetModuleHandle(NULL),0); 
            HWND st;
			st = CreateWindow("Static","Draw Tools",WS_CHILD | WS_VISIBLE ,495,60,100,20,hwnd,0,GetModuleHandle(NULL),0);
			hcombo2 = CreateWindow("ComboBox",NULL,WS_VSCROLL | WS_BORDER|CBS_DISABLENOSCROLL|WS_CHILD|CBS_DROPDOWNLIST|WS_VISIBLE,495,80,205,100,hwnd,(HMENU)ID_COMBO2,GetModuleHandle(NULL),0); 
			hstatic = CreateWindow("Static",NULL, WS_CHILD | WS_VISIBLE |SS_BITMAP,495+150+5,38,16,16,hwnd,0,GetModuleHandle(NULL),0);
			HWND st2;
			st2 = CreateWindow("Static","Objects",WS_CHILD | WS_VISIBLE ,495,80+30,100,20,hwnd,0,GetModuleHandle(NULL),0);
			hcombo3 = CreateWindow("ComboBox",NULL,WS_VSCROLL | WS_BORDER|CBS_DISABLENOSCROLL|WS_CHILD|CBS_DROPDOWNLIST|WS_VISIBLE,495,80+50,205,100,hwnd,(HMENU)ID_COMBO3,GetModuleHandle(NULL),0); 
			HWND st3;
			st3 = CreateWindow("Static",NULL,WS_CHILD | WS_VISIBLE | SS_BITMAP,495,80+50+30,10,10,hwnd,0,GetModuleHandle(NULL),0);
			SendMessage(st3,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(HBITMAP)sidelogo);

			SendMessage(hstatic,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(HBITMAP)images[0]);
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Black sky");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Grass");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Blue Brick");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Blue Sky");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Brick");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Werid Brick");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Red Block");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Sand 1");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Sand 2");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Snow");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Stone 1");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Stone 2");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Stone 3");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Stone 4");
			SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Level Exit Block");
			SendMessage(hcombo,CB_SETCURSEL,0,0);

			SendMessage(hcombo2,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Pencil");
			SendMessage(hcombo2,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Square");
			SendMessage(hcombo2,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Fill");
			SendMessage(hcombo2,CB_SETCURSEL,0,0);

			SendMessage(hcombo3,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Master");
			SendMessage(hcombo3,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Grandma");
			SendMessage(hcombo3,CB_SETCURSEL,0,0);

		}
		break;
	case WM_COMMAND:
		{
			MenuProc(wParam);

			switch(HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				{
					
					switch(LOWORD(wParam))
					{
					case ID_COMBO:
						{
							
							int sel;
							sel = SendMessage(hcombo,CB_GETCURSEL,0,0);
							SendMessage(hstatic,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(HBITMAP)images[sel]);
							cur_block = sel;
							if(sel == 0 || sel == 3 || sel == 14 )
							{
								cur_solid = false;
							}
							else
							{
								cur_solid = true;
							}
							
						}
						break;
					case ID_COMBO2:
						{
							cur_tool = SendMessage(hcombo2,CB_GETCURSEL,0,0);
						}
						break;
					case ID_COMBO3:
						{
							cur_object = SendMessage(hcombo3,CB_GETCURSEL,0,0);
						}
						break;
						
					}
									
				}
				break;
			case BN_CLICKED:
				{
					switch(LOWORD(wParam))
					{
					case 1:
						{
							ScrollLeft();
						}
						break;
					case 2:
						{
							ScrollRight();
						}
						break;
					}

				}
				break;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			int mx;
			int my;
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			int pos;
			pos = getblockbypos(mx,my);
			if(pos != -1)
			{
				if(wParam & MK_LBUTTON)
				{
					switch(cur_tool)
					{
					case 0://pencil
						{

					level.tiles[off_set+pos].block = cur_block;
					level.tiles[off_set+pos].solid = cur_solid;
					SetImage(grid[pos],cur_block);

						}
						break;
					case 1://square
						{
							level.tiles[off_set+pos].block = cur_block;
							level.tiles[off_set+pos].solid = cur_solid;

							level.tiles[off_set+pos+1].block = cur_block;
							level.tiles[off_set+pos+1].solid = cur_solid;

							level.tiles[off_set+pos+24].block = cur_block;
							level.tiles[off_set+pos+24].solid = cur_solid;

							level.tiles[off_set+pos+25].block = cur_block;
							level.tiles[off_set+pos+25].solid = cur_solid;

							SetImage(grid[pos],cur_block);
							SetImage(grid[pos+1],cur_block);
							SetImage(grid[pos+24],cur_block);
							SetImage(grid[pos+25],cur_block);

						}
						break;
					case 2://fill
						{
							for(i = 0; i < 700-4+24; i++)
							{
								level.tiles[i+off_set].block = cur_block;
								level.tiles[i+off_set].solid = cur_solid;
								//SetImage(grid[i],cur_block);
							}

							ReloadGrid();
						}
						break;
					}
				}
			}
		}
		break;
		// placing bricks
	case WM_LBUTTONDOWN:
		{
			if(cur_tool == 2)
			{
				for(i = 0; i < 700-4+24; i++)
				{
					level.tiles[i+off_set].block = cur_block;
					level.tiles[i+off_set].solid = cur_solid;
					SetImage(grid[i],cur_block);
				}
			}
		}
		break;
		// placing objects
	case WM_RBUTTONDOWN:
		{
			int mx;
			int my;
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			
			int pos;
			pos = getblockbypos(mx,my);
			if(pos != -1)
			{
				
				// place the hero, hes only allowed 1 spot
				if(cur_object == 0)
				{
					level.start_pos = pos+off_set;
					ReloadGrid();
				}
				else
				{
					for(i = 0; i < 50; i++)
					{
						if(level.grandma[i] == pos+off_set)
						{
							level.grandma[i] = 0;
							ReloadGrid();
							return 0;
						}
					}

					int off;
					off = getoffgrandma();
					if(off != -1)
					{

					level.grandma[off] = pos+off_set;
					SendMessage(grid[pos],STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(HBITMAP)grandma);

					}
				}
			}
			
		}
		break;
	default: return DefWindowProc(hwnd,msg,wParam,lParam);
	}

	return (0);
}


void MenuProc(WPARAM wParam)
{
	switch(wParam)
	{
	case ID_FILENEW:
		{
			if(MessageBox(GetForegroundWindow(),"Are you sure you wish to create a new map?","Well..?",MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				MapClear();
				UpdateTitle();
				ReloadGrid();
			}
		}
		break;
	case ID_FILESAVEAS:
		{
			char tfile[255];
			if(savefile(tfile) == true)
			{
				strcpy(filename,tfile);
				if(strstr(filename,".sml"))
				{
				}
				else
				{
				strcat(filename,".sml");
				}
				MenuProc(ID_FILESAVE);
				return;
			}
		}
		break;
	case ID_FILESAVE:
		{
			if(strcmp(filename,"Untitled.sml") == 0)
			{
				MenuProc(ID_FILESAVEAS);
				return;
			}
			else
			{
				ofstream fout(filename,ios::binary);
				fout.write((char*)&level,sizeof(level));
				fout.close();
				UpdateTitle();
			}
		}
		break;
	case ID_FILEOPEN:
		{
			char tfile[255];
			if(openfile(tfile) == true)
			{
				strcpy(filename,tfile);
				ifstream fin(filename,ios::binary);
				fin.read((char*)&level,sizeof(level));
				fin.close();
				off_set = 0;
				ReloadGrid();
				UpdateTitle();		
			}
		}
		break;
	case ID_FILEEXIT:
		{
			SendMessage(main,WM_CLOSE,0,0);
		}
		break;
	case ID_LEFT:
		{
			ScrollLeft();
		}
		break;
	case ID_RIGHT:
		{
			ScrollRight();
		}
		break;
	case ID_CLEAR:
		{
			if(MessageBox(GetForegroundWindow(),"Are you sure you wish to clear this map?","Clear Map?", MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				for(i = 0; i < MAX_TILE; i++)
				{
					level.tiles[i].block = 0;
					level.tiles[i].solid = false;
				}

				for(i = 0; i < 50; i++)
				{
				   level.grandma[i] = 0;
				}

				ReloadGrid();
			}
		}
		break;
	case ID_ABOUT:
		{
			ShowWindow(about,SW_SHOW);
			UpdateWindow(about);
		}
		break;
	case ID_HOW:
		{
			ShowWindow(help,SW_SHOW);
			UpdateWindow(help);
		}
		break;
	case ID_PROP:
		{
			ShowMap();
		}
		break;
	}
}
/*********************************************** about window ****************************/
LRESULT APIENTRY AboutProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		ShowWindow(hwnd,SW_HIDE);
		break;
	case WM_CREATE:
		{
			HWND istat;
			istat = CreateWindow("Static",NULL,WS_CHILD | WS_VISIBLE | SS_BITMAP,0,0,100,100,hwnd,0,GetModuleHandle(NULL),0);
			SendMessage(istat,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(HBITMAP)aboutlogo);
			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			ShowWindow(hwnd,SW_HIDE);
		}
		break;
	default: return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	
	return (0);
}

/************************************map properties proc **********************************/

LRESULT APIENTRY MapProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		HideMap();
		break;
	case WM_CREATE:
		{
			HWND st;
			st = CreateWindow("Static","Map Name",WS_CHILD | WS_VISIBLE,5,5,100,20,hwnd,0,GetModuleHandle(NULL),0);
			mapedit = CreateWindowEx(WS_EX_STATICEDGE,"Edit","Unknown Map",WS_CHILD | WS_VISIBLE | WS_BORDER ,5,30,285,20,hwnd,0,GetModuleHandle(NULL),0);
			mapok = CreateWindow("Button","Ok",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,295-80,55,75,25,hwnd,0,GetModuleHandle(NULL),0);
		}
		break;
	case WM_COMMAND:
		{
			switch(HIWORD(wParam))
			{
			case BN_CLICKED:
				{
					HideMap();
				}
				break;
			}
		}
		break;

	default: return DefWindowProc(hwnd,msg,wParam,lParam);
	}

	return (0);
}

/*********************************** How to Use Window ************************************/
LRESULT APIENTRY HelpProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{

	switch(msg)
	{
	case WM_CLOSE:
		ShowWindow(hwnd,SW_HIDE);
		break;
	case WM_CREATE:
		{
			HWND hedit;
			hedit = CreateWindowEx(WS_EX_STATICEDGE,"Edit","SuperMaster Level Editor\r\nWritten by Jared Bruni\r\n Open Source , Open Mind\r\n www.LostSideDead.com\r\nHow to use\r\n Select your Tool, and your Brick\r\n Then use the Left Mouse button to draw on the grid\r\nUse the Right Mouse Button to place the grandma/hero\r\nright click on a already placed grandma and she will be removed\r\npretty simple\r\n :)\r\n (-<)", ES_MULTILINE | ES_READONLY |  WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER ,5,5,285,265,hwnd,0,GetModuleHandle(NULL),0);
			
		}
		break;
	default: return DefWindowProc(hwnd,msg,wParam,lParam);
	}

	return (0);
}

/*********************************** common dialog box ************************************/



bool openfile(char* cFile)
{
	char szFile[200];
	
	
	OPENFILENAME ofn;       // common dialog box structure
	
	strcpy(szFile, "*.sml");
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = main;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "SuperMasterLevel\0*.sml\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	
	if (GetOpenFileName(&ofn)==TRUE) 
	{
		strcpy(cFile, ofn.lpstrFile);
		return true;
	}
	else
	{
		return false;
	}
	
}

bool savefile(char* cFile)
{
	
	char szFile[200];
	
	
	OPENFILENAME ofn;       // common dialog box structure
	
	strcpy(szFile, "*.sml");
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = main;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "SuperMasterLevel\0*.sml\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	
	if (GetSaveFileName(&ofn)==TRUE) 
	{
		strcpy(cFile, ofn.lpstrFile);
		return true;
	}
	else
	{
		return false;
	}
	
	
}

/************************************************ lostsidedead.com **********************/