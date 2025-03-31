#include "pch.h"
#include "CMazeScene.h"
#include "CSceneMgr.h"
#include "CGameMgr.h"

CMazeScene::CMazeScene()
	: player_x(0), player_y(0), lastKey(KEY::RIGHT), m_stage(1), Board{}, BuffBoard{}
{
}

CMazeScene::~CMazeScene()
{
}

void CMazeScene::Initialize()
{
	m_pPlayer = CGameMgr::GetInstance().GetPlayer();
	wmemset(&Board[0][0], L'□', 100);
	wmemset(&BuffBoard[0][0], L'□', 100);

	Board[player_y][player_x] = L'▶';
}

void CMazeScene::Update()
{
	
	MovePlayer();
	CopyBoard();
	EventCheck();
}

void CMazeScene::Render()
{
	wcout << m_stage << L"스테이지" << endl;
	wcout << L"------------------------------" << endl;
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			wcout <<L" " << Board[i][j];
		}
		wcout << endl;
	}
	wcout << L"--------------------------" << endl;
	wcout << L"ESC : 마을로 돌아가기" << endl;

}

void CMazeScene::Release()
{
}

void CMazeScene::MovePlayer()
{
	lastKey = CInput::GetKey();
	switch (lastKey)
	{
	case KEY::UP:
		if (player_y > 0) {
			player_y -= 1;
		}
		break;
	case KEY::DOWN:
		if (player_y < 9) {
			player_y += 1;
		}
		if (player_y == 9 && player_x == 9) {
			++m_stage;
			player_x = 0;
			player_y = 0;
		}
		break;
	case KEY::LEFT:
		if (player_x > 0) {
			player_x -= 1;
		}
		break;
	case KEY::RIGHT:
		if (player_x < 9) {
			player_x += 1;
		}
		break;
	case KEY::ESC:
		player_x = 0;
		player_y = 0;
		m_stage = 1;
		CSceneMgr::GetInstance().ChangeScene(SCENE::VILLAGE);
		break;
	default:
		break;
	}
}

void CMazeScene::CopyBoard()
{
	copy(&BuffBoard[0][0], &BuffBoard[0][0] + 100, &Board[0][0]);
	//▲ ▼ ◀ ▶
	if (lastKey == KEY::UP) {
		Board[player_y][player_x] = L'▲';
	}
	else if (lastKey == KEY::DOWN) {
		Board[player_y][player_x] = L'▼';
	}
	else if (lastKey == KEY::LEFT) {
		Board[player_y][player_x] = L'◀';
	}
	else if(lastKey ==KEY::RIGHT)
	{
		Board[player_y][player_x] = L'▶';
	}
}

void CMazeScene::EventCheck()
{
	// 시드값을 얻기 위한 random_device 생성.
	std::random_device rd;

	// random_device 를 통해 난수 생성 엔진을 초기화 한다.
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dis(0, 10);
	if (dis(gen) < 2) {
		//몬스터
		CSceneMgr::GetInstance().ChangeScene(SCENE::BATTLE);
	}
	else if (dis(gen)==4) {
		//보물 상자
	}
}
