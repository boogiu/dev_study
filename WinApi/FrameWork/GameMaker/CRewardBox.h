#pragma once
#include "CTile.h"

class CAnim;

class CRewardBox :
    public CTile
{
public:
    CRewardBox();
    CRewardBox(TILE_INFO _info);
    ~CRewardBox();
public:
    void Initialize();
    void Update();
    void Late_Update();
    void Render(HDC _hDC);
    void Release();
    void Set_Tile(TILE_INFO _info)override;
    void Active_Box() { m_bActivate = true; }
    bool Box_Opend() { return m_bOpen; }
private:
    CAnim* m_pAnim;
    ANI_FRAME m_tAnim;
    bool m_bActivate;
    bool m_bisOpened;
    bool m_bOpen;
};

