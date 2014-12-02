#include "Renderer.h"
#include "RenderObject2D.h"
#include "RenderObject.h"


namespace Graphics
{
	void CRenderSet2D::Remove( CRenderObject2D* obj )
	{
		for( unsigned i = 0;i < RO_Count;++i )
		{
			vector< CRenderObject2D* >::iterator iter = _renderObjs[i].begin();

			for( ; iter != _renderObjs[i].end();++iter )
			{
				if( (*iter) == obj )
				{
					_renderObjs[i].erase( iter );
					return;
				}
			}
		}
	}
	void CRenderSet::Remove( CRenderObject* obj )
	{
		vector< CRenderObject* >::iterator iter = _renderObjs.begin();
		
		for( ; iter != _renderObjs.end();++iter )
		{
			if( (*iter) == obj )
			{
				_renderObjs.erase( iter );
				return;
			}
		}
	}

	void CRenderer::Render( const CRenderSet2D& renderSet ) const
	{
		for( unsigned i = 0;i < RO_Count;++i )
		{
			for( unsigned j = 0;j < renderSet._renderObjs[i].size();++j )
			{
				renderSet._renderObjs[i][j]->Render();
			}
		}
	}

	
	void CRenderSet::Add( CRenderObject* obj )
	{
		_renderObjs.push_back( obj );
	}
	
	CRenderSet::CRenderSet()
	{

	}
	CRenderSet::~CRenderSet()
	{

	}
	CRenderSet2D::CRenderSet2D()
	{

	}
	CRenderSet2D::~CRenderSet2D()
	{

	}
	CRenderer::CRenderer()
	{

	}
	CRenderer::~CRenderer()
	{

	}
};