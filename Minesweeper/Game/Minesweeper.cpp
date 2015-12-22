#include "Minesweeper.h"
#include "../Engine/Engine.h"
#include "../UI/Button.h"
#include "../API Wrappers/D3D9.h"
#include "Block.h"
#include "../UI/TextComponent.h"

#include <sstream>
#include "../Tools/Util.h"

using namespace Engine;
using namespace UI;

CMinesweeper* CMinesweeper::inst = nullptr;

void GameStateInit( CGameState* pState,void* pData );
void onBlockClick( CButton* button ); // button - the block that was clicked
void onBlockRClick( CButton* button );
void onBlockDblClick( CButton* button );


void CMinesweeper::Reset()
{
	// Clear the list of clicked blocks
	_clickedBlocks.clear();
	_numBlocks = 0;

	// Set every block to it's original state
	for( unsigned i = 0;i < _blocks.size();++i )
	{
		for( unsigned j = 0;j < _blocks[i].size();++j )
		{
			_blocks[i][j]->Enabled( true );
			_blocks[i][j]->setFlagged( false );
			_blocks[i][j]->HasBomb( false );
			_blocks[i][j]->setClicked( false );
			_blocks[i][j]->setTextureID( _normal );
			_blocks[i][j]->setNormalTexID( _normal );
			_blocks[i][j]->setHoverTexID( _hover );
			_blocks[i][j]->setMouseDownTexture( _disabled );

			++_numBlocks;
		}
	}

	// Create a new randomized game
	unsigned bombCount = numBombs = _maxBombs;

	for( unsigned i = 0;i < bombCount; ++i )
	{
		int x = rand() % _blocks.size();
		int y = rand() % _blocks[x].size();

		if( !_blocks[ x ][ y ]->HasBomb() )
			_blocks[ x ][ y ]->HasBomb( true );
		else
			--i;
	}

	// Reset booleans
	_firstClick = true;
	_dead = false;
	_gameWon = false;

	// Reset Textures
	_start->setNormalTexID( _startNorm );
	_start->setTextureID( _startNorm );

	// Reset Time
	_totalTime = 0;
	Util::FormatTimeFromInt((int)_totalTime, _timeFormat);
}

void CMinesweeper::Init()
{
	inst = this;

	_gameState.setInitData( bind( &CMinesweeper::GameStateInit,this,placeholders::_1 ) );
	CEngine::AddState( &_gameState );

	Reset();
}

void CMinesweeper::Shutdown()
{

}

void CMinesweeper::Render()
{
	ostringstream bombDisplay;
	ostringstream timeDisplay;
	
	bombDisplay << "Bombs:    " << numBombs;

	if (_timeFormat != nullptr)
		timeDisplay << _timeFormat;

	int bufferWidth = (int)CEngine::getD3D9()->getBackBufferWidth();

	CEngine::getD3D9()->DrawFont(_fontID, bombDisplay.str(), 64, 8);
	CEngine::getD3D9()->DrawFont( _fontID,"Time:", bufferWidth - 170, 8);
	CEngine::getD3D9()->DrawFont( _fontID,timeDisplay.str(), bufferWidth - ( 10*timeDisplay.str().size() ) - 32,8 );

	if(_gameWon) 
		CEngine::getD3D9()->DrawFont(_fontID,"YOU WIN!", 200, 8,D3DXCOLOR(0,0,1,1));
}
void CMinesweeper::Update( float dt )
{
	if (!_firstClick && !_dead && !_gameWon)
	{
		_totalTime += dt;

		// Formatting the time
		Util::FormatTimeFromInt((int)_totalTime,_timeFormat);
	}
}

CBlock* BlockAt( vector< vector< CBlock* > >& blks,int x,int y )
{
	if( x < 0 || x >= (int)blks.size() || y < 0 || y >= (int)blks.front().size() )
		return nullptr;

	return blks[x][y];
}

bool HasBomb( vector< vector< CBlock* > >& blks,int x,int y )
{
	if( x < 0 || x >= (int)blks.size() || y < 0 || y >= (int)blks.front().size() )
		return false;

	if( blks[ x ][ y ]->HasBomb() )
		return true;

	return false;
}

