#pragma once
#include "Engine_Defines.h"
#include "Build_Struct.h"

NS_BEGIN(Engine)
class ENGINE_DLL CBuilder
{
public:
    CBuilder(const CLONE_DESC& cloneDesc, _bool* result );
    ~CBuilder();
public:
    class CGameObject* Build(const string& instanceKey);
public:
    CBuilder& Add_Layer(const LAYER_DESC& layer);/*{level,layer}*/
    CBuilder& With_Transform(const TRANSFORM_DESC& transform = TRANSFORM_DESC());/*move, rotate*/
    CBuilder& Set_Position(const _float3 position);
    CBuilder& Set_Rotate(const _float3 rotate);
    CBuilder& Set_Scale(const _float3 scale);
    CBuilder& With_Camera(const CAMERA_DESC& camera);
    CBuilder& With_Collider(const COLLIDER_DESC& collider);

private:
    class CGameInstance* m_pGameInstance = { nullptr };
    LAYER_DESC* m_layerDesc = { nullptr };
    CLONE_DESC* m_CloneDesc = {nullptr};

    unordered_map<type_index, COMPONENT_DESC*> m_CompDesc;
};

NS_END