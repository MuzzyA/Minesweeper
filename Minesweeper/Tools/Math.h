#pragma once


#include "Vector3.h"
#include <Windows.h>


namespace Math
{
	bool PointInRect( const POINT& p,const RECT& r );
	bool PointInRect( int x,int y,const RECT& r );
	bool PointInRect( int x,int y,int rx,int ry,int width,int height );
};