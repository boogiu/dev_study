#pragma once
class CObject;

class CCollision
{
public:
	CCollision();
	~CCollision();


public:
	bool CollisionByRect(const RECT& _dst, const RECT& _src);
	static bool CollisionByCircle(const INFO& _dst, const INFO& _src);

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

private:
	void UpdateByLayer(LAYER _dst, LAYER _src);
private:
	unordered_map<LAYER,vector<CObject*>> m_CollisionMap;
};

