#include "Window.h"
#include "../API Wrappers/Input.h"
#include "../Tools/Math.h"

using namespace Math;

namespace UI
{
	CWindowComponent::CWindowComponent()
	{
		_normal = -1;
		_hover = -1;

		_state = 0;

		_focus = false;

		_onMouseEnter = nullptr;
		_onMouseLeave = nullptr;

		_parent = nullptr;
	}

	CWindowComponent::~CWindowComponent()
	{

	}

	void CWindowComponent::Render()
	{
		if( !_visible )
			return;

		CRenderObject2D::Render();
	}

	void CWindowComponent::Update( const Mouse& mouse )
	{
		if( !_enabled )
			return;

		switch( _state )
		{
		case 0:
			NormalStateUpdate( mouse );
			break;
		case 1:
			HoverStateUpdate( mouse );
			break;
		}
	}

	void CWindowComponent::NormalStateUpdate( const Mouse& m )
	{
		if( PointInRect( m.x,m.y,(int)_pos.x,(int)_pos.y,_width,_height ) )
		{
			// Mouse has entered the window
			// Switch to hover state
			_state = 1;

			setTextureID( _hover );
			setSrcRect( _hoverSrc );

			if( _onMouseEnter )
				_onMouseEnter();
		}
	}
	void CWindowComponent::HoverStateUpdate( const Mouse& m )
	{
		if( !PointInRect( m.x,m.y,(int)_pos.x,(int)_pos.y,_width,_height ) )
		{
			// Switch to normal state
			_state = 0;

			setTextureID( _normal );
			setSrcRect( _normalSrc );

			if( _onMouseLeave )
				_onMouseLeave;

			_focus = false;
		}
		else if( m.buttons[0] )
		{
			_focus = true;
		}
	}
};