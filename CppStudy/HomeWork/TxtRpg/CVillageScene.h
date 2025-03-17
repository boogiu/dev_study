#ifndef _VILLAGESCENE_
#define _VILLAGESCENE_
#include "CScene.h"

class CVillageScene : public CScene
{
public:
	CVillageScene();
	~CVillageScene() override;

public:
	void Initialize() override;
	void Update() override;
	void Render() override;
	void Release() override;
};


#endif // !_VILLAGESCENE_