void onBlockClick( CButton* button )
{
	CMinesweeper* inst = CMinesweeper::Inst();
	
	vector< vector< CBlock* > >& blks = inst->getBlocks();
	vector< CBlock* >& clickedBlks = inst->getClickedBlocks();
	CBlock* b = (CBlock*)button;

	// Do nothing if the block is flagged or already clicked
	if( b->Flagged() || b->Clicked() || inst->_dead || inst->_gameWon)
		return;

	// If it's the first click, we want to make sure there's no bomb underneath
	if( inst->_firstClick )
	{
		inst->_firstClick = false;

		if (b->HasBomb())
		{
			// Very inefficient way of finding a new spot, but
			// this will find an unoccupied block to place a bomb
			while (1)
			{
				int x = rand() % blks.size();
				int y = rand() % blks[x].size();

				if (!blks[x][y]->HasBomb())
				{
					blks[x][y]->HasBomb(true);
					break;
				}
			}
		}

		b->HasBomb( false );
	}

	// Add this to t

	// See if this guy has a bomb
	if( b->HasBomb() )
	{
		// Disable everything
		for( unsigned i = 0;i < blks.size();++i )
		{
			for( unsigned j = 0;j < blks[i].size();++j )
			{
				if( blks[i][j]->HasBomb() && !blks[i][j]->Flagged() )
					blks[i][j]->setTextureID( inst->_mine );
				else if( !blks[i][j]->HasBomb() && blks[i][j]->Flagged() )
					blks[i][j]->setTextureID( inst->_mineClicked );

				blks[i][j]->Enabled( false );
			}
		}

		inst->_dead = true;
		inst->_start->setNormalTexID( inst->_startDead );
		inst->_start->setTextureID( inst->_startDead );

		b->setTextureID( inst->_mineClicked );
		return;
	}
	
	// Block has been clicked
	b->setClicked( true );

	if ( Util::VectorContainsItem( clickedBlks,b ) == -1)
		clickedBlks.push_back(b);

	int x = b->getCol();
	int y = b->getRow();

	unsigned bombCount = 0;
	vector< CBlock* > surroundingBlocks;
	CBlock* temp = nullptr;

	// See how many bombs are surrounding this block
	if( ( temp = BlockAt( blks,x-1,y-1 )) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x,y-1 )  ) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x+1,y-1 )) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x+1,y )  ) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x+1,y+1 )) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x,y+1 )  ) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x-1,y+1 )) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x-1,y )  ) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );

	for( unsigned i = 0;i < surroundingBlocks.size();++i )
		if( surroundingBlocks[i]->HasBomb() )
			++bombCount;
	
	// If there are no bombs surrounding the block, we need to expand the 'clicked' blocks
	if( bombCount == 0 )
	{
		// Disable the button
		b->Enabled( false );

		// Go through all of the surrounding blocks, 'clicking' them
		for( unsigned i = 0;i < surroundingBlocks.size();++i )
			onBlockClick( surroundingBlocks[i] );

		b->setTextureID( inst->_disabled );

	}

	// Definitely can do this better
	// Set the textures based on the number of bombs around the block
	switch( bombCount )
	{
	case 1:
		b->setTextureID( inst->_block1 );
		b->setNormalTexID( inst->_block1 );
		b->setHoverTexID( inst->_block1 );
		b->setMouseDownTexture( inst->_block1 );
		break;
	case 2:
		b->setTextureID( inst->_block2 );
		b->setNormalTexID( inst->_block2 );
		b->setHoverTexID( inst->_block2 );
		b->setMouseDownTexture( inst->_block2 );
		break;
	case 3:
		b->setTextureID( inst->_block3 );
		b->setNormalTexID( inst->_block3 );
		b->setHoverTexID( inst->_block3 );
		b->setMouseDownTexture( inst->_block3 );
		break;
	case 4:
		b->setTextureID( inst->_block4 );
		b->setNormalTexID( inst->_block4 );
		b->setHoverTexID( inst->_block4 );
		b->setMouseDownTexture( inst->_block4 );
		break;
	case 5:
		b->setTextureID( inst->_block5 );
		b->setNormalTexID( inst->_block5 );
		b->setHoverTexID( inst->_block5 );
		b->setMouseDownTexture( inst->_block5 );
		break;
	case 6:
		b->setTextureID( inst->_block6 );
		b->setNormalTexID( inst->_block6 );
		b->setHoverTexID( inst->_block6 );
		b->setMouseDownTexture( inst->_block6 );
		break;
	case 7:
		b->setTextureID( inst->_block7 );
		b->setNormalTexID( inst->_block7 );
		b->setHoverTexID( inst->_block7 );
		b->setMouseDownTexture( inst->_block7 );
		break;
	case 8:
		b->setTextureID( inst->_block8 );
		b->setNormalTexID( inst->_block8 );
		b->setHoverTexID( inst->_block8 );
		b->setMouseDownTexture( inst->_block8 );
		break;
	}

	if (!inst->CheckForFreeBlocks())
	{
		// The game has been WON!
		inst->_gameWon = true;
		inst->_start->setTextureID(inst->_startWon);
		inst->_start->setNormalTexID(inst->_startWon);
	}
}

