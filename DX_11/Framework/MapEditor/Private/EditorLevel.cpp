#include "Editor_Defines.h"
#include "EditorLevel.h"

#include "GameInstance.h"
#include "IProtoService.h"
#include "ICameraService.h"
#include "IObjectService.h"
#include "ICameraService.h"
#include "IResourceService.h"

#include "GridObject.h"
#include "Free_Camera.h"
#include "Camera.h"

CEditorLevel::CEditorLevel(const string& LevelKey)
    : CLevel{ LevelKey },
    m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CEditorLevel::Initialize()
{
	IObjectService* pObjMgr = m_pGameInstance->Get_ObjectMgr();
	CAMERA_DESC desc = {};

	CGameObject* Camera = Builder::Create_Object({ m_LevelKey ,"Proto_GameObject_Camera" })
		.Camera({ (float)g_iWinSizeX / g_iWinSizeY })
		.Position({ 0,3,-3 })
		.Build("Main_Camera");

	pObjMgr->Add_Object(Camera, { m_LevelKey,"Cameral_Layer" });
	m_pGameInstance->Get_CameraMgr()->Set_MainCam(Camera->Get_Component<CCamera>());

    return S_OK;
}

void CEditorLevel::Update()
{
}

HRESULT CEditorLevel::Render()
{
    return S_OK;
}

void CEditorLevel::PreLoad_Level()
{
	IProtoService* pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	pProto->Add_ProtoType("Editor_Level","Proto_GameObject_Camera",	CFree_Camera::Create());

	IResourceService* pService = CGameInstance::GetInstance()->Get_ResourceMgr();
	pService->Add_ResourcePath("TileCell.png", "../../Resources/TileCell.png");
	pService->Add_ResourcePath("VTX_PlaneGrid.hlsl", "../Bin/ShaderFiles/VTX_PlaneGrid.hlsl");
}

CEditorLevel* CEditorLevel::Create(const string& LevelKey)
{
	CEditorLevel* instance = new CEditorLevel(LevelKey);
	if (FAILED(instance->Initialize())) {
		MSG_BOX("CEditorLevel Create Failed");
		Safe_Release(instance);
	}

	return instance;
}

void CEditorLevel::Free()
{
	__super::Free();
	m_pGameInstance->DestroyInstance();
}
