#pragma once


namespace Engine
{
	class CGame
	{
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		
		virtual void Update( float dt ) = 0;
		virtual void Render() = 0;

		CGame() { }
		virtual ~CGame() = 0 { };
	};
};