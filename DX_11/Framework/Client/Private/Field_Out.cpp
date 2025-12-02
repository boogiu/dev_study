#include "Client_Defines.h"
#include "Field_Out.h"

#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"

#include "GameInstance.h"
#include "IResourceService.h"
#include "Texture.h"
#include "AudioSource.h"

CField_Out::CField_Out()
{
}

CField_Out::CField_Out(const CField_Out& rhs)
	:CFieldObject(rhs)
{
}


HRESULT CField_Out::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	Add_Component<CAudioSource>();

	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level","Env_SeaWaveSplashWeak00.wav","SplashWeak00",false,SOUND_GROUP::ENV);
	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level","Env_SeaWaveSplashWeak01.wav","SplashWeak01", false,SOUND_GROUP::ENV);
	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level","Env_SeaWaveSplashMid01.wav","SplashMid01", false,SOUND_GROUP::ENV);

	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level","Env_SeaWaterrock03.wav","Rock",true,SOUND_GROUP::ENV);
	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level","Env_SeaWaterWave02.wav","Wave",true,SOUND_GROUP::ENV);
	Get_Component<CAudioSource>()->Add_Slot("GamePlay_Level","Env_SeaBaseRoar00.wav","Base",true,SOUND_GROUP::ENV);

	Get_Component<CAudioSource>()->Set_SlotVolume("Rock",0.1f);
	Get_Component<CAudioSource>()->Set_SlotVolume("Wave",0.1f);
	Get_Component<CAudioSource>()->Set_SlotVolume("Base",0.1f);
	Get_Component<CAudioSource>()->Set_SlotVolume("SplashWeak01",0.1f);
	return S_OK;
}

HRESULT CField_Out::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

    return S_OK;
}

void CField_Out::Priority_Update(_float dt)
{
}

void CField_Out::Update(_float dt)
{
	m_fElpaseTime += dt * .1f;
	m_fWaveTime += dt*.1f;
	
	if (m_fWaveTime > 1.f) {
		Get_Component<CAudioSource>()->Play("SplashWeak01");
		m_fWaveTime = 0.f;
	}
	m_fCircularTime = sinf(m_fElpaseTime*2);

	Get_Component<CAudioSource>()->Play("Rock");
	Get_Component<CAudioSource>()->Play("Wave");
	Get_Component<CAudioSource>()->Play("Base");
}

void CField_Out::Late_Update(_float dt)
{
}

void CField_Out::Render_GUI()
{
	__super::Render_GUI();
}

HRESULT CField_Out::Sync_MapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{
	HRESULT hr = Get_Component<CStaticModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	hr= Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	m_iObjType = objHeader.Object_type;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));
	if (SUCCEEDED(hr)) {
		Override_Pass();
	}
	return S_OK;
}

void CField_Out::Override_Pass()
{
	CMaterial* pMaterial = Get_Component<CMaterial>();
	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
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
	SHADER_PARAM param = { pRcsMgr->Load_Texture("GamePlay_Level","Palette_mWater_Alb.png")->Get_SRV(),"Texture2D",0 };
	SHADER_PARAM Normal = { pRcsMgr->Load_Texture("GamePlay_Level","Waves_mSeaWater_Nrm.png")->Get_SRV(),"Texture2D",0 };
	SHADER_PARAM Sand = { pRcsMgr->Load_Texture("GamePlay_Level","Waves_mSand_Alb.dds")->Get_SRV(),"Texture2D",0 };
	SHADER_PARAM WaveScale = { pRcsMgr->Load_Texture("GamePlay_Level","Waves_mWaveFoam_WavSclXY.png")->Get_SRV(),"Texture2D",0 };
	SHADER_PARAM WaveParam = { &m_fWaveTime,"float",sizeof(_float) };
	SHADER_PARAM TimeParam = { &m_fElpaseTime,"float",sizeof(_float) };
	SHADER_PARAM CircularParam = { &m_fCircularTime,"float",sizeof(_float) };
	SHADER_PARAM fadeParam = { &m_fFade,"float",sizeof(_float) };

	
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mWaveFoam")) {
		instance->Set_Param("DiffuseTexture", param);
		instance->Set_Param("fWaveTime", WaveParam);
		instance->Set_Param("fElapsedTime", TimeParam);
		instance->Set_Param("fCircularTime", CircularParam);
		instance->Set_Param("ScaleXY", WaveScale);
		instance->Override_Pass("Wave");
		instance->Set_Blended(true);
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mSeaWave")) {
		instance->Set_Param("fWaveTime", TimeParam);
		instance->Override_Pass("SeaWave");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mSeabedSand")) {
		instance->Set_Param("fWaveTime", TimeParam);
		instance->Override_Pass("SeaBed");
	}
	if (auto instance = pMaterial->Get_MaterialInstanceByName("mBeach")) {
		instance->Set_Param("DiffuseTexture", Sand);
		instance->Override_Pass("Beach");
	}

	if (auto instance = pMaterial->Get_MaterialInstanceByName("mSand")) {
		instance->Override_Pass("Sand");
	}

	_int index = Get_Component< CStaticModel>()->Get_MeshIndexByName("Sand__mSand-mesh");
	if (index != -1) {
		auto Box = Get_Component<CModel>()->Get_WorldBoundingBox();

		_float4 min = { Box.vMin.x, Box.vMin.y , Box.vMin.z ,1.f};
		_float4 max = { Box.vMax.x, Box.vMax.y , Box.vMax.z ,1.f};

		auto TileSystem = CGameInstance::GetInstance()->Get_TileSystem();
		auto IndexVector = TileSystem->Get_IndeciesByArea(min, max);

		for (auto Index : IndexVector)
			TileSystem->Add_TileFlagByIndex(Index, static_cast<_uint>(TILE_FLAG::FLAG_SAND));
	};
}


CField_Out* CField_Out::Create()
{
	CField_Out* instance = new CField_Out();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CField_Out");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CField_Out::Clone(INIT_DESC* pArg)
{
	CField_Out* instance = new CField_Out(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CField_Out");
		Safe_Release(instance);
	}

	return instance;
}

void CField_Out::Free()
{
	__super::Free();
}
