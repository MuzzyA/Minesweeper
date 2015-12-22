#pragma once

#include "Renderable.h"

#include <d3dx9.h>
#pragma comment( lib,"d3dx9.lib" )


class CRenderObject2D
{
	int _texID;

protected:
	D3DXVECTOR2 _pos;
	D3DXVECTOR2 _center;
	float		_scaleX;
	float		_scaleY;
	float		_rot;
	RECT		_src;

	D3DXCOLOR	_color;

public:
	CRenderObject2D();

	virtual void Render() const;

	int getTextureID() const { return _texID; }
	void setTextureID( int v ) { _texID = v; }
	void setPos( float x,float y ) { _pos.x = x; _pos.y = y; }
	const D3DXVECTOR2& getPos() const { return _pos; }
	void setCenter( const D3DXVECTOR2& v ) { _center = v; }
	const D3DXVECTOR2& getCenter() const { return _center; }
	void setScaleX( float v ) { _scaleX = v; }
	float getScaleX() const { return _scaleX; }
	void setScaleY( float v ) { _scaleY = v; }
	float getScaleY() const { return _scaleY; }
	void setScale( float x,float y ) { _scaleX = x; _scaleY = y; }
	void setRotation( float v ) { _rot = v; }
	float getRotation() const { return _rot; }
	void setSrcRect( int left,int top,int right,int bottom ) { SetRect( &_src,left,top,right,bottom ); }
	void setSrcRect( const RECT& v ) { _src = v; }
	const RECT& getSrcRect() const { return _src; }
	void setColor( float r,float g,float b,float a ) { _color = D3DXCOLOR( r,g,b,a ); }
	void setColor( const D3DXCOLOR& v ) { _color = v; }
	const D3DXCOLOR& getColor() const { return _color; }
};