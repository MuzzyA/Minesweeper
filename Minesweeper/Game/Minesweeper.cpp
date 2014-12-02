#include "Minesweeper.h"
#include "../Engine/Engine.h"
#include "../UI/Button.h"
#include "../API Wrappers/D3D9.h"
#include "Block.h"

#include <sstream>

using namespace Engine;
using namespace UI;

CMinesweeper* CMinesweeper::inst = nullptr;

void GameStateInit( CGameState* pState,void* pData );
void onBlockClick( CButton* button ); // button - the block that was clicked
void onBlockRClick( CButton* button );
void onBlockDblClick( CButton* button );


void CMinesweeper::Reset()
{
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
		}
	}

	unsigned bombCount = numBombs = 99;
	_totalTime = 0;

	for( unsigned i = 0;i < bombCount; ++i )
	{
		int x = rand() % _blocks.size();
		int y = rand() % _blocks[x].size();

		if( !_blocks[ x ][ y ]->HasBomb() )
			_blocks[ x ][ y ]->HasBomb( true );
		else
			--i;
	}

	_firstClick = true;
	_dead = false;


	_start->setNormalTexID( _startNorm );
	_start->setTextureID( _startNorm );
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
	
	bombDisplay << "Bombs:" << numBombs;
	timeDisplay << "Time:" << (int)_totalTime;

	int bufferWidth = (int)CEngine::getD3D9()->getBackBufferWidth();

	CEngine::getD3D9()->DrawFont( _fontID,bombDisplay.str(),64,8 );
	CEngine::getD3D9()->DrawFont( _fontID,timeDisplay.str(),bufferWidth - ( 16*timeDisplay.str().size() ) - 32,8 );
}
void CMinesweeper::Update( float dt )
{
	if( !_firstClick && !_dead )
		_totalTime += dt;
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
	CBlock* b = (CBlock*)button;

	if( b->Flagged() || b->Clicked() || inst->_dead )
		return;

	if( inst->_firstClick )
	{
		inst->_firstClick = false;

		b->HasBomb( false );

		while( 1 )
		{
			int x = rand() % blks.size();
			int y = rand() % blks[x].size();

			if( !blks[ x ][ y ]->HasBomb() )
			{
				blks[ x ][ y ]->HasBomb( true );
				break;
			}
		}
	}

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
	
	b->setClicked( true );

	int x = b->getCol();
	int y = b->getRow();

	// See how many bombs are surrounding this block
	unsigned bombCount = 0;
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
		if( surroundingBlocks[i]->HasBomb() )
			++bombCount;
	

	if( bombCount == 0 )
	{
		// Disable the button
		b->Enabled( false );

		for( unsigned i = 0;i < surroundingBlocks.size();++i )
			onBlockClick( surroundingBlocks[i] );

		b->setTextureID( inst->_disabled );

	}

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
}

void onBlockRClick( CButton* button )
{
	CMinesweeper* inst = CMinesweeper::Inst();
	
	vector< vector< CBlock* > >& blks = inst->getBlocks();
	CBlock* b = (CBlock*)button;

	if( b->Clicked() )
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

	if( !b->Clicked() )
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

	inst->Reset();
}

CMinesweeper::CMinesweeper()
{
	
}
CMinesweeper::~CMinesweeper()
{

}

void CMinesweeper::GameStateInit( CGameState* pState )
{
	Graphics::CRenderSet2D& set = pState->get2DRenderSet();
	CWindowManager& wm = pState->getWindowManager();
	D3D9* d3d = CEngine::getD3D9();

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
	int startDead	= d3d->LoadTexture( "Assets/Textures/StartButtonDead.png" );

	CButton* startButton = (CButton*)wm.NewWindow( WT_Button,32,32,start,src,startHover,src );
	startButton->setOnClick( onStartButtonClick );
	startButton->setMouseDownTexture( startDown );
	startButton->setMouseDownSrc( src );
	startButton->setPos( d3d->getBackBufferWidth()/2 - startButton->getWidth()/2, -1 );
	set.Add( startButton,RO_4 );

	inst->_start = startButton;
	inst->_startNorm = start;
	inst->_startDead = startDead;
}