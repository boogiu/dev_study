#pragma once
class CMain
{
public :
	CMain();
	~CMain();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	HDC m_hDC;
};

