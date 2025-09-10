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
    CGameObjectBuilder& Add_Level(const LAYER_DESC& layer);/*{level,layer}*/
    CGameObjectBuilder& With_Camera(const CAMERA_DESC& camera);

    CGameObjectBuilder& Set_Position(const _float3 position);
    CGameObjectBuilder& Set_Rotate(const _float3 rotate);
    CGameObjectBuilder& Set_Scale(const _float3 scale);

private:
    class CGameInstance* m_pGameInstance = { nullptr };
    LAYER_DESC* m_layerDesc = { nullptr };
    CLONE_DESC* m_CloneDesc = {nullptr};
    GAMEOBJECT_DESC* m_pObjDesc = { nullptr };

    unordered_map<type_index, COMPONENT_DESC*> m_CompDesc;
};

NS_END