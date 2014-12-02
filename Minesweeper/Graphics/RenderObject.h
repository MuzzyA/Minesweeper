#pragma once

#include "../API Wrappers/D3D9.h"
#include "Renderable.h"

class D3D9;

class CRenderObject : public CRenderable
{
	IDirect3DVertexDeclaration9* _pVertDecl;
	IDirect3DVertexBuffer9*	_pVertBuff;
	IDirect3DIndexBuffer9*	_pIndexBuff;
	unsigned		_nNumVerts;
	unsigned		_nNumIndices;

public:
	CRenderObject();
	~CRenderObject();

	void Render( ID3DXEffect* pShader );

	void Init( void* pVerts,unsigned nNumVerts,unsigned nVertexSize,
				D3DVERTEXELEMENT9* pVertDecl,unsigned short* pIndices = nullptr,
				unsigned nNumIndices = 0,D3D9* d3d = nullptr );
	void Destroy();
};
