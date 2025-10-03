#include "Editor_Defines.h"
#include "EditorSystem.h"
#include "GameInstance.h"

#include "IRayService.h"
#include "ICameraService.h"
#include "IObjectService.h"
#include "IProtoService.h"
#include "IResourceService.h"
#include "IGUIService.h"
#include "GUI_Context.h"
#include "IInputService.h"
#include "ITileService.h"

#include "GridObject.h"
#include "TileObject.h"
#include "EditorPanel.h"
#include "MaterialData.h"
#include "MaterialInstance.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "TileBlock.h"

IMPLEMENT_SINGLETON(CEditorSystem)

CEditorSystem::CEditorSystem()
{
	Initialize();
}

CEditorSystem::~CEditorSystem()
{
}

HRESULT CEditorSystem::Initialize()
{
	/*Managers*/
	m_pRayManager = CGameInstance::GetInstance()->Get_RayMgr();
	m_pViewMat = CGameInstance::GetInstance()->Get_CameraMgr()->Get_ViewMatrix();
	m_pProjMat = CGameInstance::GetInstance()->Get_CameraMgr()->Get_ProjMatrix();
	m_pInputService = CGameInstance::GetInstance()->Get_InputDev();
	m_pObjMgr = CGameInstance::GetInstance()->Get_ObjectMgr();
	m_pUIContext = CGameInstance::GetInstance()->Get_GUISystem()->Get_Context();

	/*그리드 오브젝트*/
	IProtoService* pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	Execute_TileSystem();

	/*패널 생성*/
	m_pPanel = CEditorPanel::Create(m_pUIContext);
	CGameInstance::GetInstance()->Get_GUISystem()->Register_Panel(m_pPanel);
	m_pPanel->Set_Grid(m_pGrid);


	/*타일 오브젝트 원형*/
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_Tile", CTileObject::Create());

	/*기본 타일 모델 로드*/
	CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath("Base_0.model", "../../Resources/Models/BaseTile/Base_0.model");
	CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath("Base_0.mat", "../../Resources/Models/BaseTile/Base_0.mat");

	return S_OK;
}

void CEditorSystem::Update(_float dt)
{
	Create_Ray();
	DragDrop_Object();
	Select_Object();
	m_pRayManager->Register_Ray(&m_tRay);
}

HRESULT CEditorSystem::Create_Object(const string& folderName)
{
	if (m_EditorContext.pEditingObject != nullptr) return E_FAIL;

	CGameObject* pTile = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Tile" })
		.Position({ 0,0,0 })
		.Scale({ 1,1,1 })
		.Build(folderName);

	string resourcPath = "../../Resources/Models/" + folderName;
	_bool hasMaterial = { false };
	_bool hasModel = { false };

	for (const auto& entry : filesystem::directory_iterator(resourcPath))
	{
		if (!entry.exists() || !entry.is_regular_file())
			continue;

		const string filePath = filesystem::weakly_canonical(entry.path()).string();
		string file = entry.path().filename().string();
		string ext = entry.path().extension().string();

		if (ext == ".model")
		{
			CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(file, filePath);
			pTile->Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, file);
			hasModel = true;
		}
		else if (ext == ".mat")
		{
			CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(file, filePath);
			pTile->Get_Component<CMaterial>()->Link_Material(G_GlobalLevelKey, file);
			hasMaterial = true;
		}
	}

	if (!hasMaterial || !hasModel)
		Safe_Release(pTile);

	else {
		m_EditorContext.pEditingObject = pTile;
		m_pObjMgr->Add_Object(pTile, { "Editor_Level","Tile_Layer" });
	}

	return S_OK;
}

HRESULT CEditorSystem::Create_ObjectArea(_fvector vEdgeMin, _cvector vEdgeMax)
{
	_uint minX, minY, minZ = {};
	m_pTileSystem->Get_IndexByPosition(vEdgeMin, &minX, &minY, &minZ);

	_uint maxX, maxY, maxZ = {};
	m_pTileSystem->Get_IndexByPosition(vEdgeMax, &maxX, &maxY, &maxZ);


	for (size_t i = minY; i < maxY+1; i++)
	{
		for (size_t j = minZ; j < maxZ; j++)
		{
			for (size_t k = minX; k < maxX; k++)
			{
				CGameObject* pTile = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Tile" }).Build("BaseTile");
				pTile->Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, "Base_0.model");
				pTile->Get_Component<CMaterial>()->Link_Material(G_GlobalLevelKey, "Base_0.mat");
				auto& instances = pTile->Get_Component<CMaterial>()->Get_Material_Instance();
				m_pObjMgr->Add_Object(pTile, { "Editor_Level","Tile_Layer" });
				static_cast<CTileObject*>(pTile)->Object_OnGrid(k, i, j);
			}
		}
	}

	return S_OK;
}

