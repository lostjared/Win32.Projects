/* 
 abstraction of 16 bit / 8 bit characters demo, for me to remember
 the header files define whether a TCHAR is ethier a wchar_t or char
 then ethier the Windows function use A or W versions of the functions
*/


#include<windows.h>
#include<tchar.h>


void _Print(HANDLE hBuf, TCHAR *text) {
	WriteFile(hBuf, text, _tcslen(text) * sizeof(TCHAR), 0, 0);
}

//  main function (ethier char or wchar_t) ( WINDOWS )
int _tmain(int argc, LPTSTR argv[]) {

	TCHAR *hello_world = _TEXT("HELLO WORLD!@\n");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	_Print(hOut, hello_world);
	_Print(hOut, _TEXT("Hey this is Eithier a WCHAR or a CHAR\n"));
	
	CloseHandle(hOut);
	return 0;
}