void onBlockRClick( CButton* button )
{
	CMinesweeper* inst = CMinesweeper::Inst();
	
	vector< vector< CBlock* > >& blks = inst->getBlocks();
	CBlock* b = (CBlock*)button;

	if( b->Clicked() || inst->_dead || inst->_gameWon )
		return;

	if( !b->Flagged() )
	{
		--inst->numBombs;
		b->setFlagged( true );
		b->setTextureID( inst->_flagged );
		b->setNormalTexID( inst->_flagged );
		b->setHoverTexID( inst->_flagged );
		b->setMouseDownTexture( inst->_flagged );
	}
	else
	{
		++inst->numBombs;
		b->setFlagged( false );
		b->setTextureID( inst->_hover );
		b->setNormalTexID( inst->_normal );
		b->setHoverTexID( inst->_hover );
		b->setMouseDownTexture( inst->_disabled );
	}
}

void onBlockDblClick( CButton* button )
{
	CMinesweeper* inst = CMinesweeper::Inst();
	
	vector< vector< CBlock* > >& blks = inst->getBlocks();
	CBlock* b = (CBlock*)button;

	if( !b->Clicked() || inst->_dead || inst->_gameWon)
		return;

	int x = b->getCol();
	int y = b->getRow();

	// Find out how many flags are around it
	// and how many bombs are around the block
	int numFlags = 0;
	int numBombs = 0;
	
	vector< CBlock* > surroundingBlocks;
	CBlock* temp = nullptr;

	if( ( temp = BlockAt( blks,x-1,y-1 )) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x,y-1 )  ) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x+1,y-1 )) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x+1,y )  ) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x+1,y+1 )) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x,y+1 )  ) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x-1,y+1 )) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );
	if( ( temp = BlockAt( blks,x-1,y )  ) && ( !temp->Clicked() || temp->Flagged() ) ) surroundingBlocks.push_back( temp );

	for( unsigned i = 0;i < surroundingBlocks.size();++i )
	{
		if( surroundingBlocks[i]->HasBomb() )
			++numBombs;
		if( surroundingBlocks[i]->Flagged() )
			++numFlags;
	}

	// Don't do anything if the correct number of flags aren't around it
	if( numFlags < numBombs || numFlags > numBombs )
		return;

	for( unsigned i = 0;i < surroundingBlocks.size();++i )
	{
		onBlockClick( surroundingBlocks[i] );
	}
}

void onStartButtonClick( CButton* button )
{
	CMinesweeper* inst = CMinesweeper::Inst();

	if (!inst->_firstClick && !inst->_gameWon && !inst->_dead)
	{
		// Show Message window
		inst->DisplayResetWarning(true);
	}
	else
		inst->Reset();
}
void onWarningMessageYesClick(CButton* button)
{
	CMinesweeper* inst = CMinesweeper::Inst();

	inst->DisplayResetWarning(false);
	inst->Reset();
}
void onWarningMessageNoClick(CButton* button)
{
	CMinesweeper* inst = CMinesweeper::Inst();

	inst->DisplayResetWarning(false);
}

