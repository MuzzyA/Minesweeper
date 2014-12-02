#pragma once

#include <vector>
using namespace std;

#include <d3d9.h>
#include <d3dx9.h>

#include "../Window.h"

#pragma comment( lib,"d3d9.lib" )
#pragma comment( lib,"d3dx9.lib" )

class D3D9
{
	IDirect3DDevice9*	_pDevice;
	IDirect3D9*			_pD3DObject;

	ID3DXSprite*		_pSpriteMan;

	Window*		_pWindow;

	vector< IDirect3DTexture9* > _textures;
	vector< ID3DXFont* >		_fonts;

	float	_backBufferWidth;
	float	_backBufferHeight;

public:
	D3D9();
	~D3D9();

	bool	Init( Window* pWindow );
	void	Shutdown();

	int LoadTexture( const string& filePath );
	int LoadFont( const string& fontName,int nHeight = 12,int nWidth = 6,bool bItalic = false );

	void BeginDraw();
	void EndDraw();
	void Present();

	void SpriteBatchBegin();
	void DrawSprite( int tex,const RECT* src,const D3DXVECTOR2& pos,
						const D3DXVECTOR2& center,float rot,float scaleX,float scaleY, const D3DXCOLOR& color );
	void DrawFont( int fontID,const string& str,int x, int y, const D3DXCOLOR& color = D3DXCOLOR( 0,0,0,1 ),
						float fRot = 0,float fScale = 1 );
	void SpriteBatchEnd();

	float		getBackBufferWidth() const { return _backBufferWidth; }
	float		getBackBufferHeight() const { return _backBufferHeight; }
	ID3DXSprite*		getSpriteMgr() { return _pSpriteMan; }
	IDirect3DDevice9*	getDevice() { return _pDevice; }
	IDirect3D9*			getD3DObject() { return _pD3DObject; }
};