#include "Engine.h"
#include "../API Wrappers/D3D9.h"
#include "../Window.h"
#include "../UI/Button.h"
#include "GameState.h"
#include "Game.h"

#include <d3dx9.h>
#pragma comment( lib,"d3dx9.lib" )

#include <time.h>

extern Window g_MainWindow;

Engine::CEngine* Engine::CEngine::inst = nullptr;


namespace Engine
{
	void CEngine::Run()
	{

		MSG msg;
		ZeroMemory( &msg,sizeof( MSG ) );

		unsigned fps = 0;
		unsigned frames = 0;
		float time  = 0;
		
		_dt = 0;
		_prevTime = clock();

		while( g_MainWindow.active )
		{
			// Time since the previous frame
			_dt = (clock() - _prevTime)/1000.0f;
			_prevTime = clock();

			clock_t start = clock();
			
			if( PeekMessage( &msg,g_MainWindow.hwnd,0,0,PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}

			Update();
			Render();

			// Frame rate
			++frames;
			time += (clock() - start)/1000.0f;

			if( time >= 1 )
			{
				fps = frames;
				time = 0;
			}

		}
	}

	void CEngine::Update()
	{
		_input.Update();

		if( _states.size() )
			_states.back()->Update( _dt );

		// Last bit of update calls if needed by the game
		_game->Update( _dt );
	}
	void CEngine::Render()
	{
		_D3D->getDevice()->Clear( 0,0,D3DCLEAR_TARGET,D3DCOLOR_ARGB(255,127,127,127 ),1,0 );

		_D3D->BeginDraw();

		// Render 3D objects here

		_D3D->SpriteBatchBegin();

		// Render 2D Objects here
		for( unsigned i = 0;i < _states.size();++i )
			_renderer.Render( _states[i]->get2DRenderSet() );

		// Last bit of rendering calls if needed by the game
		_game->Render();

		_D3D->SpriteBatchEnd();

		_D3D->EndDraw();
		_D3D->Present();
	}

	void CEngine::AddState( CGameState* v )
	{
		v->Init();
		inst->_states.push_back( v );
	}
	void CEngine::PopState()
	{
		inst->_states.pop_back();
	}

	bool CEngine::Init( D3D9* d3d,CGame* game )
	{
		if( !d3d )
			return false;
		_D3D = d3d;

		if( !game )
			return false;
		_game = game;

		_game->Init();

		// Init
		return true;
	}

	void CEngine::Shutdown()
	{
		_D3D->Shutdown();
	}

	CEngine::CEngine()
	{
		_D3D = nullptr;
		inst = this;
	}
	CEngine::~CEngine()
	{
		Shutdown();
	}
};