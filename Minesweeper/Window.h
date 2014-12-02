#pragma once

#include <Windows.h>


struct Window
{
	int width;
	int height;
	HWND hwnd;
	bool active;
};