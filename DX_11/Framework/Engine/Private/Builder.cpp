#include "Builder.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "IProtoService.h"
#include "ILevelService.h"
#include "IObjectService.h"

CBuilder::CBuilder(const CLONE_DESC& _cloneDesc, _bool* result)
	:m_pGameInstance(CGameInstance::GetInstance())
{
	if (!CGameInstance::GetInstance()->Get_LevelMgr()->Check_ValidateLevel(_cloneDesc.OriginLevel))
	{
		MSG_BOX("Origin Level Tag is Invalidate : Builder");
		*result = false;
	}
	else {
		m_CloneDesc = new CLONE_DESC(_cloneDesc);
		*result = true;
	}
	Safe_AddRef(m_pGameInstance);
}

CBuilder::~CBuilder()
{
	for (auto& pair : m_CompDesc)
		delete(pair.second);

	m_CompDesc.clear();
	Safe_Delete(m_CloneDesc);
	Safe_Delete(m_layerDesc);

	Safe_Release(m_pGameInstance);
}


CBuilder& CBuilder::Add_Layer(const LAYER_DESC& layer)
{

	if (!m_pGameInstance->Get_LevelMgr()->Check_ValidateLevel(layer.DestLevel)) {
		MSG_BOX("Destination Level Tag is Invalidate : Builder");
		return *this;
	}

	m_layerDesc = new LAYER_DESC(layer);
	return *this;
}

CBuilder& CBuilder::With_Transform(const TRANSFORM_DESC& transform)
{
	TRANSFORM_DESC* transformDesc = new TRANSFORM_DESC(transform);
	m_CompDesc.emplace(type_index(typeid(CTransform)), transformDesc);
	return *this;
}

CGameObject* CBuilder::Build(const string& instanceKey)
{
	if (!m_CloneDesc)
	{
		MSG_BOX("CLONE_DESC is missing : Builder ");
		return nullptr; 
	}

	if (!m_layerDesc)
	{
		MSG_BOX("LAYER_DESC is missing : Builder ");
		return nullptr; 
	}

	//오브젝트 설명체 채우기
	CGameObject::GAMEOBJECT_DESC obj;
	obj.InstanceName = instanceKey;
	for(auto& pair : m_CompDesc)
		obj.m_CompDesc[pair.first] = pair.second;

	//프로토 매니저에서 가져오기
	CGameObject* instance = m_pGameInstance->Get_PrototypeMgr()->Clone_Prototype(m_CloneDesc->OriginLevel, m_CloneDesc->protoTag, &obj);
	if (!instance) {
		return nullptr;
	}

	//오브젝트 레이어에 삽입
	m_pGameInstance->Get_ObjectMgr()->Add_Object(instance, m_layerDesc);

	return instance;
}

