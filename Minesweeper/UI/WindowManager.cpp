#include "WindowManager.h"
#include "Button.h"
#include "TextComponent.h"
#include "../API Wrappers/Input.h" 


namespace UI
{
	CWindowComponent* CWindowManager::NewWindow( WindowType type,int width,int height,
				int normalTex,const RECT& normalSrc,int hoverTex, const RECT& hoverSrc,
				Func onMouseEnter,Func onMouseLeave )
	{
		CWindowComponent* comp = nullptr;

		switch( type )
		{
		case WT_Window:
			comp = new CWindowComponent();
			break;
		case WT_Button:
			comp = new CButton();
			break;
		case WT_Text:
			comp = new CTextComponent();
			break;
		}

		if( comp == nullptr ) return nullptr;

		comp->setWidth( width );
		comp->setHeight( height );
		comp->setNormalTexID( normalTex );
		comp->setNormalSrc( normalSrc );
		comp->setTextureID( normalTex );
		comp->setSrcRect( normalSrc );
		comp->setHoverTexID( hoverTex );
		comp->setHoverSrc( hoverSrc );
		comp->setOnMouseEnter( onMouseEnter );
		comp->setOnMouseLeave( onMouseLeave );

		_windows.push_back( comp );

		return comp;
	}

	void CWindowManager::Update( CInput* in )
	{
		for( list<CWindowComponent*>::iterator i = _windows.begin();i != _windows.end();++i )
		{
			(*i)->Update( in->getMouseState() );
		}
	}

	void CWindowManager::Clean()
	{
		for( list<CWindowComponent*>::iterator i = _windows.begin();i != _windows.end();++i )
			delete *i;
		
		_windows.clear();
	}
};