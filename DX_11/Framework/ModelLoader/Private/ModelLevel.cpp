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
#include "MaterialAdjust.h"

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
	pProtoMgr->Add_ProtoType(m_LevelKey, "Proto_GameObject_MaterialAdjust", CMaterialAdjust::Create());
	
	CGameObject* BaseModel = Builder::Create_Object({ m_LevelKey ,"Proto_GameObject_Model" })\
		.Scale({0.001f,0.001f,0.001f})
		.Build("Model");

	CGameObject* Camera = Builder::Create_Object({ m_LevelKey ,"Proto_GameObject_Camera" })
		.Camera({ (float)g_iWinSizeX / g_iWinSizeY })
		.Position({ 0,0,-30 })
		.Build("Main_Camera");

	CGameObject* Option = Builder::Create_Object({ m_LevelKey ,"Proto_GameObject_OptionUI" }).Build("Option");
	CGameObject* Adjust = Builder::Create_Object({ m_LevelKey ,"Proto_GameObject_MaterialAdjust" }).Build("Adjust");


	pObjMgr->Add_Object(BaseModel, { m_LevelKey,"Model_Layer" });
	pObjMgr->Add_Object(Camera, { m_LevelKey,"Model_Layer" });
	pObjMgr->Add_Object(Option, { m_LevelKey,"Optional_Layer" });
	pObjMgr->Add_Object(Adjust, { m_LevelKey,"Adjust_layer" });
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
