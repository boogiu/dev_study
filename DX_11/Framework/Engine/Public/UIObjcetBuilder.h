#pragma once
#include "Engine_Defines.h"

NS_BEGIN(Engine)
class ENGINE_DLL CUIObjcetBuilder
{
    public:
        CUIObjcetBuilder(const CLONE_DESC& cloneDesc);
        ~CUIObjcetBuilder();
    public:
        class CUI_Object* Build(const string& instanceKey, _uint* id = nullptr);
        CUIObjcetBuilder& Add_Level(const string& Level);/*{level,layer}*/
    public:
        CUIObjcetBuilder& Set_Position(const _float2 position);
        CUIObjcetBuilder& Set_Rotate(const _float rotate);
        CUIObjcetBuilder& Set_Scale(const _float2 scale);
        CUIObjcetBuilder& Set_Anchor(UI_Anchor eAnchor, _float2 vPivot);

    private:
        class CGameInstance* m_pGameInstance = { nullptr };
        string m_LevelTag = {};
        CLONE_DESC* m_CloneDesc = { nullptr };
        UI_DESC* m_pObjDesc = { nullptr };

        _bool m_bPivoted = { false };
        UI_Anchor m_eAnchor = { UI_Anchor::Center};
        _float2 m_vPivot = {};
        unordered_map<type_index, COMPONENT_DESC*> m_CompDesc;
};

NS_END