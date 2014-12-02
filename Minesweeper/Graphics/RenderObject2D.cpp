#include "RenderObject2D.h"
#include "../API Wrappers/D3D9.h"
#include "../Engine/Engine.h"

using namespace Engine;

void CRenderObject2D::Render() const
{
	D3D9* d3d = CEngine::getD3D9();

	if( _texID != -1 )
		d3d->DrawSprite( _texID,&_src,_pos,_center,_rot,_scaleX,_scaleY,_color );
}


CRenderObject2D::CRenderObject2D()
{
	_texID = -1;
	_pos = D3DXVECTOR2( 100,100 );
	_center = D3DXVECTOR2( 0,0 );
	_rot = 0;
	_scaleX = _scaleY = 1;
	_color = D3DXCOLOR( 1,1,1,1 );
}