void CEditorSystem::Execute_TileSystem()
{
	TILESYSTEM_INFO info = {};

	info.iSizeXPerTile = 10.f;
	info.iSizeYPerTile = 15;
	info.iSizeZPerTile = 10.f;

	info.iTileCountX = 112;
	info.iTileCountY = 4;
	info.iTileCountZ = 96;

	info.OriginPoint = { 0,0,0,1 };

	CGameInstance::GetInstance()->Excute_TileSystem(info);
	m_pTileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_EditorContext.ContextTileInfo = info;

	_float GridXscale = static_cast<_float>(info.iTileCountX * info.iSizeXPerTile);
	_float GridZscale = static_cast<_float>(info.iTileCountZ * info.iSizeZPerTile);
	_float GridYPostion = static_cast<_float>(info.iSizeYPerTile);

	IProtoService* pProto = CGameInstance::GetInstance()->Get_PrototypeMgr();
	pProto->Add_ProtoType(G_GlobalLevelKey, "Proto_GameObject_Grid", CGridObject::Create());

	IObjectService* pObjMgr = CGameInstance::GetInstance()->Get_ObjectMgr();

	CGameObject* pGrid = Builder::Create_Object({ G_GlobalLevelKey, "Proto_GameObject_Grid" })
		.Position({ GridXscale / 2, 0, GridZscale / 2 })
		.Scale({ GridXscale ,	1	, GridZscale })
		.Build("name");
	pObjMgr->Add_Object(pGrid, { G_GlobalLevelKey,"Global_Layer" });

	m_pGrid = dynamic_cast<CGridObject*>(pGrid);
	Safe_AddRef(m_pGrid);
}

void CEditorSystem::Create_Ray()
{
	GetCursorPos(&m_MousePt);
	ScreenToClient(g_hWnd, &m_MousePt);

	/*마우스 좌표 변환*/
	_vector vMouseOrigin = XMVectorSet(
		static_cast<float>(m_MousePt.x),
		static_cast<float>(m_MousePt.y),
		0.f,
		1.f
	);

	/*마우스 레이 변환*/
	XMVECTOR raySrc = XMVector3Unproject(
		vMouseOrigin,
		0.0f, 0.0f,
		static_cast<float>(MapEditor::g_iWinSizeX),
		static_cast<float>(MapEditor::g_iWinSizeY),
		0.0f, 1.0f,
		XMLoadFloat4x4(m_pProjMat),
		XMLoadFloat4x4(m_pViewMat),
		XMMatrixIdentity()
	);

	/*마우스 목적지 좌표*/
	_vector vMouseDest = XMVectorSet(
		static_cast<float>(m_MousePt.x),
		static_cast<float>(m_MousePt.y),
		1.f,
		1.f
	);

	XMVECTOR rayDest = XMVector3Unproject(
		vMouseDest,
		0.0f, 0.0f,
		static_cast<float>(MapEditor::g_iWinSizeX),
		static_cast<float>(MapEditor::g_iWinSizeY),
		0.0f, 1.0f,
		XMLoadFloat4x4(m_pProjMat),
		XMLoadFloat4x4(m_pViewMat),
		XMMatrixIdentity()
	);

	XMVECTOR rayOrigin = raySrc;
	XMVECTOR rayDir = XMVector3Normalize(rayDest - raySrc);

	XMStoreFloat3(&m_tRay.vRayDirection, rayDir);
	XMStoreFloat3(&m_tRay.vRayOrigin, rayOrigin);
	m_tRay.fMaxDistance = 1550.f;
}

void CEditorSystem::DragDrop_Object()
{
	if (nullptr == m_EditorContext.pEditingObject) return;

	CGameObject* obj = m_EditorContext.pEditingObject;

	CTileObject* object = dynamic_cast<CTileObject*>(obj);

	if (m_pInputService->Mouse_Hold(MOUSE_BTN::LB)) {
		object->Get_Component<CTransform>()->Set_Pos({ m_pGrid->Get_HitPos() });
		object->Set_Selected(true);
	}
	if (m_pInputService->Mouse_Away(MOUSE_BTN::LB)) {
		_uint x, y, z = {};
		m_pGrid->Get_HitIndex(&x,&y,&z);
		object->Object_OnGrid(x, y, z);
		object->Set_Selected(false);
		m_pPanel->Notify_CreateComplete();
		m_EditorContext.pEditingObject = nullptr;
		//m_pUIContext->pSelectedObject = nullptr; /*지유아이랑 분리/
	}
}

void CEditorSystem::Select_Object()
{
	//이미 짚고 있는 것이 있으면 리턴
	if (nullptr != m_EditorContext.pEditingObject) return;
	if (CGameInstance::GetInstance()->Get_GUISystem()->UsingUI()) return;

	if (m_pInputService->Mouse_Down(MOUSE_BTN::LB)) {
		RAY_HIT* hitInfo = m_pRayManager->Get_FrontRayHit();
		if (hitInfo) {
			//m_EditorContext.pEditingObject = hitInfo->pObject;
			//CGameInstance::GetInstance()->Get_GUISystem().
			m_pUIContext->pSelectedObject = hitInfo->pObject;
		}
	}
}

void CEditorSystem::Free()
{
	__super::Free();
	Safe_Release(m_pGrid);
	Safe_Release(m_pPanel);
}
