#pragma once

#include "../UI/Button.h"


class CBlock : public UI::CButton
{
	void onClick();

	int _col;
	int _row;

	bool _hasBomb;
	bool _flagged;
	bool _clicked;

public:
	void setClicked( bool v ) { _clicked = v; }
	bool Clicked() const { return _clicked; }
	void HasBomb( bool v ) { _hasBomb = v; }
	bool HasBomb() const { return _hasBomb; }
	void setFlagged( bool v ) { _flagged = v; }
	bool Flagged() const { return _flagged; }
	void setRow( int v ) { _row = v; }
	void setCol( int v ) { _col = v; }
	int getRow() const { return _row; }
	int getCol() const { return _col; }

	void func() { }
	CBlock();
	~CBlock();
};