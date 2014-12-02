#pragma once

#include "Game.h"


namespace Engine
{
	class CExampleGame : public CGame
	{
	public:
		void Init();
		void Shutdown();

		void Render();
		void Update( float dt );

		CExampleGame();
		~CExampleGame();
	};
};