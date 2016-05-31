#pragma once // include this header only once
#define WIN32_LEAN_AND_MEAN // only the nessicary stuff for windows
#include<winsock.h> // windows sockets
#include<windows.h> // windows header file
#pragma comment(lib,"wsock32.lib") // link to wsock32.lib
#define ON_SOCKET WM_USER+1 //macro to stand for are socket message

WSADATA ws;// winsock data structure
extern HWND the_wnd;// extern to the window handle

// init winsock
inline void InitWinsock() {
	WSAStartup(MAKEWORD(1,1),&ws);
	if(ws.wVersion != MAKEWORD(1,1)) {
		MessageBox(GetForegroundWindow(),"Error Initalizing Winsock!","Error!",MB_OK | MB_ICONERROR);
		WSACleanup();
	}
}

// the socket structure
struct mxSocket {
	SOCKET s;
	unsigned int port;
	char ip_addy[256];
	struct sockaddr_in *addy;
};

// Listen with a socket structure
void mxListen(mxSocket *s, struct sockaddr_in &addr) {
	s->s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    addr.sin_family = AF_INET;
	addr.sin_port = htons(s->port);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(s->s, (struct sockaddr*)&addr, sizeof(addr));
	WSAAsyncSelect (s->s, the_wnd, ON_SOCKET, (FD_READ | FD_CONNECT | FD_CLOSE | FD_ACCEPT));
	listen(s->s,1);
}

// connect with a socket structure
void mxConnect(mxSocket *s, struct sockaddr_in &addr) {
	s->s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(s->port);
	addr.sin_addr.s_addr = inet_addr(s->ip_addy);
	s->addy = &addr;
	WSAAsyncSelect (s->s, the_wnd, ON_SOCKET, (FD_READ | FD_CONNECT | FD_CLOSE));
	connect(s->s,(struct sockaddr*)&addr,sizeof(addr));
}