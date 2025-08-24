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
    
    CBuilder& Add_Layer(const LAYER_DESC& layer);/*{level,layer}*/
    CBuilder& With_Transform(const TRANSFORM_DESC& transform);/*move, rotate*/
public:
    class CGameObject* Build(const string& instanceKey);
private:
    class CGameInstance* m_pGameInstance = { nullptr };
    LAYER_DESC* m_layerDesc = { nullptr };
    CLONE_DESC* m_CloneDesc = {nullptr};

    unordered_map<type_index, INIT_DESC*> m_CompDesc;
};

NS_END