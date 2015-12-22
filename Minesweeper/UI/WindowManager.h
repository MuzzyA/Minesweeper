#pragma once

#include <Windows.h>
#include <list>
using namespace std;

#include "Window.h"

class CInput;


namespace UI
{
	enum WindowType { WT_Window,WT_Button,WT_Text,WT_Count };

	class CWindowManager
	{
		list< CWindowComponent* >	_windows;

	public:
		// Allocates new memory for a CWindowComponent object.
		// type - The type of window to create
		// width/height = dimensions
		// normalTex/hoverTex - Textures to be used when in normal state or hover state
		// normalSrc/hoverSrc - Source RECT for the textures in normal state and hover state
		// onMouseEnter - Called 'once' as soon as the mouse enters the window
		// onMouseLeave - Called 'once' as soon as the mouse leaves the window
		CWindowComponent* NewWindow( WindowType type,int width,int height,
					int normalTex,const RECT& normalSrc,int hoverTex, const RECT& hoverSrc,
					Func onMouseEnter = nullptr,Func onMouseLeave = nullptr );
		void AddWindow( CWindowComponent* obj ) { _windows.push_back( obj ); }
		void RemoveWindow( CWindowComponent* obj ) { _windows.remove( obj ); }
		void DestroyWindow( CWindowComponent* obj ) { _windows.remove( obj ); delete obj; }

		void Update( CInput* in );

		void Clean();

		CWindowManager() { }
		~CWindowManager() { Clean(); }
	};
};