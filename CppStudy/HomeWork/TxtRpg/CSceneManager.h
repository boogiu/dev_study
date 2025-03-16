#ifndef _SCENE_
#define _SCENE_

class CSceneManager
{
public:
	enum SCENE {
		MAIN,
		START,
		VILLAGE,
		SHOP,
		MOTEL,
		BATTLE,
		END
	};
public:
	CSceneManager();
	~CSceneManager();

	void Initialize();
	void Render(SCENE _scene);
	void Update();
	void Release();
	SCENE SetScene(SCENE _scene) { scene = _scene; }
	SCENE GetScene() { return scene; }
private:
	SCENE scene;
};

#endif // !_SCENE_


