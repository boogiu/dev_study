#ifndef _SCENE_
#define _SCENE_

class CScene // 해당 씬은 부모의 씬으로 활용
{
public : 
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Release();
};

#endif // !_SCENE_


