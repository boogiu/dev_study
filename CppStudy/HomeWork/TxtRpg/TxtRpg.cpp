#include "pch.h"
#include "CGameManager.h"

SCENE nowScene;
CBroker broker;

int main()
{
	//게임의 내부를 구성할 Manager객체 생성
	CGameManager gameManager;
	gameManager.Initialize();

	while (true) {//프로그램의 구동 : while을 통한 지속 실행

		//게임의 진행 :  단계를 밟아감.
		gameManager.Render();

		if (nowScene == END) {
			break;
		}
	}
	gameManager.Release();
}
