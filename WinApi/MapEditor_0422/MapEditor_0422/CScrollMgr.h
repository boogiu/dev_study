#pragma once
class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();
	CScrollMgr(CScrollMgr& rhs) = delete;
	CScrollMgr& operator=(CScrollMgr& rhs) = delete;

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC _hDC);
	void Release();

public:
	void SetScrollX(float _x) { m_fScrollX += _x; }
	void SetScrollY(float _y) { m_fScrollY+= _y; }
	float GetScrollX() { return m_fScrollX; }
	float GetScrollY() { return m_fScrollY; }

public:
	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CScrollMgr;
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CScrollMgr* m_pInstance;
	float m_fScrollX;
	float m_fScrollY;
};

