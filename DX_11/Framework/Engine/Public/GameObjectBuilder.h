#pragma once
#include "Engine_Defines.h"
#include "Build_Struct.h"

NS_BEGIN(Engine)
class ENGINE_DLL CGameObjectBuilder
{
public:
    CGameObjectBuilder(const CLONE_DESC& cloneDesc);
    ~CGameObjectBuilder();
public:
    class CGameObject* Build (const string& instanceKey,_uint* id = nullptr);
    CGameObjectBuilder& Camera(const CAMERA_DESC& camera);
    CGameObjectBuilder& Light(const LIGHT_INIT_DESC& light);

    CGameObjectBuilder& Position(const _float3 position);
    CGameObjectBuilder& Rotate(const _float3 rotate);
    CGameObjectBuilder& Scale(const _float3 scale);
    CGameObjectBuilder& Add_ObjDesc(GAMEOBJECT_DESC* pArg);

private:
    class CGameInstance* m_pGameInstance = { nullptr };
    LAYER_DESC* m_layerDesc = { nullptr };
    CLONE_DESC* m_CloneDesc = {nullptr};
    GAMEOBJECT_DESC* m_pObjDesc = { nullptr };

    unordered_map<type_index, COMPONENT_DESC*> m_CompDesc;
};

NS_END