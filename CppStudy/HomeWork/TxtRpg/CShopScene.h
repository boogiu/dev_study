#ifndef _SHOPSCENE_
#define _SHOPSCENE_
#include "CScene.h"

class CShopScene : public CScene
{
public:
	CShopScene();
	~CShopScene() override;

public:
	// CScene을(를) 통해 상속됨
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;
};

#endif // !_SHOPSCENE_
