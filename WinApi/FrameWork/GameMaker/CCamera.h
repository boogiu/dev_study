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
    LINE_INFO  WorldToScreen(const LINE_INFO& worldLINE) const;
    SIZE Get_CamSize() { return m_szScreen; }
    POINT Get_CamPoint() { return m_tPos; }
    bool Out_of_Camera( INFO_EX& Info);
    bool Out_of_Camera(RECT Info);
    POINT Get_Diff() { return m_tDiff; }
    void Camera_Reset();
    void Set_Border(int _x, int _y);
    void Cam_Shake(float _dur, bool Vertical, float amplitude);
    void Reset_Border() { m_CamBorder = { WINCX,WINCY }; }
    void MoveTo(POINT point) { m_tPos = point; }
    void Cinematic(wstring word, float duration);
private:
    void SetScreenSize(SIZE sz) { m_szScreen = sz; }
    void Calcul_Diff();
    void ShakingCam();
    void Render_Cinematic(HDC _hDC);

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
    bool m_bShake; //흔들리는가?
    bool m_bVertical; //수직 흔들림인가?
    float m_fDuration;//몇초 동안 흔들릴 것인가
    float ElapsedTime; //몇초 지났는가
    float m_fShakeAmplitude;
    float m_offsetX;
    float m_offsetY;
    float m_fShakeDelay; //몇초 단위로 흔들릴 것인가. 

    float m_CinemaTime;
    float m_CinemaElapsed;
    float m_cinemaOffset;
    bool m_bReadyToCinema;

    wstring m_CinemaStr;

    POINT m_CamBorder;
    CObject* m_pTarget;

    POINT m_tPos; //실제 포지션
    POINT m_tDiff; //화면상 좌표와 실제 좌표의 차이

    SIZE m_szScreen;
    TCHAR szBuffer[64];
    RECT tstRect;
};

