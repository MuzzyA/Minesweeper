#pragma once

#include "../Engine/Game.h"
#include "../Engine/GameState.h"

class CBlock;
namespace UI { class CButton; };

class CMinesweeper : public Engine::CGame
{
	Engine::CGameState	_gameState;

	vector< vector< CBlock* > > _blocks;

	float _totalTime;

	void GameStateInit( Engine::CGameState* pState );

public:
	void Init();
	void Shutdown();

	void Render();
	void Update( float dt );

	void Reset();
	
	int _fontID;
	
	int numBombs;

	// Texture ID's for blocks
	int _normal;
	int _hover;
	int _disabled;
	int _flagged;
	int _mine;
	int _block1;
	int _block2;
	int _block3;
	int _block4;
	int _block5;
	int _block6;
	int _block7;
	int _block8;

	// New Game/Game Over
	bool _firstClick;
	bool _dead;

	// Button to reset the game
	UI::CButton* _start;
	int _startNorm;
	int _startDead;
	int _mineClicked;

	vector< vector< CBlock* > >& getBlocks() { return _blocks; }

	CMinesweeper();
	~CMinesweeper();

	static CMinesweeper* Inst() { return inst; }

private:
	static CMinesweeper* inst;
};