#include "Math.h"


namespace Math
{
	bool PointInRect( const POINT& p,const RECT& r )
	{
		if( p.x > r.right || p.x < r.left ||
			p.y > r.bottom || p.y < r.top )
			return false;

		return true;
	}
	bool PointInRect( int x,int y,const RECT& r )
	{
		if( x > r.right || x < r.left ||
			y > r.bottom || y < r.top )
			return false;

		return true;
	}
	bool PointInRect( int x,int y,int rx,int ry,int width,int height )
	{
		if( x > rx + width || x < rx ||
			y > ry + height || y < ry )
			return false;

		return true;
	}
};