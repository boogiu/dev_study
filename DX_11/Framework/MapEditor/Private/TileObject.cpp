#include "Editor_Defines.h"
#include "TileObject.h"

#include "SkeletalModel.h"
#include "StaticModel.h"

#include "Material.h"
#include "MaterialData.h"
#include "MaterialInstance.h"

#include "Texture.h"

#include "DebugRender.h"
#include "TileBlock.h"

#include "RayReceiver.h"

#include "GameInstance.h"
#include "IInputService.h"
#include "IResourceService.h"

CTileObject::CTileObject()
{
}

CTileObject::CTileObject(const CTileObject& rhs)
	:CGameObject(rhs)
{
}

HRESULT CTileObject::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CMaterial>();
	Add_Component<CStaticModel>();
	Add_Component<CRayReceiver>();
	Add_Component<CTileBlock>();

	return S_OK;
}

HRESULT CTileObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	TILE_TYPE_DESC* tileDesc = static_cast<TILE_TYPE_DESC*>(pArg);
	m_BaseTypeName = tileDesc->TypeName;
	TILE_INDEX index;

	if (m_BaseTypeName.find("Base")!=string::npos) {
		Is_Base = true;
		Link_Data(m_BaseTypeName);
		index = Get_Component<CTileBlock>()->On_Grid(tileDesc->index, m_BaseTypeName, true);
	}
	else {
		Link_Data(m_BaseTypeName + "0A_0");
		index = Get_Component<CTileBlock>()->On_Grid(tileDesc->index, m_BaseTypeName, true);
		if (index.IndexX < 0 || index.IndexY < 0 || index.IndexZ < 0) {
			return E_FAIL;
		}
		_uint N_State = Get_Component<CTileBlock>()->Get_NeigborState();
		Update_State(N_State);
	}

	if (index.IndexX < 0 || index.IndexY < 0 || index.IndexZ < 0) {
		return E_FAIL;
	}

	return S_OK;
}


vector<CTileObject::AutoTileEditor> g_TileDB = {
	{"1A", NEIGHBOR_INDEX::UP,
	NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::RIGHT ,true
	},

	{"2A", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::DOWN,
	NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::RIGHT,true},

	{"2B", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::LEFT,
	NEIGHBOR_INDEX::UPLEFT  | NEIGHBOR_INDEX::RIGHT | NEIGHBOR_INDEX::DOWN,true},

	{"3A", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::DOWN,
	NEIGHBOR_INDEX::RIGHT | NEIGHBOR_INDEX::DOWNLEFT,true},

	{"3B", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::UPLEFT,
	NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::RIGHT,true},

	{"4A", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::UPLEFT,
	NEIGHBOR_INDEX::RIGHT | NEIGHBOR_INDEX::DOWNLEFT },

	{"4B", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::DOWNLEFT,
	NEIGHBOR_INDEX::RIGHT | NEIGHBOR_INDEX::UPLEFT},

	{"4C", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::RIGHT,
	NEIGHBOR_INDEX::DOWNLEFT | NEIGHBOR_INDEX::DOWNRIGHT | NEIGHBOR_INDEX::UPLEFT | NEIGHBOR_INDEX::UPRIGHT},

	{"5A", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::RIGHT | NEIGHBOR_INDEX::UPRIGHT,
	NEIGHBOR_INDEX::UPLEFT | NEIGHBOR_INDEX::DOWNLEFT | NEIGHBOR_INDEX::DOWNRIGHT},

	{"5B", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::UPLEFT | NEIGHBOR_INDEX::DOWNLEFT,
	 NEIGHBOR_INDEX::RIGHT,true },

	{"6A", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::RIGHT | NEIGHBOR_INDEX::UPRIGHT | NEIGHBOR_INDEX::DOWNLEFT ,
	NEIGHBOR_INDEX::UPLEFT | NEIGHBOR_INDEX::DOWNRIGHT},
	{"6B", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::RIGHT | NEIGHBOR_INDEX::DOWNRIGHT | NEIGHBOR_INDEX::DOWNLEFT,
	NEIGHBOR_INDEX::UPLEFT | NEIGHBOR_INDEX::UPRIGHT },

	{"7A", NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::RIGHT | NEIGHBOR_INDEX::DOWNRIGHT | NEIGHBOR_INDEX::DOWNLEFT | NEIGHBOR_INDEX::UPLEFT ,
	NEIGHBOR_INDEX::UPRIGHT },

	{"8A", 	NEIGHBOR_INDEX::UP | NEIGHBOR_INDEX::DOWN | NEIGHBOR_INDEX::LEFT | NEIGHBOR_INDEX::RIGHT | NEIGHBOR_INDEX::DOWNRIGHT | NEIGHBOR_INDEX::DOWNLEFT | NEIGHBOR_INDEX::UPLEFT | NEIGHBOR_INDEX::UPRIGHT,
	static_cast<NEIGHBOR_INDEX>(0)},
};


