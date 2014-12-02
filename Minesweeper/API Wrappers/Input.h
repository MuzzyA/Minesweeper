#pragma once

struct Mouse
{
	int x;
	int y;
	int scroll;
	char buttons[3];
	char dblClickButtons[3];
};

class CInput
{
	char _keys[256];
	char _prevKeys[256];

	Mouse _mouse;
	Mouse _prevMouse;

public:
	CInput();
	~CInput();

	void Update();

	const Mouse& getPrevMouseState() const	{ return _prevMouse; }
	const Mouse& getMouseState() const		{ return _mouse; }
	const char* getKeys() const				{ return _keys; }

	bool isKeyDown( char i ) const		{ return _keys[i] == 1; }
	bool isKeyUp( char i ) const		{ return !_keys[i]; }
	bool KeyPressed( char i ) const;
	bool KeyReleased( char i ) const;
};