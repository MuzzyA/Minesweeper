#include "Input.h"

#include <Windows.h>

extern Mouse g_Mouse;


void CInput::Update()
{
	memcpy( _prevKeys,_keys,sizeof( _prevKeys ) );
	GetKeyboardState( (PBYTE)_keys );

	memcpy( &_prevMouse,&_mouse,sizeof( _prevMouse ) );

	_mouse = g_Mouse;
	g_Mouse.dblClickButtons[0] = 0;
	g_Mouse.dblClickButtons[1] = 0;
	g_Mouse.dblClickButtons[2] = 0;
}

bool CInput::KeyPressed( char i ) const
{
	if( _keys[i] && !_prevKeys[i] )
		return true;

	return false;
}
bool CInput::KeyReleased( char i ) const
{
	if( !_keys[i] && _prevKeys[i] )
		return true;

	return false;
}

CInput::CInput()
{
	ZeroMemory( _keys,sizeof( _keys ) );
	ZeroMemory( _prevKeys,sizeof( _prevKeys ) );
	ZeroMemory( &_mouse,sizeof( _mouse ) );
	ZeroMemory( &_prevMouse,sizeof( _prevMouse ) );
}
CInput::~CInput()
{

}