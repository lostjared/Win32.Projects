// main source
// written by jared bruni
// for the ultimate mma simulator
// www.lostsidedead.com/fight/


#include "thehead.h"

WNDPROC mainProc;
HINSTANCE hInst;
HWND mainwindow;
HWND newwindow;
HFONT thefont =  CreateFont(14,0,0,0,FW_DONTCARE,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_MODERN,"Arial");
HWND  hcombo;
HWND  hbok;


inline void Init();
inline void MenuProc(WPARAM wParam);
void UpdateTitle();
void SplashScreen();


Fighter fighter;
char filename[150];

HWND player_list;//listbox
HWND player_change;//exitbox
HWND player_item_static; // player item static
HWND player_item; // player itme
HWND button_up; // scroll up
HWND button_down; // scroll down
HWND button_save; // save
FighterListHandler FighterList;

// enumerated constants
enum { ID_LIST = 1,ID_EDIT,ID_UP,ID_DOWN,ID_SAVE };

// splash screen
void SplashScreen()
{
	HBITMAP hbm;
   	hbm = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));
	HWND stx;
	stx = CreateWindowEx(WS_EX_TOPMOST,"Static",NULL,WS_VISIBLE|WS_POPUP|SS_BITMAP,200,200,0,0,0,0,hInst,0);
	SendMessage(stx,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(HBITMAP)hbm);
	ShowWindow(stx,SW_SHOW);
	UpdateWindow(stx);
	pause(2);// pause for 2 seconds
	ShowWindow(stx,SW_HIDE);
	DestroyWindow(stx);
	DeleteObject(hbm);
}
// winmain entry
int APIENTRY WinMain(HINSTANCE hInstx,HINSTANCE hPrev,LPSTR line,int CmdShow)
{
	
	hInst = hInstx;
	
	SplashScreen();
	
	MSG msg;
	Init();
	strcpy(filename,"Untitled.mma");
	UpdateTitle();
	
	
	
	
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}

// init application
inline void Init()
{
	// init
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.lpfnWndProc = (WNDPROC) MainProc;
	wc.lpszClassName = "Master";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	
	RegisterClass(&wc);

	wc.lpszMenuName = NULL;
	wc.lpszClassName = "MasterNew";
	wc.lpfnWndProc = (WNDPROC) NewProc;

	RegisterClass(&wc);
	
	mainwindow = CreateWindow("Master","Ultimate MMA Simulator Character Editor",WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,CW_USEDEFAULT,CW_USEDEFAULT,640-75,290,0,0,hInst,0);
	newwindow = CreateWindowEx(WS_EX_TOPMOST,"MasterNew","Create a New Character",WS_OVERLAPPED ,100,100,200,90,0,0,hInst,0);

	ShowWindow(newwindow,SW_SHOW);
	UpdateWindow(newwindow);

}

// mainwindow callback function
LRESULT APIENTRY MainProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		{
			if(MessageBox(hwnd,"Do you wish to save before exit?","Save?",MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				MenuProc(ID_FILESAVE);
			}
			
			SendMessage(hwnd,WM_DESTROY,0,0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		{
			HWND list_static;
			list_static = CreateWindow("Static","Player data",WS_CHILD|WS_VISIBLE,5,5,200,20,hwnd,0,hInst,0);
			SetMyFont(list_static);
			player_list = CreateWindowEx(WS_EX_STATICEDGE,"ListBox",NULL,WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|LBS_NOTIFY,5,25,625-75,200,hwnd,(HMENU)ID_LIST,hInst,0);
			SetMyFont(player_list);
			player_item_static = CreateWindow("Static","name",WS_CHILD | WS_VISIBLE,5,5+5+5+200+5,100,25,hwnd,0,hInst,0);
			SetMyFont(player_item_static); 
			player_item = CreateWindowEx(WS_EX_STATICEDGE,"Edit","",WS_CHILD|WS_VISIBLE|ES_MULTILINE|WS_VSCROLL|WS_BORDER,115,15+200,200,25,hwnd,(HMENU)ID_EDIT,hInst,0);
			SetMyFont(player_item);
			button_up = CreateWindowEx(WS_EX_STATICEDGE,"Button","Move up",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,115+200+5,15+200,75,25,hwnd,(HMENU)ID_UP,hInst,0);
			SetMyFont(player_item);
			button_down = CreateWindowEx(WS_EX_STATICEDGE,"Button","Move down",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,115+200+5+75+5,15+200,75,25,hwnd,(HMENU)ID_DOWN,hInst,0);
			SetMyFont(button_down);
			button_save = CreateWindowEx(WS_EX_STATICEDGE,"Button","Save",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,115+200+5+75+5+75+5,15+200,75,25,hwnd,(HMENU)ID_SAVE,hInst,0);
			SetMyFont(button_save);
			SetMyFont(button_up);
			FighterList.loadfightlist();
			mainProc = (WNDPROC)SetWindowLong(player_item,GWL_WNDPROC,(LPARAM)(WNDPROC)EditProc);
			SendMessage(player_list,LB_SETCURSEL,0,0);
			SetFocus(player_item);
		}
		break;
	case WM_COMMAND:
		{
			MenuProc(wParam);
			
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					switch(LOWORD(wParam))
					{
					case ID_LIST:
						{
							int current;
							current = SendMessage(player_list,LB_GETCURSEL,0,0);
							FighterList.setcurrent(current);
						}
						break;
					}
				}
				break;
			case BN_CLICKED:
				{
					switch(LOWORD(wParam))
					{
					case ID_UP:
						FighterList.scroll_up();
						break;
					case ID_DOWN:
						FighterList.scroll_down();
						break;
					case ID_SAVE:
						MenuProc(ID_FILESAVE);
						break;
					}
				}
				break;
			}
		}
		break;
	default: return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	
	return 0;
}
// subclassed edit callback
LRESULT APIENTRY EditProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYDOWN:
		{
			
			switch(wParam)
			{
			case VK_RETURN:
				{
					FighterList.scroll_down();
				}
				break;
			}
		}
		break;
		
	default: return CallWindowProc(mainProc,hwnd,msg,wParam,lParam);
	}
	return 0;
}

