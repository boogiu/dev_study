#ifndef _STARTSCENE_
#define _STARTSCENE_
#include "CScene.h"

class CStartScene : public CScene
{
public:
	CStartScene();
	~CStartScene() override;

	// CScene을(를) 통해 상속됨
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;
};

#endif // !_STARTSCENE_



