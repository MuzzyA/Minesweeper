#pragma once

#include "../Engine/Game.h"
#include "../Engine/GameState.h"

#include "../UI/TextComponent.h"

class CBlock;
namespace UI { class CButton; };

class CMinesweeper : public Engine::CGame
{
	Engine::CGameState	_gameState;

	vector< vector< CBlock* > > _blocks;
	vector< CBlock* > _clickedBlocks;

	float _totalTime;
	char* _timeFormat;

	int	_maxBombs;
	int _numBlocks;

	UI::CWindowComponent* _pWarningMessageWindow;
	UI::CWindowComponent* _pBombCountWindow;
	UI::CTextComponent*	  _pBombTextLabel;
	UI::CButton*		  _pBombCountButton;


	void GameStateInit( Engine::CGameState* pState );

	// Initialize the warning message window
	void SetUpMessageWindow(Engine::CGameState* pState);

	// Initialize the bomb count window
	void SetUpBombCountWindow(Engine::CGameState* pState);

	// Enable/Disable all buttons
	void EnableButtons(bool bEnable);


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
	bool _gameWon;

	// Button to reset the game
	UI::CButton* _start;
	int _startNorm;
	int _startDead;
	int _startWon;
	int _mineClicked;

	// Checks the board for any blocks that are available to be clicked
	bool CheckForFreeBlocks();

	// Displays popup window for confirming the game restart
	void DisplayResetWarning(bool bDisplay);

	// Displays popup window for setting the amount of bombs
	void DisplayBombCountPopup(bool bDisplay);

	void UpdateBombCountLabel();

	void setNumBlocks(int val) { _numBlocks = val; }
	int getNumBlocks() const { return _numBlocks; }
	void setMaxBombs(int val) { _maxBombs = val; }
	int getMaxBombs() const { return _maxBombs; }

	vector< vector< CBlock* > >& getBlocks() { return _blocks; }
	vector< CBlock* >& getClickedBlocks() { return _clickedBlocks; }

	CMinesweeper();
	~CMinesweeper();

	static CMinesweeper* Inst() { return inst; }

private:
	static CMinesweeper* inst;
};