// create a new fighter proccess
LRESULT APIENTRY NewProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg) 
	{
	case WM_CLOSE:
		break;
	case WM_CREATE:
		{
			 hcombo = CreateWindow("ComboBox",NULL,WS_BORDER|CBS_DISABLENOSCROLL|WS_CHILD|CBS_DROPDOWNLIST|WS_VISIBLE,5,5,200-15,100,hwnd,NULL,hInst,0); 
			 SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Blank Character");
			 SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"Striker");
			 SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"GNPer");
			 SendMessage(hcombo,CB_ADDSTRING,255,(LPARAM)(LPCSTR)"BJJer");
			 SetMyFont(hcombo);
			 hbok = CreateWindow("Button","Create",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,200-15-75+5,25+10,75,25,hwnd,(HMENU)1,hInst,0);
			 SetMyFont(hbok);
			 HWND mst;
			 mst = CreateWindow("Static","Select Type",WS_CHILD | WS_VISIBLE,5+5,25+10+5,100,20,hwnd,0,hInst,0);
			 SetMyFont(mst);
			 SendMessage(hcombo,CB_SETCURSEL,0,0);
		}
		break;
	case WM_COMMAND:
		{
			switch(HIWORD(wParam))
			{
			case BN_CLICKED:
				{
					switch(LOWORD(wParam))
					{
					case 1:
						{
							ZeroMemory(&fighter,sizeof(fighter));

							int cursel;
							cursel = SendMessage(hcombo,CB_GETCURSEL,0,0);
							switch(cursel)
							{
							case 0:
								fighter.Clear();
								break;
							case 1:
								fighter.SetTemplate(STRIKER);
								break;
							case 2:
								fighter.SetTemplate(GNPER);
								break;
							case 3:
								fighter.SetTemplate(BJJER);
								break;

							}

							strcpy(filename,"Untitled.mma");

							ShowWindow(hwnd,SW_HIDE);
							ShowWindow(mainwindow,SW_SHOW);
							UpdateWindow(mainwindow);
						}
						break;
					}
				}
				break;
			}
		}
		break;
	default: return DefWindowProc(hwnd,msg,wParam,lParam);
	}

	return 0;
}

// menu proccess
inline void MenuProc(WPARAM wParam)
{
	switch(wParam)
	{
	case ID_FILENEW:
		ShowWindow(newwindow,SW_SHOW);
		break;
	case ID_FILESAVE: 
		{
			
			if(strstr(filename,"Untitled.mma"))
			{
				MenuProc(ID_FILESAVEAS);
				return;
			}
			
			
			ofstream fout(filename,ios::binary);
			
			fout.write((char*)&fighter,sizeof(fighter));
			
			fout.close();
			
			UpdateTitle();
		}
		break;
	case ID_FILESAVEAS:
		
		char savename[100];
		if(savefile(savename))
		{
			strcpy(filename,savename);
			if(strstr(filename,".mma"))
			{return;
			} else {
				
				strcat(filename,".mma");
			}
			
			MenuProc(ID_FILESAVE);
			return;
			
		}
		
		break;
	case ID_FILELOAD:
		
		char files[100];
		if(openfile(files))
		{
			strcpy(filename,files);
			ZeroMemory(&fighter,sizeof(fighter));
			
			SendMessage(player_list,LB_SETCURSEL,0,0);
			
			ifstream fin(filename,ios::binary);
			
			fin.read((char*)&fighter,sizeof(fighter));
			
			fin.close();
			
			SendMessage(player_item,WM_SETTEXT,100,(LPARAM)(LPCSTR)fighter.namex);
			
			UpdateTitle();
		}
		
		break;
	case ID_FILEEXIT:
		SendMessage(mainwindow,WM_CLOSE,0,0);
		break;
	case ID_EDITCLEAR:
		{
			if(MessageBox(0,"Do you wish to clear all fields?","Well..?",MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				fighter.Clear();
				FighterList.setcurrent(0);
				SendMessage(player_list,LB_SETCURSEL,0,0);
			}
		}
		break;
	case ID_HELPONLINE:
		{
			ShellExecute(0,"open","http://www.lostsidedead.com/fight/",NULL,NULL,SW_SHOW);
		}
		break;
	case ID_HELPABOUT:
		{
			// Shell About
			ShowAbout("Ultimate MMA Character Editor","Ultimate MMA Character Editor 1.0\r\nWritten by Jared Bruni\r\nwww.lostsidedead.com\r\nsupport me, by supporting my sponsors!");
		}
		break;
	}
}

// update the title
void UpdateTitle()
{
	char buffer[100];
	strcpy(buffer,"Ultimate MMA Simulator Character Editor - ");
	strcat(buffer,filename);
	SendMessage(mainwindow,WM_SETTEXT,strlen(buffer),(LPARAM)(LPCSTR)buffer);
}



