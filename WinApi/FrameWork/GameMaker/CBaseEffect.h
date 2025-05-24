#pragma once
#include "CObject.h"
class CAnim;
class CAttackBox;
class CBaseEffect :
    public CObject
{
public:
    CBaseEffect();
   ~CBaseEffect();
public:
   void Initialize() override;
   void Update() override;
   void Late_Update() override;
   void Render(HDC _hDC) override;
   void Release() override;

public:
    void PlayAnim(wstring pFrameKey, ANI_FRAME _Frame);

   void Set_Frame(ANI_FRAME _Frame) { m_tFrame = _Frame; }
   ANI_FRAME Get_Frame() { return m_tFrame; }

   void Set_Anim(ANI_FRAME _Frame);
   CAnim* Get_Anim() { return m_pAnim; }

   void Set_Name(wstring name);
   wstring Get_Name() { return m_EffectName; }

   bool Is_AnimEnd() { return m_bAnimEnd; }
   void Set_Tag(EFFECT_TAG _tag) { m_eTag = _tag; }
   void Set_AutoDeaActive(bool deAct) { m_bAutoDective = deAct; }

   void Sync_AtkBox(CAttackBox* sync) { m_SyncBox = sync; };

   void Set_Layer(EFFECT_LAYER layer) { m_eLayer = layer; }
   EFFECT_LAYER Get_Layer() { return m_eLayer; }
protected:
    bool m_bAutoDective;
    bool m_bAnimEnd;
    ANI_FRAME m_tFrame;
    wstring m_EffectName;
    CAnim* m_pAnim;
    CAttackBox* m_SyncBox;
    EFFECT_TAG m_eTag;
    EFFECT_LAYER m_eLayer;
};