void onBombCountAddClick(CButton* button)
{
	CMinesweeper* inst = CMinesweeper::Inst();

	if (inst->getMaxBombs() < inst->getNumBlocks() - 1)
	{
		inst->setMaxBombs(inst->getMaxBombs() + 1);
		inst->UpdateBombCountLabel();
	}

}
void onBombCountSubtractClick(CButton* button)
{
	CMinesweeper* inst = CMinesweeper::Inst();

	if (inst->getMaxBombs() > 1)
	{
		inst->setMaxBombs(inst->getMaxBombs() - 1);
		inst->UpdateBombCountLabel();
	}
}
void onBombCountOK_Click(CButton* button)
{
	CMinesweeper* inst = CMinesweeper::Inst();

	inst->DisplayBombCountPopup(false);

	if (!inst->_firstClick && !inst->_gameWon && !inst->_dead)
		inst->DisplayResetWarning(true);
	else
		inst->Reset();
}
void onBombCountCancelClick(CButton* button)
{
	CMinesweeper* inst = CMinesweeper::Inst();

	inst->DisplayBombCountPopup(false);
}
void onBombCountClick(CButton* button)
{
	CMinesweeper* inst = CMinesweeper::Inst();

	inst->DisplayBombCountPopup(true);
}

CMinesweeper::CMinesweeper()
{
	_timeFormat = nullptr;

	_pWarningMessageWindow = nullptr;
	_pBombCountWindow = nullptr;
	_pBombTextLabel = nullptr;
	_pBombCountButton = nullptr;

	_maxBombs = 50;
	_numBlocks = 0;
}
CMinesweeper::~CMinesweeper()
{
	delete [] _timeFormat;
}

void CMinesweeper::GameStateInit( CGameState* pState )
{
	Graphics::CRenderSet2D& set = pState->get2DRenderSet();
	CWindowManager& wm = pState->getWindowManager();
	D3D9* d3d = CEngine::getD3D9();
	
	_maxBombs = 50;

	// Load textures that will be used
	_normal = d3d->LoadTexture( "Assets/Textures/BlockNormal.png" );
	_hover = d3d->LoadTexture( "Assets/Textures/BlockHover.png" );
	_disabled = d3d->LoadTexture( "Assets/Textures/BlockDisabled.png" );
	_flagged = d3d->LoadTexture( "Assets/Textures/BlockFlagged.png" );
	_mine = d3d->LoadTexture( "Assets/Textures/BlockMine.png" );
	_block1 = d3d->LoadTexture( "Assets/Textures/Block1.png" );
	_block2 = d3d->LoadTexture( "Assets/Textures/Block2.png" );
	_block3 = d3d->LoadTexture( "Assets/Textures/Block3.png" );
	_block4 = d3d->LoadTexture( "Assets/Textures/Block4.png" );
	_block5 = d3d->LoadTexture( "Assets/Textures/Block5.png" );
	_block6 = d3d->LoadTexture( "Assets/Textures/Block6.png" );
	_block7 = d3d->LoadTexture( "Assets/Textures/Block7.png" );
	_block8 = d3d->LoadTexture( "Assets/Textures/Block8.png" );
	_mineClicked = d3d->LoadTexture( "Assets/Textures/BlockMineClicked.png" );

	RECT src = { 0,0,32,32 };

	int width = (int)d3d->getBackBufferWidth()/32;
	int height = (int)d3d->getBackBufferHeight()/32 - 1;
	int totalWidth = width*32 + 32;
	int totalHeight = height*32 + 32;

	if( totalHeight + 32 > d3d->getBackBufferHeight() )
		height -= int((totalHeight + 32) - d3d->getBackBufferHeight())/32;
	if( totalWidth > d3d->getBackBufferHeight() )
		width -= int(totalWidth - d3d->getBackBufferWidth())/32;

	for( int i = 0;i < width;++i )
	{
		vector< CBlock* > blocks;

		for( int j = 0; j < height;++j )
		{
			CBlock* b = new CBlock();
			
			b->setWidth( 32 );
			b->setHeight( 32 );
			b->setTextureID( _normal );
			b->setSrcRect( src );
			b->setNormalTexID( _normal );
			b->setHoverTexID( _hover );
			b->setMouseDownTexture( _disabled );
			b->setNormalSrc( src );
			b->setHoverSrc( src );
			b->setMouseDownSrc( src );
			b->setPos( float(i*(1+b->getWidth())) - 1,float((1+j)*(1+b->getHeight())) - 1 );
			b->setRow( j );
			b->setCol( i );
			b->setOnClick( onBlockClick );
			b->setOnRClick( onBlockRClick );
			b->setOnDblClick( onBlockDblClick );
			b->setOnMClick( onBlockDblClick );

			wm.AddWindow( b );
			set.Add( b,RO_2 );
			blocks.push_back( b );
		}

		_blocks.push_back( blocks );
	}

	inst->_fontID = d3d->LoadFont( "Verdana",16,8 );

	int menuTex = d3d->LoadTexture( "Assets/Textures/MenuBar.png" );
	RECT src2 = { 0,0,1024,32 };

	CWindowComponent* wc = wm.NewWindow( WT_Window,(int)d3d->getBackBufferWidth(),32,menuTex,src2,menuTex,src2 );
	wc->setPos( -1,-1 );
	wc->setScaleX( d3d->getBackBufferWidth()/1024.0f );
	set.Add( wc,RO_3 );

	int start		= d3d->LoadTexture( "Assets/Textures/StartButton.png" );
	int startHover	= d3d->LoadTexture( "Assets/Textures/StartButtonHover.png" );
	int startDown	= d3d->LoadTexture( "Assets/Textures/StartButtonDown.png" );
	int startWon    = d3d->LoadTexture( "Assets/Textures/StartButtonWon.png");
	int startDead	= d3d->LoadTexture( "Assets/Textures/StartButtonDead.png" );

	// Reset Game Button
	CButton* startButton = (CButton*)wm.NewWindow( WT_Button,32,32,start,src,startHover,src );
	startButton->setOnClick( onStartButtonClick );
	startButton->setMouseDownTexture( startDown );
	startButton->setMouseDownSrc( src );
	startButton->setPos( d3d->getBackBufferWidth()/2 - startButton->getWidth()/2, -1 );
	startButton->Enabled(true);
	set.Add( startButton,RO_4 );

	inst->_start = startButton;
	inst->_startNorm = start;
	inst->_startDead = startDead;
	inst->_startWon = startWon;

	SetUpMessageWindow(pState);
	SetUpBombCountWindow(pState);

	int mineTex = d3d->LoadTexture("Assets/Textures/MineButton.png");
	int mineHoverTex = d3d->LoadTexture("Assets/Textures/MineButtonHover.png");

	RECT mineSrc = { 0,0,30,30 };

	_pBombCountButton = (CButton*)wm.NewWindow(WT_Button, 30, 30, mineTex, src, mineHoverTex, src);

	_pBombCountButton->setOnClick(onBombCountClick);
	_pBombCountButton->setMouseDownTexture(_mine);
	_pBombCountButton->setMouseDownSrc(src);
	_pBombCountButton->setPos(16, 0);
	_pBombCountButton->Enabled(true);
	set.Add(_pBombCountButton,RO_4);
}

