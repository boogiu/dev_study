#include "pch.h"
#include "GamePlay.h"

int main()
{
	GamePlay mainGame;
	mainGame.Initialize();

	while (true) 
	{
		//게임 진행
		if (GamePlay::START == mainGame.GetGameState()) {
			mainGame.Update();
			mainGame.Render();
		}

		//게임 일시 중지
		else if (GamePlay::PAUSE == mainGame.GetGameState()) {
			continue;
		}

		//게임 중단
		else if (GamePlay::END == mainGame.GetGameState()) {
			break;
		}

	}
}
