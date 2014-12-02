#include "Button.h"
#include "../API Wrappers/Input.h"
#include "../Tools/Math.h"
using namespace Math;


namespace UI
{
	void CButton::Update( const Mouse& mouse )
	{
		if( !_enabled )
			return;

		switch( _state )
		{
		case 0: // Normal
			NormalStateUpdate( mouse );
			break;
		case 1: // Hover
			HoverStateUpdate( mouse );
			break;
		case 2: // Mouse down
			MouseDownUpdate( mouse );
			break;
		case 3: // Right Mouse Down
			RMouseDownUpdate( mouse );
			break;
		case 4: // Middle Mouse Down
			MMouseDownUpdate( mouse );
			break;
		}
	}
	
	void CButton::RMouseDownUpdate( const Mouse& m )
	{
		if( !PointInRect( m.x,m.y,(int)_pos.x,(int)_pos.y,_width,_height ) )
		{
			// Switch to normal state
			_state = 0;

			setTextureID( _normal );
			setSrcRect( _normalSrc );

			if( _onMouseLeave )
				_onMouseLeave;

			// Must lose focus so button presses don't confuse each
			// other with other buttons
			_focus = false;
		}
		else if( !m.buttons[1] )
		{
			// Switch to hover state
			_state = 1;

			setTextureID( _hover );
			setSrcRect( _hoverSrc );

			if( _focus && _onRClick )
				_onRClick( this );
		}

		if( m.dblClickButtons[1] )
		{
			if( _onRDblClick )
				_onRDblClick( this );
		}
	}
	void CButton::MouseDownUpdate( const Mouse& m )
	{
		if( !PointInRect( m.x,m.y,(int)_pos.x,(int)_pos.y,_width,_height ) )
		{
			// Switch to normal state
			_state = 0;

			setTextureID( _normal );
			setSrcRect( _normalSrc );

			if( _onMouseLeave )
				_onMouseLeave;

			// Must lose focus so button presses don't confuse each
			// other with other buttons
			_focus = false;
		}
		else if( !m.buttons[0] )
		{
			// Switch to hover state
			_state = 1;

			setTextureID( _hover );
			setSrcRect( _hoverSrc );

			if( _onMouseUp )
				_onMouseUp();

			if( _focus && _onClick )
				_onClick( this );
		}
		
		if( m.dblClickButtons[0] )
		{
			if( _onDblClick )
				_onDblClick( this );
		}
	}
	void CButton::MMouseDownUpdate( const Mouse& m )
	{
		if( !PointInRect( m.x,m.y,(int)_pos.x,(int)_pos.y,_width,_height ) )
		{
			// Switch to normal state
			_state = 0;

			setTextureID( _normal );
			setSrcRect( _normalSrc );

			if( _onMouseLeave )
				_onMouseLeave;

			// Must lose focus so button presses don't confuse each
			// other with other buttons
			_focus = false;
		}
		else if( !m.buttons[1] )
		{
			// Switch to hover state
			_state = 1;

			setTextureID( _hover );
			setSrcRect( _hoverSrc );

			if( _focus && _onMClick )
				_onMClick( this );
		}

		if( m.dblClickButtons[2] )
		{
			if( _onRDblClick )
				_onRDblClick( this );
		}
	}
	void CButton::HoverStateUpdate( const Mouse& m )
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
			// Switch to mouse down state
			_state = 2;

			setTextureID( _mouseDown );
			setSrcRect( _mouseDownSrc );
			
			if( _onMouseDown )
				_onMouseDown();

			// This button should now be the button of focus
			_focus = true;
		}
		else if( m.buttons[1] )
		{
			// Switch to right mouse down state
			_state = 3;

			// This button should now be the button of focus
			_focus = true;
		}
		else if( m.buttons[2] )
		{
			// Switch to middle mouse down state
			_state = 4;

			// This button should now be the button of focus
			_focus = true;
		}
		
		if( m.dblClickButtons[0] )
		{
			if( _onDblClick )
				_onDblClick( this );
		}
		else if( m.dblClickButtons[1] )
		{
			if( _onRDblClick )
				_onRDblClick( this );
		}
		else if( m.dblClickButtons[2] )
		{
			if( _onMDblClick )
				_onMDblClick( this );
		}
	}

	CButton::CButton() : CWindowComponent()
	{
		_onMouseDown = nullptr;
		_onMouseUp = nullptr;
		_onClick = nullptr;

		_mouseDown = -1;

	}
	CButton::~CButton()
	{
		
	}
};