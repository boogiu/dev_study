#pragma once
class CInfiniteObj;

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize()PURE;
	virtual void Update()PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hDC)PURE;
	virtual void Release()PURE;
	virtual void CloseScene()PURE;
protected:
	vector<CInfiniteObj*> m_BackStage;
	vector<CInfiniteObj*> m_BackCutton;
	vector<CInfiniteObj*> m_BackBridge;
};

