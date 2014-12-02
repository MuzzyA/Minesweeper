#pragma once

#include "../Graphics/Renderer.h"
#include "../UI/WindowManager.h"
#include <functional>

namespace Engine
{
	class CGameState;
	typedef function< void( CGameState* ) > Func;

	class CGameState
	{
		Graphics::CRenderSet2D	_renderSet2D;
		Graphics::CRenderSet	_renderSet;
		UI::CWindowManager		_windowMan;

		Func	_init;

	public:
		virtual void Update( float dt );
		void Init();


		UI::CWindowManager& getWindowManager() { return _windowMan; }
		Graphics::CRenderSet2D& get2DRenderSet() { return _renderSet2D; }
		Graphics::CRenderSet& getRenderSet() { return _renderSet; }
		void setInitData( Func f ) { _init = f; }

		CGameState();
		~CGameState();
	};
}