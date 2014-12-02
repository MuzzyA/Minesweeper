#pragma once

#include "Window.h"


namespace UI
{
	class CButton;
	typedef function< void( CButton* )> ClickFunc;
	
	class CButton : public CWindowComponent
	{
		int		_mouseDown;
		RECT	_mouseDownSrc;

	protected:
		Func	_onMouseDown;
		Func	_onMouseUp;
		ClickFunc	_onClick;
		ClickFunc	_onRClick;
		ClickFunc	_onMClick;
		ClickFunc	_onDblClick;
		ClickFunc	_onRDblClick;
		ClickFunc	_onMDblClick;

		void HoverStateUpdate( const Mouse& m );
		void MouseDownUpdate( const Mouse& m );
		void RMouseDownUpdate( const Mouse& m );
		void MMouseDownUpdate( const Mouse& m );

	public:
		virtual void Update( const Mouse& mouse );

		
		void setMouseDownSrc( const RECT& v ) { _mouseDownSrc = v; }
		void setOnMouseDown( Func v ) { _onMouseDown = v; }
		void setOnMouseUp( Func v ) { _onMouseUp = v; }
		void setOnDblClick( ClickFunc v ) { _onDblClick = v; }
		void setOnRDblClick( ClickFunc v ) { _onRDblClick = v; }
		void setOnClick( ClickFunc v ) { _onClick = v; }
		void setOnRClick( ClickFunc v ) { _onRClick = v; }
		void setMouseDownTexture( int v ) { _mouseDown = v; }
		int getMouseDownTexture() const { return _mouseDown; }
		void setOnMDblClick( ClickFunc v ) { _onMDblClick = v; }
		void setOnMClick( ClickFunc v ) { _onMClick = v; }

		CButton();
		virtual ~CButton();
	};
};