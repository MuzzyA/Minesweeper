#pragma once

#include "../UI/WindowManager.h"
#include "../API Wrappers/Input.h"
#include "../Graphics/Renderer.h"
using namespace Graphics;

class D3D9;


namespace Engine
{
	class CGameState;
	class CGame;

	class CEngine
	{
		D3D9*		_D3D;
		CInput		_input;
		CRenderer	_renderer;
		CGame*		_game;
	
		vector< CGameState* > _states;

		// For handling time per frame
		unsigned	_prevTime;
		float		_dt;

	public:
		CEngine();
		~CEngine();

		bool Init( D3D9* d3d,CGame* game );
		void Shutdown();

		void Run();
		void Update();
		void Render();

		D3D9* getD3D() { return _D3D; }

		static D3D9* getD3D9() { return inst->_D3D; }
		static CInput* getInput() { return &inst->_input; }
		static void AddState( CGameState* v );
		static void PopState();

	private:
		static CEngine* inst;
	};
};