void CTileObject::Priority_Update(_float dt)
{
	if (Is_Base) return;
	_uint N_State = Get_Component<CTileBlock>()->Get_NeigborState();

	if(m_CurState != N_State)
		Update_State(N_State);

}

void CTileObject::Update(_float dt)
{

}

void CTileObject::Late_Update(_float dt)
{
}

HRESULT CTileObject::Link_Data(const string& folderName, _bool Base)
{
	HRESULT hr = Get_Component<CModel>()->Link_Model(G_GlobalLevelKey, folderName + ".model");
	hr = Get_Component<CMaterial>()->Link_Material(G_GlobalLevelKey, folderName + ".mat");
	CMaterial* pMaterial = Get_Component<CMaterial>();

	if (Base) {
		for (auto& instance : pMaterial->Get_Material_Instance()) {
			instance->Override_Pass("Base");
		}
	}

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

HRESULT CTileObject::Save_MapData(ofstream& ofs, _bool Base)
{
	/*현재 인덱스*/
	MAP_TILE_HEADER mapTile = {};
	strcpy_s(mapTile.BaseTypeName, sizeof(mapTile.BaseTypeName), m_BaseTypeName.c_str());
	mapTile.CurState = m_CurState;
	mapTile.fRotation = m_fRotation;
	mapTile.Index = Get_Component<CTileBlock>()->Get_Index();
	mapTile.Is_Base = Is_Base;

	ofs.write(reinterpret_cast<const char*>(&mapTile), sizeof(MAP_TILE_HEADER));
	return S_OK;
}

void CTileObject::Render_GUI()
{
	__super::Render_GUI();

	_uint state = Get_Component<CTileBlock>()->Get_NeigborState();
	ImGui::SeparatorText("Neighbor State");

	const char* dirLabels[9] = {
		"LT", "T", "RT",
		"L",  "C", "R",
		"LB", "B", "RB"
	};

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			int idx = row * 3 + col;
			if (idx == 4) { // 가운데 자기 자신
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), "C");
			}
			else
			{
				bool active = (state & (1 << idx));
				ImVec4 color = active ? ImVec4(0.2f, 0.8f, 0.3f, 1.f)
					: ImVec4(0.3f, 0.3f, 0.3f, 1.f);

				ImGui::PushStyleColor(ImGuiCol_Button, color);
				ImGui::Button(dirLabels[idx], ImVec2(22, 22));
				ImGui::PopStyleColor();
			}

			if (col < 2) ImGui::SameLine();
		}
	}
	ImGui::PopStyleVar();
	ImGui::Text("Neighbor Int : %u", state);
}

void CTileObject::Update_State(_uint N_State)
{
	string selectedName;
	_float rotation = 0.f;
	string yIndex = "_" + to_string(Get_Component<CTileBlock>()->Get_Index().IndexY);

	for (auto& tile : g_TileDB)
	{
		_float rot = tile.Check_Connect(static_cast<NEIGHBOR_INDEX>(N_State));//내 주변 이웃들이 몇명인지 ->회전 각도

		if (rot < 360.f) { // 회전 가능한 경우 찾음
			selectedName = tile.name;
			rotation = rot;
			m_fRotation = rot;
			break;
		}
	}

	if (!selectedName.empty())
	{
		m_pTransform->Override_Rotation({ 0,1,0,0 }, XMConvertToRadians(rotation));
		Link_Data(m_BaseTypeName + selectedName+ yIndex);
		return;
	}

	else {
		Link_Data(m_BaseTypeName + "0A"+ yIndex);
	}

	m_CurState = N_State;
}

