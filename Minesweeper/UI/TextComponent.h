#pragma once

#include "Window.h"


namespace UI
{
	class CTextComponent : public CWindowComponent
	{
		string _text;

		int _fontID;

	public:
		virtual void Render() const;

		void setText(const string& Text) { _text = Text; }
		const string& getText() const { return _text; }

		void setFontID(int nFontID) { _fontID = nFontID; }
		int getFontID() const { return _fontID; }

		CTextComponent();
		virtual ~CTextComponent();
	};
}