#ifndef _GAMEPLAY_
#define _GAMEPLAY_

#include "CSceneManager.h"

class GamePlay
{

public:
	GamePlay();
	~GamePlay();

	void Initialize();
	void Update();
	void Release();
	void Render();

	enum GAMESTATE
	{
		START,
		END,
		PAUSE
	};

	GAMESTATE GetGameState() { return gameState; }

private:
	GAMESTATE gameState;
	CSceneManager* sceneManager;
};

#endif // !_GAMEPLAY_
