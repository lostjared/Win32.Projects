// thehead 
// written by jared bruni
// www.lostsidedead.com

#include <windows.h>
#include <commdlg.h>
#include <fstream.h>
#include <ctime>
#include "supportj.h"

LRESULT APIENTRY MainProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
LRESULT APIENTRY EditProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
LRESULT APIENTRY NewProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool openfile(char*);
bool savefile(char*);
void pause(long seconds);

enum FTEMPLATE { STRIKER , GNPER , BJJER };
extern HWND mainwindow;
extern HINSTANCE hInst;
// handle for a font
extern HFONT thefont;
extern HWND player_item;
int pre_current = -1;


struct Fighter
{
	// character data
	char namex[100];
	char nickx[100];
	double height;
	double weight;
	int fight_freq; // fight frequency ?
    ////////////////////////////////////
	double strength;
	double speed;
	double conditioning;
	double legstrike;
	double armstrike;
	double groundstrike;
	double submissions;
	double groundfighting;
	double takedowns;
	double standupdefense;
	double grounddefense;
	double takedowndefense;
	double aggression;
	double intelligence;
	double toughness;
	double clinching;
	double overall;

	// Basic movement %
    double arm_strike;
	double takedown_attempt;
	double clinch;
	double rest;
	double combostrike;
	double fightdirty;
	

	inline Fighter()
	{
		Clear();
	}
	// get the average
	inline void GetAverage()
	{
		
		overall = (strength + speed + conditioning + legstrike + armstrike + groundstrike + submissions 
			 + groundfighting + takedowns + standupdefense + grounddefense + takedowndefense + aggression
			 + intelligence + toughness + clinching) / 16;
	}

	inline void SetTemplate(FTEMPLATE type)
	{
		switch(type)
		{
		case STRIKER:
			strength = 7;
			speed = 8;
			conditioning = 7;
			legstrike = 8;
			armstrike = 8;
			groundstrike = 7;
			submissions = 6;
			groundfighting = 6;
			takedowns = 6;
			standupdefense = 8;
			grounddefense  = 6;
			takedowndefense = 7;
			aggression = 7;
			intelligence = 8;
			toughness = 8;
			clinching = 6;
			GetAverage();
			break;
		case GNPER:
			strength = 8;
			speed = 7;
			conditioning = 7;
			legstrike = 6;
			armstrike = 7;
			groundstrike = 8;
			submissions = 6;
			groundfighting = 7;
			takedowns = 8;
			standupdefense = 6;
			grounddefense  = 7;
			takedowndefense = 8;
			aggression = 7;
			intelligence = 8;
			toughness = 8;
			clinching = 8;
			GetAverage();
			break;
		case BJJER:
			strength = 6;
			speed = 8;
			conditioning = 9;
			legstrike = 7;
			armstrike = 6;
			groundstrike = 7;
			submissions = 8;
			groundfighting = 8;
			takedowns = 7;
			standupdefense = 6;
			grounddefense  = 8;
			takedowndefense = 7;
			aggression = 6;
			intelligence = 8;
			toughness = 8;
			clinching = 7;
			GetAverage();
			break;
		default: 
			return;
			break;
		}

		return;
	}

	// clear the fighter
	inline void Clear()
	{
		strength = 0;
		speed = 0;
		conditioning = 0;
		legstrike = 0;
		armstrike = 0;
		groundstrike = 0;
		submissions = 0;
		groundfighting = 0;
		takedowns = 0;
		standupdefense = 0;
		grounddefense = 0;
		takedowndefense = 0;
		aggression = 0;
		intelligence = 0;
		toughness = 0;
		clinching = 0;
		overall = 0;
		height = 0;
		weight = 0;
		fight_freq = 0;
		strcpy(namex,"");
		strcpy(nickx,"");
		arm_strike = 0;
		takedown_attempt = 0;
		clinch = 0;
		combostrike = 0;
		fightdirty = 0;
		height = 0;
		weight = 0;
		fight_freq = 0;
	}

