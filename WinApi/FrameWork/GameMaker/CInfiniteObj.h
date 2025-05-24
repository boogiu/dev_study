#pragma once
class CCamera;

class CInfiniteObj
{
public:
    CInfiniteObj();
    ~CInfiniteObj();
public:
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();

public:
    void LoadSprite(const TCHAR* ImgKey);
    void Set_SpriteScale(int _x, int _y) { m_iSpWidth = _x; m_iSpHeight = _y; };
    void Set_MoveSpeed(float _speed) { m_fMoveSpeed = _speed; }
    void Set_Info(INFO _info) { m_tInfo.Translate_Info(_info); }
    void Set_Info(INFO_EX _info) { m_tInfo= _info; }
    void Set_Index(int _index) { m_iIndex = _index; };
    INFO_EX& Get_Info() { return m_tInfo; };
    bool Get_isOut() { return m_bIsOut; };
    VECTOR2 Get_Vector() { return m_tVector; };

private:
    bool m_bIsOut;
    int m_iIndex;

    int m_iSpWidth;
    int m_iSpHeight;

    float m_fMoveSpeed;
    float m_fMovePointX;
    float m_fMovePointY;

    POINT m_prevPoint;
    HDC memDC;

    CCamera* m_pCam;
    VECTOR2 m_tVector;
    INFO_EX m_tInfo;
};

