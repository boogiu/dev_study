#include "Loader_Defines.h"
#include "ModelLevel.h"

#include "GameInstance.h"
#include "IProtoService.h"
#include "ICameraService.h"
#include "IObjectService.h"

#include "ModelObject.h"
#include "Free_Camera.h"
#include "PartsObject.h"
#include "OptionUI.h"

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
	IObjectService* pObjMgr = m_pGameInstance->Get_ObjectMgr();
	pProtoMgr->Add_ProtoType(m_LevelKey, "Proto_GameObject_Model", CModelObject::Create());
	pProtoMgr->Add_ProtoType(m_LevelKey, "Proto_GameObject_Camera", CFree_Camera::Create());
	pProtoMgr->Add_ProtoType(m_LevelKey, "Proto_GameObject_Part", CPartsObject::Create());
	pProtoMgr->Add_ProtoType(m_LevelKey, "Proto_GameObject_OptionUI", COptionUI::Create());
	
	CGameObject* BaseModel = Builder::Create_Object({ m_LevelKey ,"Proto_GameObject_Model" })
		.Build("Model");

	CGameObject* Camera = Builder::Create_Object({ m_LevelKey ,"Proto_GameObject_Camera" })
		.Camera({ (float)g_iWinSizeX / g_iWinSizeY })
		.Position({ 0,0,-10 })
		.Build("Main_Camera");

	CGameObject* Option = Builder::Create_Object({ m_LevelKey ,"Proto_GameObject_OptionUI" }).Build("Option");


	pObjMgr->Add_Object(BaseModel, { m_LevelKey,"Model_Layer" });
	pObjMgr->Add_Object(Camera, { m_LevelKey,"Model_Layer" });
	pObjMgr->Add_Object(Option, { m_LevelKey,"Optional_Layer" });
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
