
#include "CoralSoulApp.h"


#ifndef TARGET_WIN32
//========================================================================
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);    // <--- ofxShadertoy NEEDS the GL Programmable Renderer
	settings.width = 1024;
	settings.height = 768;
	ofCreateWindow(settings);       // <-------- setup the GL context

									// this kicks off the running of my app
	ofRunApp(new CoralSoulApp());

}

#else

#include "../../resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);    // <--- ofxShadertoy NEEDS the GL Programmable Renderer
	settings.width = 1024;
	settings.height = 768;
	ofCreateWindow(settings);       // <-------- setup the GL context

	HWND hwnd = ofGetWin32Window();
	HICON hMyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hMyIcon);


	ofRunApp(new CoralSoulApp());

}

#endif




