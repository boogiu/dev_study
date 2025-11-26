#include "Editor_Defines.h"
#include "MapObject.h"

#include "SkeletalModel.h"
#include "StaticModel.h"
#include "ModelData.h"

#include "Material.h"
#include "MaterialInstance.h"

#include "TileBlock.h"

#include "GameInstance.h"
#include "RayReceiver.h"
#include "EditorSystem.h"
#include "ITileService.h"
#include "IResourceService.h"
#include "Layer.h"
#include "DebugRender.h"
CMapObject::CMapObject()
{
}

CMapObject::CMapObject(const CMapObject& rhs)
	:CGameObject(rhs)
{
}

HRESULT CMapObject::Initialize_Prototype()
{
	Load_ModelMap();
	__super::Initialize_Prototype();
	Add_Component<CMaterial>();
	Add_Component<CTileBlock>();
	Add_Component<CDebugRender>();
	return S_OK;
}

HRESULT CMapObject::Initialize(INIT_DESC* pArg)
{

	__super::Initialize(pArg);
	return S_OK;
}

void CMapObject::Priority_Update(_float dt)
{
}

void CMapObject::Update(_float dt)
{
}

void CMapObject::Late_Update(_float dt)
{
}

void CMapObject::Object_OnGrid(TILE_INDEX index)
{
	CEditorSystem::Editor_Context* pContext = CEditorSystem::GetInstance()->Get_Context();
	_float4 Anchor = CGameInstance::GetInstance()->Get_TileSystem()->Get_PositionByIndex(index, static_cast<ANCHOR>(pContext->eAnchor));
	m_pTransform->Set_Pos({ Anchor.x,Anchor.y,Anchor.z });
	m_SyncedIndex = index;
}

HRESULT CMapObject::Link_Data(const string& folderName)
{
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	CModelData* pData = pRcsMgr->Load_ModelData(G_GlobalLevelKey, folderName + ".model");
	if (!pData)
		return E_FAIL;

	_bool SkinnedMesh = pData->isSkinned();
	CModel* pMyModel = { nullptr };
	if (SkinnedMesh) {
		pMyModel= Add_Component<CSkeletalModel>();
	}
	else {
		pMyModel= Add_Component<CStaticModel>();
	}

	HRESULT hr = pMyModel->Link_Model(G_GlobalLevelKey, folderName + ".model");
	CMaterial* pMaterial = Get_Component<CMaterial>();

	hr = pMaterial->Link_Material(G_GlobalLevelKey, folderName + ".mat");

	if (FAILED(hr))
		return E_FAIL;

	if (SkinnedMesh) {
		for (auto& instance : pMaterial->Get_Material_Instance()) {
			instance->Override_Pass("ForceSee");
		};
	}
	else {
		for (auto& instance : pMaterial->Get_Material_Instance()) {
			instance->Override_Pass("Force_See");
		};
		if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassXlu")) {
			instance->Override_Pass("Edge");
		}
		if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrassRiverXlu")) {
			instance->Override_Pass("Edge");
		}
		if (auto instance = pMaterial->Get_MaterialInstanceByName("mWaterfall")) {
			instance->Override_Pass("Water");
		}
		if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
			instance->Override_Pass("Base");
		}

		if (auto instance = pMaterial->Get_MaterialInstanceByName("mWaveFoam")) {
			instance->Override_Pass("Wave");
			instance->Set_Blended(true);
		}
		if (auto instance = pMaterial->Get_MaterialInstanceByName("mSeaWave")) {
			instance->Override_Pass("SeaWave");
		}

		if (auto instance = pMaterial->Get_MaterialInstanceByName("mBeach")) {
			instance->Override_Pass("Beach");
		}

		if (auto instance = pMaterial->Get_MaterialInstanceByName("mSand")) {
			instance->Override_Pass("Sand");
		}
	}

	if (auto instance = pMaterial->Get_MaterialInstanceByName("mGrass")) {
		instance->Override_Pass("Base");
	}

	vector<string> datas = {
			to_string(ModelMapTable.size()),
			folderName + ".model",
			folderName + ".mat",
			pRcsMgr->Get_ResourcePath(folderName + ".model"),
			pRcsMgr->Get_ResourcePath(folderName + ".mat"),
			""
	};

	ModelMapTable.emplace(
		folderName,
		datas
	);
	m_ObjName = folderName;

	auto iter=  ModelMapTable.find(folderName);
	if (iter != ModelMapTable.end()) {
		m_ObjeType = stoi(iter->second[0]);
	}

	Get_Component<CDebugRender>()->Add_DebugBounding(Get_Component<CModel>()->Get_LocalBoundingBox());

	return hr;
}

HRESULT CMapObject::Load_Object(NEW_MAP_OBJECT_HEADER ObjHeader)
{
	m_SyncedIndex = ObjHeader.Index;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&ObjHeader.vWorldMatrix));
	m_ObjeType = ObjHeader.Object_type;
	m_AdditionalData = ObjHeader.AdditionalData;
	auto iter = ModelMapTable.find(ObjHeader.ObjectName);
	if (iter != ModelMapTable.end()) {
		m_ObjeType = stoi(iter->second[0]);
		Link_Data(ObjHeader.ObjectName);
	}

	m_InstanceName = ObjHeader.ObjectName;
	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	tileSystem->Set_Material_ID(m_SyncedIndex, {0,0,0,0 });

	return S_OK;
}

