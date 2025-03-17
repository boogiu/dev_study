#ifndef _SELECTSCENE_
#define _SELECTSCENE_
#include "CScene.h"

class CSelectScene  : public CScene
{
public:
	CSelectScene();
	~CSelectScene() override;

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;
};


#endif // !_SELECTSCENE_
