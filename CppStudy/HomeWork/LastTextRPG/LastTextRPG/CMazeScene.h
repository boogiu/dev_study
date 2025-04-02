#pragma once
#include "CScene.h"
class CMazeScene : public CScene
{
public: 
	CMazeScene();
	~CMazeScene() override;

public:
	void Initialize()override;
	void Update()override;
	void Render()override;
	void Release()override;

private:
	void MovePlayer();
	void CopyBoard();
	void EventCheck();
	void NextStage();
private:
	int player_x;
	int player_y;
	wchar_t Board[10][10];
	wchar_t BuffBoard[10][10];
	KEY lastKey;
	int m_stage;
	bool isFinished;
};

