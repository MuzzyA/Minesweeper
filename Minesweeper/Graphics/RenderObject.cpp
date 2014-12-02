#include "RenderObject.h"
#include "../API Wrappers/D3D9.h"
#include "../Engine/Engine.h"
#include "../Tools/Math.h"

using namespace Math;
using namespace Engine;

#include <d3dx9.h>
#pragma comment( lib,"d3dx9.lib" )


#define SAFE_RELEASE( p ) if( p ) p->Release(); p = nullptr

void CRenderObject::Render( ID3DXEffect* pShader )
{
	// SEEMS AS THOUGH THE PIXEL SHADER IS GETTING IGNORED

	// TEMP
	D3DXMATRIX cam;
	D3DXVECTOR3 pos( 5,5,-5 );
	D3DXVECTOR3 at( 0,0,0 );

	D3DXMatrixLookAtLH( &cam,&pos,&at,&D3DXVECTOR3( 0,1,0 ) );

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH( &proj,3.141592f/4.0f,800.0f/600.0f,.01f,100.0f );

	D3DXMATRIX transform;
	D3DXMATRIX rot;
	D3DXMatrixTranslation( &transform,0,0,0 );
	D3DXMatrixRotationZ( &rot,3.141592f/4.0f );

	//transform = rot * transform;
	//


	IDirect3DDevice9* device = CEngine::getD3D9()->getDevice();

	unsigned passes = 0;

	pShader->Begin( &passes,0 );

	pShader->SetMatrix( "gWorld",&transform );
	pShader->SetMatrix( "viewProj",&(cam * proj) );
	pShader->CommitChanges();

	device->SetVertexDeclaration( _pVertDecl );
	device->SetStreamSource( 0,_pVertBuff,0,sizeof( Vector3 ) );
	device->SetIndices( _pIndexBuff );

	for( unsigned i = 0;i < passes;++i )
	{
		pShader->BeginPass( i );
	
		device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,_nNumVerts,0,_nNumIndices/3 );
		pShader->EndPass();
	}

	pShader->End();
}

void CRenderObject::Init( void* pVerts,unsigned nNumVerts,unsigned nVertexSize,
				D3DVERTEXELEMENT9* pVertDecl,unsigned short* pIndices,
				unsigned nNumIndices,D3D9* d3d )
{
	IDirect3DDevice9* device = nullptr;

	if( d3d )
		device = d3d->getDevice();
	else
		device = CEngine::getD3D9()->getDevice();

	unsigned vertSize = nNumVerts * nVertexSize;
	unsigned indexSize = nNumIndices * sizeof( unsigned short );

	device->CreateVertexBuffer( vertSize,0,0,D3DPOOL_DEFAULT,&_pVertBuff,0 );
	device->CreateVertexDeclaration( pVertDecl,&_pVertDecl );

	void* vram = nullptr;

	_pVertBuff->Lock( 0,0,&vram,0 );
	memcpy( vram,pVerts,vertSize );
	_pVertBuff->Unlock();

	if( pIndices )
	{
		device->CreateIndexBuffer( indexSize,0,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&_pIndexBuff,0 );

		_pIndexBuff->Lock( 0,0,&vram,0 );
		memcpy( vram,pIndices,indexSize );
		_pIndexBuff->Unlock();
	}

	_nNumVerts = nNumVerts;
	_nNumIndices = nNumIndices;
}
void CRenderObject::Destroy()
{
	SAFE_RELEASE( _pVertBuff );
	SAFE_RELEASE( _pVertDecl );
	SAFE_RELEASE( _pIndexBuff );
}

CRenderObject::CRenderObject()
{
	_pVertBuff = nullptr;
	_pVertDecl = nullptr;
	_pIndexBuff = nullptr;
}
CRenderObject::~CRenderObject()
{
	Destroy();
}