#pragma once


struct ID3DXEffect;

class CRenderable
{
public:
	virtual void Render()  { }
	virtual void Render( ID3DXEffect* pShader ) { }
};