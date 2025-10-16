#include "Builder.h"
#include "GameInstance.h"

#include "GameObjectBuilder.h"
#include "UI_Object.h"
#include "IProtoService.h"
#include "ILevelService.h"
#include "IObjectService.h"

#include "Camera.h"
#include "Model.h"
#include "Light.h"

CGameObjectBuilder::CGameObjectBuilder(const CLONE_DESC& _cloneDesc)
	:m_pGameInstance(CGameInstance::GetInstance())
{
	if (!CGameInstance::GetInstance()->Get_LevelMgr()->Check_ValidateLevel(_cloneDesc.OriginLevel))
	{
		MSG_BOX("Origin Level Tag is Invalidate : Builder");
	}
	else {
		m_CloneDesc = new CLONE_DESC(_cloneDesc);
	}
	Safe_AddRef(m_pGameInstance);
}

CGameObjectBuilder::~CGameObjectBuilder()
{
	for (auto& pair : m_CompDesc)
		delete(pair.second);

	m_CompDesc.clear();
	Safe_Delete(m_CloneDesc);
	Safe_Delete(m_layerDesc);
	Safe_Delete(m_pObjDesc);

	Safe_Release(m_pGameInstance);
}

CGameObject* CGameObjectBuilder::Build(const string& instanceKey, _uint* id)
{
	if (!m_CloneDesc)
	{
		MSG_BOX("CLONE_DESC is missing : CGameObjectBuilder ");
		return nullptr;
	}

	//오브젝트 설명체 채우기
	if(!m_pObjDesc)
		m_pObjDesc = new GAMEOBJECT_DESC;

	m_pObjDesc->InstanceName = instanceKey;

	for (auto& pair : m_CompDesc)
		m_pObjDesc->CompDesc[pair.first] = pair.second;

	//프로토 매니저에서 가져오기
	CGameObject* instance = m_pGameInstance->Get_PrototypeMgr()->Clone_Prototype(
		m_CloneDesc->OriginLevel, m_CloneDesc->protoTag, 
		m_pObjDesc);

	if (!instance) {
		return nullptr;
	}
	/*즉 -> 클론 후에 레이어에서 삽입하고 있는 중임*/
	
	if (instance && id)
		*id = instance->Get_ObjectID();

	return instance;
}

CGameObjectBuilder& CGameObjectBuilder::Position(const _float3 position)
{
	auto iter = m_CompDesc.find(type_index(typeid(CTransform)));

	if (iter == m_CompDesc.end())
	{
		TRANSFORM_DESC* transformDesc = new TRANSFORM_DESC();
		iter = m_CompDesc.emplace(type_index(typeid(CTransform)), transformDesc).first;
	}

	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(iter->second);
	pDesc->vInitialPosition = position;
	return *this;
}

CGameObjectBuilder& CGameObjectBuilder::Rotate(const _float3 rotate)
{
	auto iter = m_CompDesc.find(type_index(typeid(CTransform)));


	if (iter == m_CompDesc.end())
	{
		TRANSFORM_DESC* transformDesc = new TRANSFORM_DESC();
		iter = m_CompDesc.emplace(type_index(typeid(CTransform)), transformDesc).first;
	}


	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(iter->second);
	pDesc->vInitialEulerVector = rotate;
	return *this;
}

CGameObjectBuilder& CGameObjectBuilder::Scale(const _float3 scale)
{
	auto iter = m_CompDesc.find(type_index(typeid(CTransform)));

	if (iter == m_CompDesc.end())
	{
		TRANSFORM_DESC* transformDesc = new TRANSFORM_DESC();
		iter = m_CompDesc.emplace(type_index(typeid(CTransform)), transformDesc).first;
	}

	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(iter->second);
	pDesc->vInitialScale = scale;
	return *this;
}

CGameObjectBuilder& CGameObjectBuilder::Add_ObjDesc(GAMEOBJECT_DESC* pArg)
{
	m_pObjDesc = pArg;
	return *this;
}

CGameObjectBuilder& CGameObjectBuilder::Camera(const CAMERA_DESC& camera)
{
	CAMERA_DESC* CameraDesc = new CAMERA_DESC(camera);
	m_CompDesc.emplace(type_index(typeid(CCamera)), CameraDesc);
	return *this;
}

CGameObjectBuilder& CGameObjectBuilder::Light(const LIGHT_INIT_DESC& light)
{
	LIGHT_INIT_DESC* LightDesc = new LIGHT_INIT_DESC(light);
	m_CompDesc.emplace(type_index(typeid(CLight)), LightDesc);
	return *this;
}
