#include<windows.h>
#include<windowsx.h>
#include<tchar.h>

HWND mainWnd;

VOID RunJob(PTSTR, UINT timeout);

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
					RunJob(TEXT("cmd.exe"), 1000); // 1 second
					break;
			}
			
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC dc;
			dc = BeginPaint(hwnd, &ps);
			TextOut(dc, 25, 25, TEXT("Press 1 to CreateProcess on CMD.EXE"), 35);
			TextOut(dc, 25, 50, TEXT("Try running dir /s on C: to see job timeout"), 43), 
			EndPaint(hwnd, &ps);
		}
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


VOID RunJob(PTSTR cmd, UINT timeout) {
	
	HANDLE hj_ = CreateJobObject(NULL, TEXT("JobOne"));
	JOBOBJECT_BASIC_LIMIT_INFORMATION limit_ = { 0 };
	limit_.PriorityClass = IDLE_PRIORITY_CLASS;
	limit_.PerJobUserTimeLimit.QuadPart = timeout; // cannont use more than 1 second of CPU time
	limit_.LimitFlags = JOB_OBJECT_LIMIT_PRIORITY_CLASS | JOB_OBJECT_LIMIT_JOB_TIME;
	
	SetInformationJobObject(hj_, JobObjectBasicLimitInformation, &limit_, sizeof(limit_));
	
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	TCHAR cmdi[MAX_PATH];
	_tcscpy_s(cmdi, _countof(cmdi), cmd);
	BOOL result = CreateProcess(NULL, cmdi, NULL, NULL, FALSE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	AssignProcessToJobObject(hj_, pi.hProcess);
	ResumeThread(pi.hThread);
	CloseHandle(pi.hThread);	
	
	HANDLE handles[2];
	handles[0] = pi.hProcess;
	handles[1] = hj_;
	
	DWORD dw = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
	
	switch( dw - WAIT_OBJECT_0) {
		case 0:
			MessageBox(mainWnd, TEXT("Process Terminated"), TEXT(""), MB_OK | MB_ICONINFORMATION);
			break;
		case 1:
			MessageBox(mainWnd, TEXT("Process Time ran out"), TEXT(""), MB_OK | MB_ICONINFORMATION);
			break;
	}
	
	CloseHandle(pi.hProcess);
	CloseHandle(hj_);
}


int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR lpszLine, int iCmdShow) {
	
	
	WNDCLASSEX wcex;
	ZeroMemory(&wcex,sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wcex.lpszClassName = TEXT("Jobs");
	wcex.lpfnWndProc = (WNDPROC) WindowProc;
	wcex.lpszMenuName = 0;
	wcex.hIcon = wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hInstance = hInst;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	
	RegisterClassEx (&wcex);
	
	if( (mainWnd = CreateWindowEx(0, TEXT("Jobs"), TEXT("Startup Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0, 0, GetModuleHandle(0), 0)) ) {
		
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
	return 0;
}


