/*

 This Example creates a basic Window, but uses TCHAR to abstract use of either 8 bit or 16 bit
 characters.

*/


#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include<tchar.h>
HWND mainWnd;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	switch(msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE)
				SendMessage(hwnd,WM_CLOSE,0,0);
			
		}
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}



int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszLine, int iCmdShow) {

	WNDCLASSEX wcex;
	ZeroMemory(&wcex,sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wcex.lpszClassName = TEXT("WndOne"); /* To Convert a Constant String use _T() */
	wcex.lpfnWndProc = (WNDPROC) WindowProc;
	wcex.lpszMenuName = 0;
	wcex.hIcon = wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hInstance = hInst;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	
		
	RegisterClassEx (&wcex);

	if( (mainWnd = CreateWindowEx(0, TEXT("WndOne"), TEXT("Window "), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0, 0, GetModuleHandle(0), 0)) ) {
	
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

