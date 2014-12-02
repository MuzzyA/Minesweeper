#include "GameState.h"
#include "Engine.h"


namespace Engine
{
	void CGameState::Update( float dt )
	{
		_windowMan.Update( CEngine::getInput() );
	}

	void CGameState::Init()
	{
		if( !_init )
			return;

		_init( this );
	}

	CGameState::CGameState()
	{
		_init = nullptr;
	}
	CGameState::~CGameState()
	{
		
	}
};