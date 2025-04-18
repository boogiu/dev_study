#pragma once
class CGameObject;

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize()				PURE;
	virtual void Update()				PURE;
	virtual void Late_Update()	PURE;
	virtual void Render(HDC _hDC)				PURE;
	virtual void Release()				PURE;

public:
	CGameObject* GetPlayer() {	return m_pPlayer;};
	void SetPlayer(CGameObject* _pPlayer) { m_pPlayer = _pPlayer; };

protected:
	CGameObject* m_pPlayer;
	vector<CGameObject*> m_objPool;
};

