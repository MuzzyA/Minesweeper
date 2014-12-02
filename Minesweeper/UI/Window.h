#pragma once

#include "../Graphics/RenderObject2D.h"
#include <functional>
#include <vector>
using namespace std;

struct Mouse;

namespace UI
{
	typedef function<void()> Func;

	class CWindowComponent : public CRenderObject2D
	{
		vector< CWindowComponent* > _children;
		CWindowComponent*			_parent;

	protected:
		int		_state;

		int		_normal;		// Different textures for each state
		int		_hover;			// of the window
		RECT	_normalSrc;		// Texture Src
		RECT	_hoverSrc;		//

		Func	_onMouseEnter;
		Func	_onMouseLeave;

		int		_width;
		int		_height;

		bool	_focus;
		bool	_visible;
		bool	_enabled;

		virtual void	NormalStateUpdate( const Mouse& m );
		virtual void	HoverStateUpdate( const Mouse& m );

	public:
		CWindowComponent();
		virtual ~CWindowComponent();

	
		// Detects whether the mouse is hovering or leaving the window
		// Call once per frame
		// mouse - the current state of the mouse
		virtual void Update( const Mouse& mouse );
		virtual void Render();

		void AddChild( CWindowComponent* p ) { _children.push_back( p ); p->_parent = this; }

		bool isVisible() const { return _visible; }
		void Visible( bool v ) { _visible = v; }
		bool isEnabled() const { return _enabled; }
		void Enabled( bool v ) { _enabled = v; }
		void setNormalSrc( const RECT& v ) { _normalSrc = v; }
		void setHoverSrc( const RECT& v ) { _hoverSrc = v; }
		void setOnMouseEnter( Func v ) { _onMouseEnter = v; }
		void setOnMouseLeave( Func v ) { _onMouseLeave = v; }
		void setWidth( int v ) { _width = v; }
		int getWidth() const { return _width; }
		void setHeight( int v ) { _height = v; }
		int getHeight() const { return _height; }
		void setNormalTexID( int v ){ _normal = v; }
		void setHoverTexID( int v ) { _hover = v; }
		int getNormalTexID() const { return _normal; }
		int getHoverTexID() const { return _hover; }
	};
};