	// boy was writing all this fun 
	// the translation from input, into the fighter structure
	inline void setfighterdata(char* tname,char* data)
	{
		if(strstr(tname,"name"))
		{
			strcpy(namex,data);
			return;
		}

		if(strstr(tname,"nick"))
		{
			strcpy(nickx,data);
			return;
		}

		if(strstr(tname,"height"))
		{
			height = atof(data);
			if(height == 0) {
				MessageBox(mainwindow,"Invalid Height","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"weight"))
		{
			weight = atof(data);
			if(weight == 0) {
				MessageBox(mainwindow,"Invalid Weight","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"fight frequency"))
		{
			fight_freq = atoi(data);
			if(fight_freq == 0) {
				MessageBox(mainwindow,"Invalid Frequency","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"strength"))
		{
			strength = atof(data);
			if(strength == 0) {
				MessageBox(mainwindow,"Invalid Strength","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"speed"))
		{
		    speed = atof(data);
			if(speed == 0) {
				MessageBox(mainwindow,"Invalid speed","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"conditioning"))
		{
		    conditioning = atof(data);
			if(conditioning == 0) {
				MessageBox(mainwindow,"Invalid conditiong","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"legs strike"))
		{
		    legstrike = atof(data);
			if(legstrike == 0) {
				MessageBox(mainwindow,"Invalid leg strike value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"arm strike"))
		{
		    armstrike = atof(data);
			if(armstrike == 0) {
				MessageBox(mainwindow,"Invalid arm strike value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}
		
		if(strstr(tname,"ground strike"))
		{
		    groundstrike = atof(data);
			if(groundstrike == 0) {
				MessageBox(mainwindow,"Invalid ground strike value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"submissions"))
		{
		    submissions = atof(data);
			if(submissions == 0) {
				MessageBox(mainwindow,"Invalid submissions value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"ground fighting"))
		{
		    groundfighting = atof(data);
			if(groundfighting == 0) {
				MessageBox(mainwindow,"Invalid ground fighting value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"take downs"))
		{
		    takedowns = atof(data);
			if(takedowns == 0) {
				MessageBox(mainwindow,"Invalid takedowns value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"stand up defense"))
		{
		    standupdefense = atof(data);
			if(standupdefense == 0) {
				MessageBox(mainwindow,"Invalid stand up defense value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"ground defense"))
		{
		    grounddefense = atof(data);
			if(grounddefense == 0) {
				MessageBox(mainwindow,"Invalid ground defense value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"take down defense"))
		{
		    takedowndefense = atof(data);
			if(takedowndefense == 0) {
				MessageBox(mainwindow,"Invalid take down value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		
		if(strstr(tname,"aggression"))
		{
		    aggression = atof(data);
			if(aggression == 0) {
				MessageBox(mainwindow,"Invalid aggression value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"intelligence"))
		{
		    intelligence = atof(data);
			if(intelligence == 0) {
				MessageBox(mainwindow,"Invalid intelligence value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		
		if(strstr(tname,"toughness"))
		{
		    toughness = atof(data);
			if(toughness == 0) {
				MessageBox(mainwindow,"Invalid toughness value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"clinching"))
		{
		    clinching = atof(data);
			if(clinching == 0) {
				MessageBox(mainwindow,"Invalid clinching value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"arms strike %"))
		{
		    arm_strike = atof(data);
			if(arm_strike == 0) {
				MessageBox(mainwindow,"Invalid arm strike value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		
		if(strstr(tname,"take down attempt"))
		{
		    takedown_attempt = atof(data);
			if(takedown_attempt == 0) {
				MessageBox(mainwindow,"Invalid  takedown value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		if(strstr(tname,"clinch"))
		{
		    clinch = atof(data);
			if(clinch == 0) {
				MessageBox(mainwindow,"Invalid  clinch value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		
		if(strstr(tname,"rest"))
		{
		    rest = atof(data);
			if(rest == 0) {
				MessageBox(mainwindow,"Invalid rest value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}
		
		if(strstr(tname,"combo strike"))
		{
		    combostrike = atof(data);
			if(combostrike == 0) {
				MessageBox(mainwindow,"Invalid combostrike value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

		
		if(strstr(tname,"fight dirty"))
		{
		    fightdirty = atof(data);
			if(fightdirty == 0) {
				MessageBox(mainwindow,"Invalid fightdirty value","Invalid Data",MB_OK| MB_ICONERROR);
				return;
			}
		}

	}

	// get the fighter data, (translate from input from the edit, into the doubles)
	inline void getfighterdata(char* tname,char* data)
	{
		if(strstr(tname,"name"))
		{
			strcpy(data,namex);
			return;
		}

		if(strstr(tname,"nick"))
		{
			strcpy(data,nickx);
			return;
		}

		if(strstr(tname,"height"))
		{
			char tdouble[100];
			gcvt(height,10,tdouble);
			strcpy(data,tdouble);
			return;
		}
		
		if(strstr(tname,"weight"))
		{
			char tdouble[100];
			gcvt(weight,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"fight frequency"))
		{
			char tfreq[100];
			itoa(fight_freq,tfreq,10);
			strcpy(data,tfreq);
			return;		
		}

		if(strstr(tname,"strength"))
		{
			char tdouble[100];
			gcvt(strength,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"speed"))
		{
			char tdouble[100];
			gcvt(speed,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"conditioning"))
		{
			char tdouble[100];
			gcvt(conditioning,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"legs strike"))
		{
			char tdouble[100];
			gcvt(legstrike,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"arm strike"))
		{
			char tdouble[100];
			gcvt(armstrike,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"ground strike"))
		{
			char tdouble[100];
			gcvt(groundstrike,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"submissions"))
		{
			char tdouble[100];
			gcvt(submissions,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"ground fighting"))
		{
			char tdouble[100];
			gcvt(groundfighting,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"take downs"))
		{
			char tdouble[100];
			gcvt(takedowns,10,tdouble);
			strcpy(data,tdouble);
			return;
		}
		
		if(strstr(tname,"ground defense"))
		{
			char tdouble[100];
			gcvt(grounddefense,10,tdouble);
			strcpy(data,tdouble);
			return;
		}
			
		if(strstr(tname,"stand up defense"))
		{
			char tdouble[100];
			gcvt(standupdefense,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"take down defense"))
		{
			char tdouble[100];
			gcvt(takedowndefense,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"aggression"))
		{
			char tdouble[100];
			gcvt(aggression,10,tdouble);
			strcpy(data,tdouble);
			return;
		}
		
		if(strstr(tname,"intelligence"))
		{
			char tdouble[100];
			gcvt(intelligence,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		
		if(strstr(tname,"toughness"))
		{
			char tdouble[100];
			gcvt(toughness,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"clinching"))
		{
			char tdouble[100];
			gcvt(clinching,10,tdouble);
			strcpy(data,tdouble);
			return;
		}
		
		if(strstr(tname,"arms strike %"))
		{
			char tdouble[100];
			gcvt(arm_strike,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		if(strstr(tname,"take down attempt"))
		{
			char tdouble[100];
			gcvt(takedown_attempt,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		
		if(strstr(tname,"clinch"))
		{
			char tdouble[100];
			gcvt(clinch,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		
		if(strstr(tname,"rest"))
		{
			char tdouble[100];
			gcvt(rest,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		
		if(strstr(tname,"combo strike"))
		{
			char tdouble[100];
			gcvt(combostrike,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		
		if(strstr(tname,"fight dirty"))
		{
			char tdouble[100];
			gcvt(fightdirty,10,tdouble);
			strcpy(data,tdouble);
			return;
		}

		strcpy(data,""); // null
	}


};


extern Fighter fighter;
extern char filename[150];
extern HWND player_list;//listbox
extern HWND player_item_static; // player item static

void SetMyFont(HWND hwnd);


void SetMyFont(HWND hwnd)
{
	SendMessage(hwnd,WM_SETFONT,(WPARAM)(HFONT)thefont,0);
}



// current list
struct FightList 
{
	char fight_title[200][200];
	int fight_off;

	inline FightList() { fight_off = 0; }

	inline void addnew(char* buff) 
	{                            
		strcpy(fight_title[fight_off],buff);
		fight_off++;
	}

	inline void getindex(int index,char* buff)
	{
		strcpy(buff,fight_title[index]);
	}

	inline void loadtohwnd()
	{
		SendMessage(player_list,LB_RESETCONTENT,0,0);

		for(int i = 0; i < fight_off;i++)
		{
			SendMessage(player_list,LB_ADDSTRING,strlen(fight_title[i]),(LPARAM)(LPCSTR)fight_title[i]);
		}
	}

	inline void getdatabyindex(int index,char* data)
	{
		char curname[100];
		getindex(index,curname);
		fighter.getfighterdata(curname,data);
	}

	inline void setdatabyindex(int index,char* data)
	{
		char curname[100];
		getindex(index,curname);
		fighter.setfighterdata(curname,data);
	}
};


// takes care of the list
class FighterListHandler
{
	
public:

	FightList flist;

	inline void scroll_up() 
	{
		int current;
		current = SendMessage(player_list,LB_GETCURSEL,0,0);
		current--;
		if(current >= 0)
		{
			SendMessage(player_list,LB_SETCURSEL,current,0);
			setcurrent(current);
		}

	}
	
	inline void scroll_down()
	{
		int current;
		current = SendMessage(player_list,LB_GETCURSEL,0,0);
		current++;
		int max;
		max = SendMessage(player_list,LB_GETCOUNT,0,0);

		if(current <= max)
		{
			SendMessage(player_list,LB_SETCURSEL,current,0);
			setcurrent(current);
		}
	}

	inline void savelist() 
	{


	}

	inline void updatelist()
	{

	}

	inline void loadfightlist()
	{
		flist.addnew("name");
		flist.addnew("nick");
		flist.addnew("height");
		flist.addnew("weight");
		flist.addnew("fight frequency");
		flist.addnew("strength");
		flist.addnew("speed");
		flist.addnew("conditioning");
		flist.addnew("legs strike");
		flist.addnew("arm strike");
		flist.addnew("ground strike");
		flist.addnew("submissions");
		flist.addnew("ground fighting");
		flist.addnew("take downs");
		flist.addnew("stand up defense");
		flist.addnew("ground defense");
		flist.addnew("take down defense");
		flist.addnew("aggression");
		flist.addnew("intelligence");
		flist.addnew("toughness");
		flist.addnew("clinching");
		flist.addnew("arms strike %");
		flist.addnew("take down attempt");
		flist.addnew("clinch");
		flist.addnew("rest");
		flist.addnew("combo strike");
		flist.addnew("fight dirty");
		// load to hwnd
		flist.loadtohwnd();
	}


	inline void clearlist()
	{
		SendMessage(player_list,LB_RESETCONTENT,0,0);
	}

	inline void setstatname(char* thecurrent)
	{
		SendMessage(player_item_static,WM_SETTEXT,strlen(thecurrent),(LPARAM)(LPCSTR)thecurrent);
	}

	inline void setcurrent(int current)
	{
		if(current != pre_current)
		{
			// first save the data
			char getstuff[100];
			char currentname[100];
			SendMessage(player_item_static,WM_GETTEXT,100,(LPARAM)(LPCSTR)currentname);
			SendMessage(player_item,WM_GETTEXT,100,(LPARAM)(LPCSTR)getstuff);
			fighter.setfighterdata(currentname,getstuff);

		char curtype[100];
		flist.getindex(current,curtype);
		setstatname(curtype);
		char curdata[100];
		fighter.getfighterdata(curtype,curdata);
		if(strcmp(curdata,"0.") == 0) {

			strcpy(curdata,""); // null data
		}
		SendMessage(player_item,WM_SETTEXT,100,(LPARAM)(LPCSTR)curdata);

		pre_current = current;

		// set the new data

		}
	}

};


extern FighterListHandler FighterList;


// the dialogs

bool openfile(char* cFile)
{
	char szFile[200];


OPENFILENAME ofn;       // common dialog box structure

strcpy(szFile, "*.mma");

// Initialize OPENFILENAME
ZeroMemory(&ofn, sizeof(OPENFILENAME));
ofn.lStructSize = sizeof(OPENFILENAME);
ofn.hwndOwner = mainwindow;
ofn.lpstrFile = szFile;
ofn.nMaxFile = sizeof(szFile);
ofn.lpstrFilter = "mma\0*.mma\0ALL\0*.*\0";
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

strcpy(szFile, "*.mma");

// Initialize OPENFILENAME
ZeroMemory(&ofn, sizeof(OPENFILENAME));
ofn.lStructSize = sizeof(OPENFILENAME);
ofn.hwndOwner = mainwindow;
ofn.lpstrFile = szFile;
ofn.nMaxFile = sizeof(szFile);
ofn.lpstrFilter = "mma\0*.mma\0ALL\0*.*\0";
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

void pause(long seconds)
{
 clock_t delay = seconds * CLOCKS_PER_SEC;
 clock_t start = clock();
 while (clock() - start < delay)
    ;
}


