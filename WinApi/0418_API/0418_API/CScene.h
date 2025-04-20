#pragma once
class CScene
{
public: 
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize() PURE;
	virtual void Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hDC)PURE;
	virtual void Release()PURE;

public:
	
};