HRESULT CTileObject::Save_RuleFile()
{
	string path = "../../Resources/Data/RuleTile.dat";
	filesystem::path directory(path);

	ofstream ofs(path.c_str(), ios::binary);
	if (!ofs.is_open())
		return E_FAIL;

	AUTO_TILE_HEADER autoHeader = {};
	autoHeader.RuleCount = g_TileDB.size();
	ofs.write(reinterpret_cast<const char*>(&autoHeader), sizeof(AUTO_TILE_HEADER));

	for (size_t i = 0; i < g_TileDB.size(); i++)
	{
		char Name[MAX_PATH];
		_bool Partitial = g_TileDB[i].Partitial;
		strcpy_s(Name, sizeof(Name), g_TileDB[i].name.c_str());

		AUTO_TILE_DESC Zero_Rotate = {};
		Zero_Rotate.Connectable = g_TileDB[i].Connectable;
		Zero_Rotate.NeverConnectable = g_TileDB[i].NeverConnectable;
		Zero_Rotate.rotation = 0;

		AUTO_TILE_DESC First_Rotate = {};
		First_Rotate.Connectable = AutoTileEditor::Rotate90(g_TileDB[i].Connectable,1);
		First_Rotate.NeverConnectable = AutoTileEditor::Rotate90(g_TileDB[i].NeverConnectable, 1); 
		First_Rotate.rotation = 90;

		AUTO_TILE_DESC Second_Rotate = {};
		Second_Rotate.Connectable = AutoTileEditor::Rotate90(g_TileDB[i].Connectable, 2);
		Second_Rotate.NeverConnectable = AutoTileEditor::Rotate90(g_TileDB[i].NeverConnectable, 2);
		Second_Rotate.rotation = 180;

		AUTO_TILE_DESC Third_Rotate = {};
		Third_Rotate.Connectable = AutoTileEditor::Rotate90(g_TileDB[i].Connectable, 3);
		Third_Rotate.NeverConnectable = AutoTileEditor::Rotate90(g_TileDB[i].NeverConnectable, 3);
		Third_Rotate.rotation = 270;


		ofs.write(reinterpret_cast<const char*>(&Name),sizeof(Name));
		ofs.write(reinterpret_cast<const char*>(&Zero_Rotate),sizeof(AUTO_TILE_DESC));
		ofs.write(reinterpret_cast<const char*>(&First_Rotate),sizeof(AUTO_TILE_DESC));
		ofs.write(reinterpret_cast<const char*>(&Second_Rotate),sizeof(AUTO_TILE_DESC));
		ofs.write(reinterpret_cast<const char*>(&Third_Rotate),sizeof(AUTO_TILE_DESC));
		ofs.write(reinterpret_cast<const char*>(&Partitial),sizeof(_bool));
	}

	ofs.close();
	return S_OK;
}

HRESULT CTileObject::PrepareForTile(const string& baseFolderPath)
{
	for (const auto& entry : filesystem::recursive_directory_iterator(filesystem::path(baseFolderPath))) {
		if (!entry.is_regular_file())
			continue;

		string ext = entry.path().extension().string();
		string fileName = entry.path().filename().string();

		if (ext == ".model" || ext == ".mat") {
			CGameInstance::GetInstance()->Get_ResourceMgr()->Add_ResourcePath(fileName, entry.path().string());
		}
	}

	return S_OK;
}


CTileObject* CTileObject::Create()
{
	CTileObject* instance = new CTileObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CTileObject::Clone(INIT_DESC* pArg)
{
	CTileObject* instance = new CTileObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		Safe_Release(instance);
	}

	return instance;
}

void CTileObject::Free()
{
	__super::Free();
}
