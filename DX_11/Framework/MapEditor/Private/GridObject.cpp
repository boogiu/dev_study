#include "Editor_Defines.h"
#include "GridObject.h"
/*Comp*/
#include "PlaneModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"
#include "RayReceiver.h"
/*Manager*/
#include "GameInstance.h"
#include "IInputService.h"
#include "DebugRender.h"
#include "ITileService.h"
#include "IGUIService.h"

/*ClientManager*/
#include "EditorSystem.h"

CGridObject::CGridObject()
	:m_pDevice{ CGameInstance::GetInstance()->Get_Device() }
	, m_pContext{ CGameInstance::GetInstance()->Get_Context() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGridObject::CGridObject(const CGridObject& rhs)
	:CGameObject(rhs), m_pDevice(rhs.m_pDevice), m_pContext(rhs.m_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGridObject::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	CPlaneModel* pModel = Add_Component<CPlaneModel>();
	Add_Component<CMaterial>();
	Add_Component<CRayReceiver>()->Set_ReturnType(false);
	return S_OK;
}

HRESULT CGridObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	CMaterial* pMaterial = Get_Component<CMaterial>();
	CMaterialInstance* customInstance = CMaterialInstance::Create_Handle("Grid_Material", "Opaque", m_pDevice);

	_uint Index = {};
	pMaterial->Insert_MaterialInstance(customInstance, &Index);
	
	auto MaterialDat =customInstance->Get_MaterialData();
	if(MaterialDat)
	MaterialDat->Link_Shader(G_GlobalLevelKey, "VTX_PlaneGrid.hlsl");
	customInstance->Get_MaterialData()->Link_Texture(G_GlobalLevelKey, "TileCell.png", TEXTURE_TYPE::ALBEDO);
	SHADER_PARAM ScaleXParam = {};
	ScaleXParam.iSize = sizeof(_uint);
	ScaleXParam.pData = &m_iScaleX;
	ScaleXParam.typeName = "uint";

	SHADER_PARAM ScaleZParam = {};
	ScaleZParam.iSize = sizeof(_uint);
	ScaleZParam.pData = &m_iScaleZ;
	ScaleZParam.typeName = "uint";

	customInstance->Set_Param("XScale", ScaleXParam);
	customInstance->Set_Param("ZScale", ScaleZParam);


	SHADER_PARAM vMinParam = {};
	vMinParam.iSize = sizeof(_float4);
	vMinParam.pData = &m_HittedArea.vEdgeMin;
	vMinParam.typeName = "float4";

	SHADER_PARAM vMaxParam = {};
	vMaxParam.iSize = sizeof(_float4);
	vMaxParam.pData = &m_HittedArea.vEdgeMax;
	vMaxParam.typeName = "float4";

	customInstance->Set_Param("vEdgeMin", vMinParam);
	customInstance->Set_Param("vEdgeMax", vMaxParam);

	m_NowIndex.IndexY = 0;

	return S_OK;
}

void CGridObject::Priority_Update(_float dt)
{
	/*자신의 크기 ()*/
	TILESYSTEM_INFO contextInfo = CEditorSystem::GetInstance()->Get_Context()->ContextTileInfo;

	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_DOWN)) {
		if (m_NowIndex.IndexY == 0)
			m_NowIndex.IndexY = 0;
		else
			m_NowIndex.IndexY -= 1;
	}

	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Tap(VK_UP)) {
		if (m_NowIndex.IndexY == contextInfo.iTileCountY-1)
			m_NowIndex.IndexY = contextInfo.iTileCountY - 1;
		else
			m_NowIndex.IndexY += 1;
	}

	Get_Component<CTransform>()->Set_Pos({ Get_Position().x, 
		static_cast<_float>(contextInfo.SizePerTile().y) * m_NowIndex.IndexY
		,Get_Position().z });

}

void CGridObject::Update(_float dt)
{
	CRayReceiver* pReceiver = Get_Component<CRayReceiver>();
	_bool isHit = { false };
	_float3 rayPos = pReceiver->Get_RayHittedPos(&isHit);

	if (isHit) {
		m_HittedPos = rayPos;
	}

	/*자신의 크기 ()*/
	TILESYSTEM_INFO contextInfo = CEditorSystem::GetInstance()->Get_Context()->ContextTileInfo;
	m_iScaleX = contextInfo.iTileCountX;
	m_iScaleZ = contextInfo.iTileCountZ;

	m_HittedIndex =CGameInstance::GetInstance()->Get_TileSystem()->Get_IndexByPosition({ m_HittedPos.x,m_HittedPos.y,m_HittedPos.z ,0});
	Check_Dragging(contextInfo);
}

