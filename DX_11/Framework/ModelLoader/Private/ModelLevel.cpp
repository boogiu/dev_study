#include "Loader_Defines.h"
#include "ModelLevel.h"

#include "GameInstance.h"
#include "IProtoService.h"
#include "ICameraService.h"

#include "ModelObject.h"
#include "Free_Camera.h"
#include "Camera.h"

CModelLevel::CModelLevel(const string& LevelKey)
	: CLevel{ LevelKey },
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CModelLevel::Initialize()
{
	IProtoService* pProtoMgr = m_pGameInstance->Get_PrototypeMgr();
	pProtoMgr->Add_ProtoType(m_LevelKey, "Proto_GameObject_Model", CModelObject::Create());
	pProtoMgr->Add_ProtoType(m_LevelKey, "Proto_GameObject_Camera", CFree_Camera::Create());
	
	Builder::Create_Object({ m_LevelKey ,"Proto_GameObject_Model" })
		.Add_To_Layer({ m_LevelKey,"Model_Layer" })
		.Build("Model");

	CGameObject* Camera = Builder::Create_Object({ m_LevelKey ,"Proto_GameObject_Camera" })
		.Add_To_Layer({ m_LevelKey,"Model_Layer" })
		.Camera({ (float)g_iWinSizeX / g_iWinSizeY })
		.Position({ 0,10,0 })
		.Build("Main_Camera");

	m_pGameInstance->Get_CameraMgr()->Set_MainCam(Camera->Get_Component<CCamera>());

	return S_OK;
}

void CModelLevel::Update()
{
}

HRESULT CModelLevel::Render()
{
	return S_OK;
}

void CModelLevel::PreLoad_Level()
{
}

CModelLevel* CModelLevel::Create(const string& LevelKey)
{
	CModelLevel* instance = new CModelLevel(LevelKey);
	if (FAILED(instance->Initialize())) {
		MSG_BOX("Model level Create Failed");
		Safe_Release(instance);
	}

	return instance;
}

void CModelLevel::Free()
{
	m_pGameInstance->DestroyInstance();
	__super::Free();
}	