void CMinesweeper::SetUpMessageWindow(Engine::CGameState* pState)
{
	Graphics::CRenderSet2D& set = pState->get2DRenderSet();
	CWindowManager& wm = pState->getWindowManager();
	D3D9* d3d = CEngine::getD3D9();

	int bgTexture = d3d->LoadTexture("Assets/Textures/MessageWindowBG.png");
	int buttonYesTex = d3d->LoadTexture("Assets/Textures/YesButton.png");
	int buttonYesHoverTex = d3d->LoadTexture("Assets/Textures/YesButtonHover.png");
	int buttonNoTex = d3d->LoadTexture("Assets/Textures/NoButton.png");
	int buttonNoHoverTex = d3d->LoadTexture("Assets/Textures/NoButtonHover.png");

	// srcRect to use for the bgTexture
	RECT bgSrc = { 0,0,384,128 };
	RECT buttonSrc = { 0,0,64,24 };

	// Warning message popup window
	_pWarningMessageWindow = wm.NewWindow(WT_Window, 384, 128, bgTexture, bgSrc, bgTexture, bgSrc);
	_pWarningMessageWindow->Enabled(true);

	_pWarningMessageWindow->setPos(152, 80);

	// Buttons for the popup window
	CButton* yesButton = (CButton*)wm.NewWindow(WT_Button, 65, 25, buttonYesTex, buttonSrc, buttonYesHoverTex, buttonSrc);
	CButton* noButton = (CButton*)wm.NewWindow(WT_Button, 65, 25, buttonNoTex, buttonSrc, buttonNoHoverTex, buttonSrc);
	
	yesButton->setOnClick(onWarningMessageYesClick);
	yesButton->setPos(_pWarningMessageWindow->getPos().x + 110, _pWarningMessageWindow->getPos().y + 82);
	yesButton->setMouseDownTexture(buttonYesTex);
	yesButton->setMouseDownSrc(buttonSrc);
	yesButton->Enabled(true);

	noButton->setOnClick(onWarningMessageNoClick);
	noButton->setPos( yesButton->getPos().x + noButton->getWidth() + 32, _pWarningMessageWindow->getPos().y + 82);
	noButton->setMouseDownTexture(buttonNoTex);
	noButton->setMouseDownSrc(buttonSrc);
	noButton->Enabled(true);

	// Text for the popup window
	CTextComponent* textLabel = (CTextComponent*)wm.NewWindow(WT_Text, 256, 32, -1, bgSrc, -1, bgSrc);

	textLabel->setText("Are you sure you want to restart?");
	textLabel->setPos(_pWarningMessageWindow->getPos().x + 72, _pWarningMessageWindow->getPos().y + 32);
	textLabel->setFontID(_fontID);
	textLabel->Enabled(true);

	_pWarningMessageWindow->AddChild(yesButton);
	_pWarningMessageWindow->AddChild(noButton);
	_pWarningMessageWindow->AddChild(textLabel);

	set.Add(_pWarningMessageWindow, RO_3);
	set.Add(yesButton, RO_4);
	set.Add(noButton, RO_4);
	set.Add(textLabel, RO_4);

	DisplayResetWarning(false);
}