void CGridObject::Late_Update(_float dt)
{
}

TILE_INDEX CGridObject::Get_HitIndex()
{
	return m_HittedIndex;
}

void CGridObject::Render_GUI()
{
	ImVec2 windowPos = ImVec2((float)g_iWinSizeX - 850, 0);
	ImGui::SetNextWindowPos(ImVec2(windowPos), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(340, 100), ImGuiCond_Once);
	ImGui::Begin("Grid", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
	ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Ray Hit : ");
	ImGui::SameLine();
	ImGui::InputFloat3("##Position", reinterpret_cast<float*>(&m_HittedPos), "%.2f", ImGuiInputTextFlags_ReadOnly);

	_int Idx[3] = {
			(m_HittedIndex.IndexX),
			(m_HittedIndex.IndexZ)
	};

	ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Hitted_Index : ");
	ImGui::SameLine();
	ImGui::InputInt2("##Index", Idx, ImGuiInputTextFlags_ReadOnly);

	_int yLayer = static_cast<_int>(m_HittedIndex.IndexY);
	ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Now Y Layer : ");
	ImGui::SameLine();
	ImGui::InputInt("##Index", &yLayer);

	ImGui::End();
}

void CGridObject::Check_Dragging(TILESYSTEM_INFO ContextInfo)
{
	/*Pivot을 잡고.*/
	/*현재 커서의 위치가 더 크면 민을 고정, 작으면 맥스를 고정*/

	/*시프트 드래그 */
	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Hold(VK_SHIFT)
		&&
		CGameInstance::GetInstance()->Get_InputDev()->Mouse_Hold(MOUSE_BTN::LB))
	{
		if (!isDragging) {
			m_DragPivotPos = m_HittedPos;
			isDragging = true;
		}
	}
	else if (!CGameInstance::GetInstance()->Get_InputDev()->Mouse_Away(MOUSE_BTN::LB)) {
		isDragging = false;
	}

	if (!isDragging) {
		m_HittedArea.vEdgeMax = {
			ContextInfo.vWorldMin.x + ContextInfo.SizePerTile().x * (m_HittedIndex.IndexX + 1),
			ContextInfo.vWorldMin.y,
			ContextInfo.vWorldMin.z + ContextInfo.SizePerTile().z * (m_HittedIndex.IndexZ + 1),
			1.f
		};

		m_HittedArea.vEdgeMin = {
					ContextInfo.vWorldMin.x + ContextInfo.SizePerTile().x  * m_HittedIndex.IndexX,
					ContextInfo.vWorldMin.y,
					ContextInfo.vWorldMin.z + ContextInfo.SizePerTile().z  * m_HittedIndex.IndexZ,
				1.f
		};
	}

	else {
		if (m_DragPivotPos.x > m_HittedPos.x) { //이전 위치가 현재 위치보다 크면-> 맥시멈 갱신 x
			m_HittedArea.vEdgeMin.x = ContextInfo.vWorldMin.x + ContextInfo.SizePerTile().x  * m_HittedIndex.IndexX;
		}
		else {//이전 위치가 현재 위치보다 작으면-> 미니멈 갱신 x
			m_HittedArea.vEdgeMax.x = ContextInfo.vWorldMin.x + ContextInfo.SizePerTile().x * (m_HittedIndex.IndexX + 1);
		}
		if (m_DragPivotPos.z > m_HittedPos.z) {
			m_HittedArea.vEdgeMin.z = ContextInfo.vWorldMin.z + ContextInfo.SizePerTile().z* m_HittedIndex.IndexZ;
		}
		else {
			m_HittedArea.vEdgeMax.z = ContextInfo.vWorldMin.z + ContextInfo.SizePerTile().z * (m_HittedIndex.IndexZ + 1);
		}
		m_HittedArea.vEdgeMax.y = ContextInfo.vWorldMin.y;
		m_HittedArea.vEdgeMin.y = ContextInfo.vWorldMin.y;
	}

}

CGridObject* CGridObject::Create()
{
	CGridObject* instance = new CGridObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CGridObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CGridObject::Clone(INIT_DESC* pArg)
{
	CGridObject* instance = new CGridObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CGridObject");
		Safe_Release(instance);
	}

	return instance;
}
void CGridObject::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
