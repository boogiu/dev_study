#include "Editor_Defines.h"
#include "MapTileInstance.h"

#include "StaticModel.h"
#include "InstanceModel.h"

#include "Material.h"
#include "MaterialInstance.h"

#include "GameInstance.h"
#include "ITileService.h"
#include "IResourceService.h"
#include "Texture.h"
#include	"GameInstance.h"
#include "ITileService.h"
#include "EditorSystem.h"

CMapTileInstance::CMapTileInstance()
{
}

CMapTileInstance::CMapTileInstance(const CMapTileInstance& rhs)
	:CGameObject(rhs)
{

}

HRESULT CMapTileInstance::Initialize_Prototype()
{
	__super::Initialize();
	return S_OK;
}

HRESULT CMapTileInstance::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	ID3D11Device* pDevice = CGameInstance::GetInstance()->Get_Device();

	//	CInstanceModel::INSTANCE_INIT_DESC instanceDesc = {};
	//	instanceDesc.ElementCount = 5;
	//	instanceDesc.ElementKey = "Tile_Instancing";
	//	instanceDesc.instanceCount = 16000;
	//	instanceDesc.instanceStride = sizeof(INSTANCE_TILE);
	//	instanceDesc.pElementDesc = VTX_TILEINSTANCE::Elements;
	//	
	//	vector<CInstanceModel::INSTANCE_INIT_DESC> pVector;
	//	pVector.push_back(instanceDesc);
	//	
	//	CMaterial* pMaterial = Get_Component<CMaterial>();
	//	pMaterial->Link_Material(G_GlobalLevelKey, "Base_0.mat");
	//	for (auto& instance : pMaterial->Get_Material_Instance()) {
	//		instance->Override_Pass("Instancing");
	//	}
	//	Get_Component<CInstanceModel>()->Link_InstanceData(pDevice, pVector, G_GlobalLevelKey, "Base_0.model");
	//	Get_Component<CInstanceModel>()->Link_InstanceWithMesh(0, 0);
	//	Get_Component<CInstanceModel>()->Link_InstanceWithMesh(1, 0);

	m_Tiles.reserve(16000);

	return S_OK;
}

void CMapTileInstance::Priority_Update(_float dt)
{
}

void CMapTileInstance::Update(_float dt)
{
	
}

void CMapTileInstance::Late_Update(_float dt)
{
}

void CMapTileInstance::Add_Tile(_float4x4 matrix, _float4 materialType)
{
	INSTANCE_TILE tile = {};
	tile.vRight = { matrix._11, matrix._12, matrix._13, matrix._14 };
	tile.vUp = { matrix._21, matrix._22, matrix._23, matrix._24 };
	tile.vLook = { matrix._31, matrix._32, matrix._33, matrix._34 };
	tile.vTranslation = { matrix._41, matrix._42, matrix._43, matrix._44 };
	tile.vMaterialType = materialType;

	m_Tiles.push_back(tile);
}

void CMapTileInstance::Add_Tile(_float4 position, _float4 materialType)
{
	auto tileSys = CGameInstance::GetInstance()->Get_TileSystem();
	TILE_INDEX index = tileSys->Get_IndexByPosition(position);
	_uint Flag =tileSys->Get_TileFlagByIndex(index);

	if ((Flag & TileExist) != 0) {
		return;
	}
	else {
		INSTANCE_TILE tile = {};
		tile.vRight = { 1.f, 0.f, 0.f, 0.f };
		tile.vUp = { 0.f, 1.f, 0.f, 0.f };
		tile.vLook = { 0.f, 0.f, 1.f, 0.f };
		tile.vTranslation = position;
		tile.vMaterialType = materialType;
		m_Tiles.push_back(tile);
		tileSys->Add_TileFlagByIndex(index, TileExist);
	}
}

void CMapTileInstance::Load_Tile(INSTANCE_TILE tile)
{
	auto tileSys = CGameInstance::GetInstance()->Get_TileSystem();
	TILE_INDEX index = tileSys->Get_IndexByPosition(tile.vTranslation);
 	m_Tiles.push_back(tile);
	tileSys->Add_TileFlagByIndex(index, TileExist);
}

HRESULT CMapTileInstance::Clear_Tile()
{
	auto tileSys = CGameInstance::GetInstance()->Get_TileSystem();
	auto Context = CEditorSystem::GetInstance()->Get_Context();
	_float3 vMin = Context->ContextTileInfo.vWorldMin;
	_float3 vMax = Context->ContextTileInfo.vWorldMax;

	auto IndexArea = tileSys->Get_IndeciesByArea({ vMin.x,vMin.y,vMin.z,1.f }, { vMax.x,vMax.y,vMax.z,1.f });

	for (auto index : IndexArea) {
		tileSys->Remove_TileFlagByIndex(index, TileExist);
	}
	m_Tiles.clear();
	return S_OK;

}

void CMapTileInstance::Render_GUI()
{
	ImGui::Begin("Tile Texture Array Viewer");
	ImGui::Text(to_string(m_Tiles.size()).c_str());

	if (ImGui::Button("Clear")) {
		Clear_Tile();
	}
	ImGui::End();
}

HRESULT CMapTileInstance::Save_Tiles(ofstream& ofs)
{
	/*ÇöÀç ÀÎµ¦½º*/
	for (auto& tile : m_Tiles) {
		ofs.write(reinterpret_cast<const char*>(&tile), sizeof(INSTANCE_TILE));
	}

	return S_OK;
}

CMapTileInstance* CMapTileInstance::Create()
{
	CMapTileInstance* instance = new CMapTileInstance();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CMapTileInstance");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CMapTileInstance::Clone(INIT_DESC* pArg)
{
	CMapTileInstance* instance = new CMapTileInstance(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CMapTileInstance");
		Safe_Release(instance);
	}

	return instance;
}

void CMapTileInstance::Free()
{
	__super::Free();
}