void CMinesweeper::SetUpBombCountWindow(Engine::CGameState * pState)
{
	Graphics::CRenderSet2D& set = pState->get2DRenderSet();
	CWindowManager& wm = pState->getWindowManager();
	D3D9* d3d = CEngine::getD3D9();

	int bgTexture = d3d->LoadTexture("Assets/Textures/MessageWindowBG.png");
	int buttonAddTex = d3d->LoadTexture("Assets/Textures/AddButton.png");
	int buttonAddHoverTex = d3d->LoadTexture("Assets/Textures/AddButtonHover.png");
	int buttonSubtractTex = d3d->LoadTexture("Assets/Textures/SubtractButton.png");
	int buttonSubtractHoverTex = d3d->LoadTexture("Assets/Textures/SubtractButtonHover.png");
	int buttonOKTex = d3d->LoadTexture("Assets/Textures/OKButton.png");
	int buttonOKHoverTex = d3d->LoadTexture("Assets/Textures/OKButtonHover.png");
	int buttonCancelTex = d3d->LoadTexture("Assets/Textures/CancelButton.png");
	int buttonCancelHoverTex = d3d->LoadTexture("Assets/Textures/CancelButtonHover.png");

	// srcRect to use for the bgTexture
	RECT bgSrc = { 0,0,384,128 };
	RECT buttonSrc = { 0,0,64,24 };

	// Warning message popup window
	_pBombCountWindow = wm.NewWindow(WT_Window, 384, 128, bgTexture, bgSrc, bgTexture, bgSrc);
	_pBombCountWindow->Enabled(true);

	_pBombCountWindow->setPos(152, 80);

	// Buttons for the popup window
	CButton* addButton = (CButton*)wm.NewWindow(WT_Button, 65, 25, buttonAddTex, buttonSrc, buttonAddHoverTex, buttonSrc);
	CButton* subtractButton = (CButton*)wm.NewWindow(WT_Button, 65, 25, buttonSubtractTex, buttonSrc, buttonSubtractHoverTex, buttonSrc);
	CButton* okButton = (CButton*)wm.NewWindow(WT_Button, 65, 25, buttonOKTex, buttonSrc, buttonOKHoverTex, buttonSrc);
	CButton* cancelButton = (CButton*)wm.NewWindow(WT_Button, 65, 25, buttonCancelTex, buttonSrc, buttonCancelHoverTex, buttonSrc);

	subtractButton->setOnClick(onBombCountSubtractClick);
	subtractButton->setPos(_pBombCountWindow->getPos().x + 85, _pBombCountWindow->getPos().y + 50);
	subtractButton->setMouseDownTexture(buttonSubtractTex);
	subtractButton->setMouseDownSrc(buttonSrc);
	subtractButton->Enabled(true);

	addButton->setOnClick(onBombCountAddClick);
	addButton->setPos(subtractButton->getPos().x + addButton->getWidth() + 76, _pBombCountWindow->getPos().y + 50);
	addButton->setMouseDownTexture(buttonAddTex);
	addButton->setMouseDownSrc(buttonSrc);
	addButton->Enabled(true);

	okButton->setOnClick(onBombCountOK_Click);
	okButton->setPos(_pBombCountWindow->getPos().x + 118, _pBombCountWindow->getPos().y + 90);
	okButton->setMouseDownTexture(buttonOKTex);
	okButton->setMouseDownSrc(buttonSrc);
	okButton->Enabled(true);

	cancelButton->setOnClick(onBombCountCancelClick);
	cancelButton->setPos(okButton->getPos().x + okButton->getWidth() + 24, _pBombCountWindow->getPos().y + 90);
	cancelButton->setMouseDownTexture(buttonCancelTex);
	cancelButton->setMouseDownSrc(buttonSrc);
	cancelButton->Enabled(true);

	// Text for the popup 
	CTextComponent* label = (CTextComponent*)wm.NewWindow(WT_Text, 256, 32, -1, bgSrc, -1, bgSrc);
	_pBombTextLabel = (CTextComponent*)wm.NewWindow(WT_Text, 256, 32, -1, bgSrc, -1, bgSrc);

	// Bomb Count Display
	string text = "50";
	_pBombTextLabel->setText(text);
	_pBombTextLabel->setPos(subtractButton->getPos().x + subtractButton->getWidth() + 28, _pBombCountWindow->getPos().y + 52);
	_pBombTextLabel->setFontID(_fontID);
	_pBombTextLabel->Enabled(true);

	// Message to display to the user
	label->setText("Select the amount of bombs");
	label->setPos(subtractButton->getPos().x -5, _pBombCountWindow->getPos().y + 20);
	label->setFontID(_fontID);
	label->Enabled(true);

	// Add the children to the bomb count window
	_pBombCountWindow->AddChild(addButton);
	_pBombCountWindow->AddChild(subtractButton);
	_pBombCountWindow->AddChild(okButton);
	_pBombCountWindow->AddChild(cancelButton);
	_pBombCountWindow->AddChild(_pBombTextLabel);
	_pBombCountWindow->AddChild(label);

	set.Add(_pBombCountWindow, RO_3);
	set.Add(addButton, RO_4);
	set.Add(subtractButton, RO_4);
	set.Add(okButton, RO_4);
	set.Add(cancelButton, RO_4);
	set.Add(_pBombTextLabel, RO_4);
	set.Add(label, RO_4);

	DisplayBombCountPopup(false);
}

