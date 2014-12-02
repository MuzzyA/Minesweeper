#include "D3D9.h"

#include <d3dx9.h>
#pragma comment( lib,"d3dx9.lib" )

#define SAFE_RELEASE( p ) if( p ) p->Release(); p = nullptr


bool D3D9::Init( Window* pWindow )
{
	// Create the d3d object for D3D9
	_pD3DObject = Direct3DCreate9( D3D_SDK_VERSION );

	// Parameters for creating the D3D9 device
	D3DPRESENT_PARAMETERS pp;
	ZeroMemory( &pp,sizeof( D3DPRESENT_PARAMETERS ) );

	// TODO: Make the back buffer and window
	// able to keep accurate mouse positions
	pp.BackBufferWidth = pWindow->width - 16;
	pp.BackBufferHeight = pWindow->height - 40;
	pp.hDeviceWindow = pWindow->hwnd;
	pp.Windowed = true;
	pp.BackBufferFormat = D3DFMT_X8R8G8B8;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// Create the handle to the d3d device for D3D9
	HRESULT hr = _pD3DObject->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,pWindow->hwnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&pp,&_pDevice );

	if( FAILED( hr ) )
		return false;

	D3DXCreateSprite( _pDevice,&_pSpriteMan );

	IDirect3DSurface9* backBuffer = nullptr;
	_pDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&backBuffer );
	
	D3DSURFACE_DESC desc;
	backBuffer->GetDesc( &desc );

	_backBufferWidth = (float)desc.Width;
	_backBufferHeight = (float)desc.Height;

	// Resize the window
	RECT rect = {0,0, (long)_backBufferWidth, (long)_backBufferHeight };
	AdjustWindowRect( &rect, GetWindowLong(pWindow->hwnd, WS_OVERLAPPEDWINDOW), FALSE );

	return true;
}

int D3D9::LoadTexture( const string& filePath )
{
	HRESULT hr;

	IDirect3DTexture9* tex = nullptr;
	hr = D3DXCreateTextureFromFileA( _pDevice,filePath.c_str(),&tex );

	if( FAILED( hr ) )
		return -1;

	_textures.push_back( tex );
	return _textures.size()-1;
}

int D3D9::LoadFont( const string& fontName,int nHeight,int nWidth,bool bItalic )
{
	ID3DXFont* font = nullptr;

	D3DXCreateFontA( _pDevice,nHeight,nWidth,1,1,bItalic,DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,fontName.c_str(),&font );

	_fonts.push_back( font );

	return _fonts.size()-1;
}

void D3D9::DrawFont( int fontID,const string& str,int x, int y, const D3DXCOLOR& color,float fRot,float fScale )
{
	D3DXMATRIX world;
	D3DXMATRIX rot;
	D3DXMATRIX scale;

	D3DXMatrixTranslation( &world,(float)x,(float)y,0 );
	D3DXMatrixRotationZ( &rot,fRot );
	D3DXMatrixScaling( &scale,fScale,fScale,1 );

	_pSpriteMan->SetTransform( &( world * scale * rot ) );

	RECT r = { 0,0,1,1 };
	_fonts[ fontID ]->DrawTextA( _pSpriteMan,str.c_str(),-1,&r,DT_NOCLIP,color );
}

void D3D9::DrawSprite( int tex,const RECT* src,const D3DXVECTOR2& pos,
						const D3DXVECTOR2& center,float rot,float scaleX,float scaleY, const D3DXCOLOR& color )
{
	D3DXMATRIX world;
	D3DXMATRIX r;
	D3DXMATRIX s;
	D3DXMatrixTranslation( &world,1/scaleX * pos.x,1/scaleY * pos.y, 0 );
	D3DXMatrixRotationZ( &r,rot );
	D3DXMatrixScaling( &s,scaleX,scaleY,1 );

	D3DXVECTOR3 c( center.x,center.y,0 );

	world *= s * r;
	_pSpriteMan->SetTransform( &world );

	const RECT* source = nullptr;

	if( src ) source = src;

	_pSpriteMan->Draw( _textures[tex],source,&c,&D3DXVECTOR3( 0,0,0 ),color );
}

void D3D9::SpriteBatchBegin()
{
	_pSpriteMan->Begin( D3DXSPRITE_ALPHABLEND );
}
void D3D9::SpriteBatchEnd()
{
	_pSpriteMan->End();
}

void D3D9::BeginDraw()
{
	_pDevice->BeginScene();
}
void D3D9::EndDraw()
{
	_pDevice->EndScene();
}
void D3D9::Present()
{
	_pDevice->Present( 0,0,0,0 );
}

void D3D9::Shutdown()
{
	SAFE_RELEASE( _pDevice );
	SAFE_RELEASE( _pD3DObject );
}

D3D9::D3D9()
{
	_pDevice = nullptr;
	_pD3DObject = nullptr;
}

D3D9::~D3D9()
{
	Shutdown();
}