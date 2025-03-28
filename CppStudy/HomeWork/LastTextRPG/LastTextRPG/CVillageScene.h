#pragma once
#include "CScene.h"
class CVillageScene:public CScene
{
public:
	CVillageScene();
	~CVillageScene() override;

public:
	void Initialize()override;
	void Update()override;
	void Render()override;
	void Release()override;
private:
	enum class STATE
	{
		FIRST,
		SECOND,
		QUEST
	};
private:
	STATE nowState;

};
