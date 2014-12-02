#pragma once

#include <vector>
using namespace std;

class CRenderObject2D;
class CRenderObject;


namespace Graphics
{
	enum RenderOrder{ RO_1,RO_2,RO_3,RO_4,RO_Count };

	class CRenderSet;
	class CRenderSet2D;
	class CRenderer;

	class CRenderSet
	{
		vector< CRenderObject* >	_renderObjs;

	public:
		void Add( CRenderObject* obj );
		void Remove( CRenderObject* obj );

		CRenderSet();
		virtual ~CRenderSet();
	};
	class CRenderSet2D
	{
		friend class CRenderer;

		vector< CRenderObject2D* >	_renderObjs[RO_Count];

	public:
		// order - the order the render object should be drawn
		// RO_1 draws first, RO_4 draws last
		// RO_1 - back, RO_4 - front
		void Add( CRenderObject2D* obj,RenderOrder order ) { _renderObjs[ order ].push_back( obj ); }
		void Remove( CRenderObject2D* obj );

		CRenderSet2D();
		virtual ~CRenderSet2D();
	};

	class CRenderer
	{
		friend class CRenderSet2D;

	public:
		void Render( const CRenderSet2D& renderSet ) const;

		CRenderer();
		~CRenderer();
	};
};