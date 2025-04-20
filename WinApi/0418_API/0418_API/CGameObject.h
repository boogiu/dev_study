#pragma once
class CGameObject
{
public: 
	CGameObject();
	virtual ~CGameObject();
public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update()PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release()PURE;

public:
	INFO GetInfo();
	void SetINFO(INFO _info);
	void SetPos(float _x, float _y);
	RECT GetRect();
	bool GetActive();
	void SetActive(bool _active);
protected:
	//중점 좌표와 크기를 기준으로 자신의 RECT를 업데이트 합니다.
	void Update_Rect();
protected:
	//오브젝트 활성화 여부
	bool m_bActive;
	//좌표와 크기
	INFO m_tInfo;
	//렌더링할 끝점
	RECT m_tRect;
};

