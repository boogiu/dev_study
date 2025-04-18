#pragma once
class CObject;
class CBorder;

class CScene
{
public :
	CScene();
	virtual ~CScene() PURE;

public:
	virtual void Initialize()	 PURE;
	virtual void Update()	 PURE;
	virtual void Render(HDC _hDC)	 PURE;
	virtual void Release()	 PURE;

protected:
	CObject* m_Player;
};

