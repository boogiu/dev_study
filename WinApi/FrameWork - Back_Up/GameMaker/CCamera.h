#pragma once
class CObject;

class CCamera
{
public: 
	CCamera();
	~CCamera();

public:
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();

public:
    void Set_Target(CObject* _target);
    INFO_EX Get_RenderPos(INFO_EX Info);
    INFO_EX  WorldToScreen(const INFO_EX& worldRect) const;

private:
    void SetScreenSize(SIZE sz) { m_szScreen = sz; }
    void Calcul_Diff();

public:
    static CCamera* Get_Instance() {
        if (!m_pInstance) {
            m_pInstance = new CCamera;
        }
        return m_pInstance;
    }
    static void Destroy_Instance() {
        if (m_pInstance) {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    }

private:
    static CCamera* m_pInstance;

private:
    CObject* m_pTarget;

    POINT m_tPos; //실제 포지션
    POINT m_tDiff; //화면상 좌표와 실제 좌표의 차이

    SIZE m_szScreen;

    TCHAR szBuffer[64];
    RECT tstRect;
};