void CMinesweeper::EnableButtons(bool bEnable)
{
	for (unsigned i = 0; i < _blocks.size(); ++i)
	{
		for (unsigned j = 0; j < _blocks[i].size(); ++j)
		{
			if (bEnable)
				_blocks[i][j]->Enabled(!_blocks[i][j]->Clicked());
			else
				_blocks[i][j]->Enabled(bEnable);
		}
	}

	if(_start)
		_start->Enabled(bEnable);

	if(_pBombCountButton)
		_pBombCountButton->Enabled(bEnable);
}

void CMinesweeper::DisplayResetWarning(bool bDisplay)
{
	_pWarningMessageWindow->Enabled(bDisplay);
	_pWarningMessageWindow->Visible(bDisplay);

	EnableButtons(!bDisplay);
}

void CMinesweeper::DisplayBombCountPopup(bool bDisplay)
{
	_pBombCountWindow->Enabled(bDisplay);
	_pBombCountWindow->Visible(bDisplay);

	EnableButtons(!bDisplay);
	UpdateBombCountLabel();
}

void CMinesweeper::UpdateBombCountLabel()
{
	ostringstream bombs;
	bombs << _maxBombs;

	_pBombTextLabel->setText(bombs.str());
}

bool CMinesweeper::CheckForFreeBlocks()
{
	return _clickedBlocks.size() < ( _blocks.size() * _blocks[0].size() - _maxBombs );
}
