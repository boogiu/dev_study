#include "Client_Defines.h"
#include "AutoTile.h"
#include "InstanceModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "GameInstance.h"
#include "StaticModel.h"
#include "TileBlock.h"
#include "ITileService.h"
#include "IResourceService.h"
#include "Transform.h"

static vector<AUTO_TILE> TileRuleDB;

CAutoTile::CAutoTile()
{
}

CAutoTile::CAutoTile(const CAutoTile& rhs)
	:CGameObject(rhs)
{
}

HRESULT CAutoTile::Initialize_Prototype()
{
	__super::Initialize();
	Add_Component<CMaterial>();
	Add_Component<CStaticModel>();
	Add_Component<CTileBlock>();
	return S_OK;
}

HRESULT CAutoTile::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	TILE_TYPE_DESC* tileDesc = static_cast<TILE_TYPE_DESC*>(pArg);
	m_BaseTypeName = tileDesc->TypeName;

	Link_Data(m_BaseTypeName + "0A_0");
	m_Index = Get_Component<CTileBlock>()->On_Grid(tileDesc->index, m_BaseTypeName, true);
	if (m_Index.IndexX < 0 || m_Index.IndexZ < 0) {
		return E_FAIL;
	}

	_uint N_State = Get_Component<CTileBlock>()->Get_NeigborState();
	Update_State(N_State);
	if (m_Index.IndexX < 0 || m_Index.IndexZ < 0) {
		return E_FAIL;
	}

	if (TileRuleDB.empty()) {
		Read_TileRule();
	}
	auto tileSys = CGameInstance::GetInstance()->Get_TileSystem();

	tileSys->Add_TileFlagByIndex(m_Index, static_cast<_uint>(TILE_FLAG::FLAG_TILE | TILE_FLAG::FLAG_WALKABLE));
	tileSys->Remove_TileFlagByIndex(m_Index, static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED));
	if (m_BaseTypeName.find("River") != string::npos) {
		tileSys->Set_Material_ID(m_Index, { 0,0,0,0 });
		tileSys->Add_TileFlagByIndex(m_Index, static_cast<_uint>(TILE_FLAG::FLAG_BLOCKED));
	}

	return S_OK;
}

void CAutoTile::Priority_Update(_float dt)
{
	_uint N_State = Get_Component<CTileBlock>()->Get_NeigborState();

	if (m_CurState != N_State)
		Update_State(N_State);
}

void CAutoTile::Update(_float dt)
{

}

void CAutoTile::Late_Update(_float dt)
{
}

void CAutoTile::Render_GUI()
{
}

HRESULT CAutoTile::Link_Data(const string& folderName)
{
	HRESULT hr = Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, folderName + ".model");
	hr = Get_Component<CMaterial>()->Link_Material(G_GlobalLevelKey, folderName + ".mat");
	CMaterial* pMaterial = Get_Component<CMaterial>();

	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassCliffXlu")) {
		instance->Override_Pass("Edge");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassRiverXlu")) {
		instance->Override_Pass("Edge");
	}
	if (SUCCEEDED(hr)) {
		ModelName = folderName + ".model";
		MaterialName = folderName + ".mat";
	}

	return hr;
}

HRESULT CAutoTile::Read_TileRule()
{
	string path = "../../Resources/Data/RuleTile.dat";
	filesystem::path directory(path);

	ifstream ifs(path.c_str(), ios::binary);
	if (!ifs.is_open())
		return E_FAIL;

	AUTO_TILE_HEADER autoHeader = {};
	ifs.read(reinterpret_cast<char*>(&autoHeader), sizeof(AUTO_TILE_HEADER));

	for (size_t i = 0; i < autoHeader.RuleCount; i++)
	{
		AUTO_TILE tile = {};

		char Name[MAX_PATH];
		ifs.read(reinterpret_cast<char*>(&Name), sizeof(Name));
		tile.TypeName = Name;

		ifs.read(reinterpret_cast<char*>(&tile.rotateType[0]), sizeof(AUTO_TILE_DESC));
		ifs.read(reinterpret_cast<char*>(&tile.rotateType[1]), sizeof(AUTO_TILE_DESC));
		ifs.read(reinterpret_cast<char*>(&tile.rotateType[2]), sizeof(AUTO_TILE_DESC));
		ifs.read(reinterpret_cast<char*>(&tile.rotateType[3]), sizeof(AUTO_TILE_DESC));
		ifs.read(reinterpret_cast<char*>(&tile.Patial), sizeof(_bool));
		TileRuleDB.push_back(tile);
	}

	ifs.close();
	return S_OK;
}

void CAutoTile::Update_State(_uint N_State)
{
	string selectedName;
	float rotation = 0.f;

	// CAutoTile의 룰 DB 참조
	auto& RuleDB = TileRuleDB;

	for (const AUTO_TILE& tile : RuleDB)
	{
		for (int i = 0; i < 4; ++i)
		{
			const AUTO_TILE_DESC& desc = tile.rotateType[i];

			const _uint s = static_cast<_uint>(N_State);
			const _uint c = static_cast<_uint>(desc.Connectable);
			const _uint n = static_cast<_uint>(desc.NeverConnectable);

			// "이웃 상태가 금지 방향을 포함하지 않아야 한다"
			if ((s & n) != 0)
				continue;

			// 부분 매칭 / 완전 매칭 구분
			bool match = false;
			if (tile.Patial)
				match = ((s & c) == c);      // 부분 일치: 필요한 방향만 있으면 OK
			else
				match = (s == c);            // 완전 일치

			if (match)
			{
				selectedName = tile.TypeName;
				rotation = static_cast<float>(desc.rotation);
				m_fRotation = rotation;
				break;  // 이 tile에서 일치하는 회전 발견
			}
		}

		if (!selectedName.empty())
			break;      // 더 이상 검사할 필요 없음
	}

	// 선택된 룰이 있으면 회전 + 데이터 연결
	if (!selectedName.empty())
	{
		m_pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(rotation));
		string yIndex = "_0";
		Link_Data(m_BaseTypeName + selectedName + yIndex);
	}
	else
	{
		string yIndex = "_0";
		Link_Data(m_BaseTypeName + "0A" + yIndex);
	}

	m_CurState = N_State;
}

CAutoTile* CAutoTile::Create()
{
	CAutoTile* instance = new CAutoTile();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CAutoTile");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CAutoTile::Clone(INIT_DESC* pArg)
{
	CAutoTile* instance = new CAutoTile(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CBaseField");
		Safe_Release(instance);
	}

	return instance;
}
void CAutoTile::Free()
{
	__super::Free();
}
