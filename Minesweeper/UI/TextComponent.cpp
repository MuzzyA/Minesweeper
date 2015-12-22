#include "TextComponent.h"
#include "../Engine/Engine.h"
#include "../API Wrappers/D3D9.h"



namespace UI
{
	void CTextComponent::Render() const
	{
		if (isVisible())
		{
			CWindowComponent::Render();

			Engine::CEngine::getD3D9()->DrawFont(_fontID, _text, (int)getPos().x, (int)getPos().y);
		}
	}
	CTextComponent::CTextComponent() : CWindowComponent()
	{
	}
	CTextComponent::~CTextComponent()
	{
	}
}