HRESULT CMapObject::Save_MapData(ofstream& ofs)
{
	/*현재 인덱스*/
	//		MAP_OBJECT_HEADER ObjHeader = {};
	//		
	//		ObjHeader.Index = m_SyncedIndex;
	//		ObjHeader.Object_type = m_ObjeType;
	//		ObjHeader.vWorldMatrix = m_pTransform->Get_WorldMatrix();
	//		strcpy_s(ObjHeader.ObjectName, m_ObjName.c_str());
	//		ofs.write(reinterpret_cast<const char*>(&ObjHeader), sizeof(MAP_OBJECT_HEADER));

	NEW_MAP_OBJECT_HEADER ObjHeader = {};
	ObjHeader.Index = m_SyncedIndex;
	ObjHeader.Object_type = m_ObjeType;
	ObjHeader.vWorldMatrix = m_pTransform->Get_WorldMatrix();
	strcpy_s(ObjHeader.ObjectName, m_ObjName.c_str());
	strcpy_s(ObjHeader.AdditionalData, m_AdditionalData.c_str());
	ofs.write(reinterpret_cast<const char*>(&ObjHeader), sizeof(NEW_MAP_OBJECT_HEADER));

	if (Get_Position().x == 475.f
		&& Get_Position().z == 605.f)
		int i = 0;
	return S_OK;
}

void CMapObject::Render_GUI()
{
	__super::Render_GUI();

	static char nameBuf[64] = "";

	// 현재 ObjectType을 버퍼에 반영 (초기 한 번만)
	if (strlen(nameBuf) == 0 && !m_AdditionalData.empty())
		strcpy_s(nameBuf, IM_ARRAYSIZE(nameBuf), m_AdditionalData.c_str());

	// 입력창
	ImGui::Text("AdditionalData");
	if (ImGui::InputText("##AdditionalData", nameBuf, IM_ARRAYSIZE(nameBuf)))
	{
		// 입력 내용이 바뀔 때만 적용
		m_AdditionalData = nameBuf;
	}

	if (ImGui::Button("Rotate 90"))
		m_pTransform->Rotation({ 0, XMConvertToRadians(90), 0 });

	if (ImGui::ArrowButton("Up15", ImGuiDir_Up))
		m_pTransform->Translate({ 0, 15, 0, 0 });

	if (ImGui::ArrowButton("Down15", ImGuiDir_Down))
		m_pTransform->Translate({ 0, -15, 0, 0 });
}


HRESULT CMapObject::Save_ModelMap()
{
	wstring path = L"../../Resources/Data/ModelMap.json";

	// 배열 형태로 초기화
	json jScene = json::array();

	HANDLE hFile = ::CreateFileW(
		path.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	for (auto& pair : ModelMapTable)
	{
		const string& key = pair.first;
		const vector<string>& values = pair.second;

		if (values.size() >= 5)
		{
			json entry = {
				{"Key",       key},
				{"ID",        stoi(values[0])},
				{"Model",     values[1]},
				{"Mat",       values[2]},
				{"ModelPath", values[3]},
				{"MatPath",   values[4]},
				{"AdditionalData",   values[5]},
			};
			jScene.push_back(entry);
		}
	}

	string jsonText = jScene.dump(4); // UTF-8 문자열

	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD written = 0;
		::WriteFile(hFile, jsonText.data(), (DWORD)jsonText.size(), &written, NULL);
		::CloseHandle(hFile);
	}
	else
	{
		MessageBoxW(nullptr, L"파일 저장 실패", L"Error", MB_OK);
	}

	return S_OK;
}

HRESULT CMapObject::Load_ModelMap()
{
	wstring path = L"../../Resources/Data/ModelMap.json";
	ifstream ifs(path);
	if (!ifs.is_open()) {
		MessageBoxW(nullptr, L"ModelMap.json 파일을 찾을 수 없습니다.", L"Error", MB_OK);
		return E_FAIL;
	}

	json jScene;
	try {
		ifs >> jScene;
	}
	catch (const json::parse_error& e) {
		MessageBoxA(nullptr, e.what(), "JSON Parse Error", MB_OK);
		return E_FAIL;
	}
	ifs.close();

	ModelMapTable.clear();

	// JSON 배열 순회
	for (auto& item : jScene)
	{
		try
		{
			string key = item.value("Key", "");
			_uint id = 0;
			if (item.contains("ID"))
			{
				if (item["ID"].is_number_integer())
					id = item["ID"].get<_uint>();
				else if (item["ID"].is_string())
					id = std::stoul(item["ID"].get<string>());
			}

			string model = item.value("Model", "");
			string mat = item.value("Mat", "");
			string modelPath = item.value("ModelPath", "");
			string matPath = item.value("MatPath", "");
			string AdditionalData = item.value("AdditionalData", "");
		
			if (key.empty())
				continue;

			ModelMapTable[key] = { to_string(id), model, mat, modelPath, matPath,AdditionalData };
		}
		catch (...)
		{
			continue;
		}
	}

	// 리소스 매니저에 등록
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	for (auto& pair : ModelMapTable)
	{
		const vector<string>& values = pair.second;
		if (values.size() < 5)
			continue;

		pRcsMgr->Add_ResourcePath(values[1], values[3]); // model
		pRcsMgr->Add_ResourcePath(values[2], values[4]); // mat
	}

	return S_OK;
}

CMapObject* CMapObject::Create()
{
	CMapObject* instance = new CMapObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CMapObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CMapObject::Clone(INIT_DESC* pArg)
{
	CMapObject* instance = new CMapObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CMapObject");
		Safe_Release(instance);
	}

	return instance;
}

void CMapObject::Free()
{
	__super::Free();
}