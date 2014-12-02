#include <Windows.h>
#include <time.h>
#include "Window.h"
#include "Engine/Engine.h"
#include "API Wrappers/D3D9.h"
#include "API Wrappers/Input.h"

using namespace Engine;


#include "Game/Minesweeper.h"
#define YOUR_GAME_CLASS		CMinesweeper

#define WINDOW_WIDTH		690
#define WINDOW_HEIGHT		430

// Globals
Window	g_MainWindow;
Mouse	g_Mouse;


// Initializes the window with the width and height given
// also initializes the engine
bool Init( HINSTANCE hInst,int nWinWidth,int nWinHeight,bool bFullscreen );
void Shutdown();


LRESULT CALLBACK WinProc( HWND,UINT,WPARAM,LPARAM );

int WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrev,LPSTR szCmdLine,int nCmdShow )
{
	srand( (unsigned) time( 0 ) );

	// Initialize windows, close the program if it fails
	if( !Init( hInst,WINDOW_WIDTH,WINDOW_HEIGHT,false ) )
		return 0;

	D3D9 d3d;
	
	if( !d3d.Init( &g_MainWindow ) )
		return 0;

	CEngine engine;

	// -- Create game object here -- //
	YOUR_GAME_CLASS game;

	engine.Init( &d3d,&game );
	
	engine.Run();

	engine.Shutdown();
	Shutdown();

	return 0;
}

LRESULT CALLBACK WinProc( HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
	case WM_QUIT:
		g_MainWindow.active = false;
		break;
		
		// MOUSE INPUT
	case WM_LBUTTONDOWN:
		g_Mouse.buttons[0] = 1;
		break;
	case WM_LBUTTONUP:
		g_Mouse.buttons[0] = 0;
		break;
	case WM_RBUTTONDOWN:
		g_Mouse.buttons[1] = 1;
		break;
	case WM_RBUTTONUP:
		g_Mouse.buttons[1] = 0;
		break;
	case WM_MBUTTONDOWN:
		g_Mouse.buttons[2] = 1;
		break;
	case WM_MBUTTONUP:
		g_Mouse.buttons[2] = 0;
		break;
	case WM_LBUTTONDBLCLK:
		g_Mouse.dblClickButtons[0] = 1;
		break;
	case WM_RBUTTONDBLCLK:
		g_Mouse.dblClickButtons[1] = 1;
		break;
	case WM_MBUTTONDBLCLK:
		g_Mouse.dblClickButtons[3] = 1;
		break;
	case WM_MOUSEWHEEL:
		g_Mouse.scroll = HIWORD( wParam );
		break;
	case WM_MOUSEMOVE:
		g_Mouse.x = LOWORD( lParam );
		g_Mouse.y = HIWORD( lParam );
		break;
	}
	
	return DefWindowProc( hwnd,msg,wParam,lParam );
}

bool Init( HINSTANCE hInst,int nWinWidth,int nWinHeight,bool bFullscreen )
{
	// Set up the window class
	WNDCLASSEX wc;
	ZeroMemory( &wc,sizeof(WNDCLASSEX) );

	wc.hInstance = hInst;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.lpfnWndProc = WinProc;
	wc.lpszClassName = L"WindowClass";
	wc.hCursor = LoadCursor( NULL,IDC_ARROW );
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	RegisterClassEx( &wc );

	if( !bFullscreen )
	{
		nWinWidth += 16;
		nWinHeight += 40;
	}

	HWND hwnd = 
		CreateWindowEx( NULL,
		wc.lpszClassName,
		L"Minesweeper",
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nWinWidth,
		nWinHeight,
		NULL,
		NULL,
		hInst,
		NULL );

	if( hwnd == NULL )
		return false;
	
	ShowWindow( hwnd,5 );
	UpdateWindow( hwnd );

	g_MainWindow.width = nWinWidth;
	g_MainWindow.height = nWinHeight;
	g_MainWindow.hwnd = hwnd;
	g_MainWindow.active = true;

	return true;
}
void Shutdown()
{
	//PostQuitMessage( 0 );
}