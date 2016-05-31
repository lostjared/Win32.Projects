/*

 This Example creates a basic Window, but uses TCHAR to abstract use of either 8 bit or 16 bit
 characters. As well as fiddles with CreateProcess just trying to cement the concepts in my mind.


*/


#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include<tchar.h>
HWND mainWnd;

VOID CreateProc(PTSTR, BOOL);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch(msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE)
				SendMessage(hwnd,WM_CLOSE,0,0);

			switch(wParam) {
			case '1':
				CreateProc(TEXT("notepad"), FALSE);
				break;
			case '2':
				CreateProc(TEXT("crtproc.exe"), TRUE);
				break;
			}

		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC dc;
			dc = BeginPaint(hwnd, &ps);
			TextOut(dc, 25, 25, TEXT("Press 1 to CreateProcess on Notepad"), 35);
			TextOut(dc, 25, 50, TEXT("Press 2 To CreateProcess (Inherit) This App"), 43);
			EndPaint(hwnd, &ps);


		}
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


/* CreateProc - Creates a Process with the option of allowing
 * new process to inherit its HANDLE's to kernel objects.
*/
VOID CreateProc(PTSTR cmd, BOOL bInherit) {

	STARTUPINFO si = { sizeof(si) };
	SECURITY_ATTRIBUTES proc1,thread1;
	PROCESS_INFORMATION pi;

	proc1.nLength = sizeof(SECURITY_ATTRIBUTES);
	proc1.lpSecurityDescriptor = NULL;
	proc1.bInheritHandle = bInherit;
	thread1.nLength = sizeof(SECURITY_ATTRIBUTES);
	thread1.lpSecurityDescriptor = NULL;
	thread1.bInheritHandle = FALSE;

	TCHAR szPath[MAX_PATH];
#ifdef MINGW32
	_tcscpy(szPath, cmd); // if VC++ == 8 _tcscpy_s
#else
	_tcscpy_s(szPath, _countof(szPath), cmd);
#endif
	CreateProcess(NULL, szPath, &proc1, &thread1, FALSE, 0, NULL, NULL, &si, &pi);
}


int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR lpszLine, int iCmdShow) {

	HANDLE hMut = CreateMutex(NULL, FALSE, TEXT("{-Mutex-}-For_This_Program"));
	if(GetLastError() == ERROR_ALREADY_EXISTS) {
		MessageBox(NULL, TEXT("Application Already Running"), TEXT("Already Open"), MB_ICONINFORMATION);
		CloseHandle(hMut);
		return 0;
	}

	WNDCLASSEX wcex;
	ZeroMemory(&wcex,sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wcex.lpszClassName = TEXT("CrtProc");
	wcex.lpfnWndProc = (WNDPROC) WindowProc;
	wcex.lpszMenuName = 0;
	wcex.hIcon = wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hInstance = hInst;
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx (&wcex);

	if( (mainWnd = CreateWindowEx(0, TEXT("CrtProc"), TEXT("Startup Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0, 0, GetModuleHandle(0), 0)) ) {

		ShowCursor(TRUE);
		ShowWindow(mainWnd, SW_SHOW);
		UpdateWindow(mainWnd);
		MSG msg;
		while(GetMessage(&msg, 0, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}
	else {
	   MessageBox(GetForegroundWindow(), TEXT("Error could not create Window"), TEXT("Error"), MB_ICONINFORMATION);
	}
	CloseHandle(hMut);
	return 0;
}

