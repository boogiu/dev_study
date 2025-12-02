#include "Client_Defines.h"
#include "Plant_Grass.h"
#include "SkeletalModel.h"
#include "StaticModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "Texture.h"

CPlant_Grass::CPlant_Grass()
{
}

CPlant_Grass::CPlant_Grass(const CPlant_Grass& rhs)
	:CFieldObject(rhs)
{
}

HRESULT CPlant_Grass::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CSkeletalModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CPlant_Grass::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CPlant_Grass::Priority_Update(_float dt)
{
}

void CPlant_Grass::Update(_float dt)
{
	_float bounceTime = 0.3f;
	if (m_bPlayerOn)
	{
		m_fScaleTime += dt;

		_float t = m_fScaleTime / bounceTime;
		t = min(t, 1.f);

		_float bounce = 1.0f + sinf(t * XM_PI) * 0.1f;

		m_pTransform->Scale({ bounce, 1, bounce });

		if (m_fScaleTime > bounceTime)
		{
			m_bPlayerOn = false;
			m_fScaleTime = 0.f;
		}
	}

}

void CPlant_Grass::Late_Update(_float dt)
{
	_uint Flag = CGameInstance::GetInstance()->Get_TileSystem()->Get_TileFlagByIndex(m_Index);
	if ((Flag & TILE_FLAG::ONPLAYER) != 0) {
		m_bPlayerOn = true;
	}
}

void CPlant_Grass::Render_GUI()
{
	__super::Render_GUI();
	ImGui::Text("Type : %d", m_iObjType);
}



HRESULT CPlant_Grass::Sync_MapData(NEW_MAP_OBJECT_HEADER objHeader, vector<string> modelMapTable)
{
	auto isWeed = [](_uint type)->_bool { return 200 <= type && type < 300; };
	auto isFlower = [](_uint type) ->_bool { return 300 <= type && type < 400; };

	HRESULT hr = Get_Component<CModel>()->Link_Model("GamePlay_Level", modelMapTable[1]);
	Get_Component<CModel>()->ShadowCast(true);
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", modelMapTable[2]);
	m_iObjType = objHeader.Object_type;
	m_pTransform->TranslateMatrix(XMLoadFloat4x4(&objHeader.vWorldMatrix));

	auto tileSystem = CGameInstance::GetInstance()->Get_TileSystem();
	m_Index = tileSystem->Get_IndexByPosition(Get_Position());

	
	auto instance = Get_Component<CMaterial>()->Get_Material_Instance();

	for (auto& inst : instance) {
		if (isWeed(objHeader.Object_type)) {
			inst->Override_Pass("Grass");
			m_InstanceTag = "Grass";
			tileSystem->Add_TileFlagByIndex(objHeader.Index, static_cast<_uint>(TILE_FLAG::FLAG_GRASS));
			tileSystem->Set_Material_ID(objHeader.Index, { 1,1,0,0 });
		}
		else {
			m_InstanceTag = "Flower";
			tileSystem->Add_TileFlagByIndex(objHeader.Index, static_cast<_uint>(TILE_FLAG::FLAG_FLOWER));
			tileSystem->Set_Material_ID(objHeader.Index, { 1,1,0,0 });
			string mtl = inst->Get_MaterialName();
			if (mtl == "mLilyFlower")
				continue;

			CTexture* grd =CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture("GamePlay_Level", "Palette_" + mtl + "Grd_Grd.png");
			if(grd)
 				inst->Set_Param("GradationTexture", { grd->Get_SRV(), "Texture2D", 0 });
			else {
				int i =0;
			}
			inst->Override_Pass("Flower");
		}
	}
	return hr;
}

CPlant_Grass* CPlant_Grass::Create()
{
	CPlant_Grass* instance = new CPlant_Grass();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CPlant_Grass");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CPlant_Grass::Clone(INIT_DESC* pArg)
{
	CPlant_Grass* instance = new CPlant_Grass(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CPlant_Grass");
		Safe_Release(instance);
	}

	return instance;
}

void CPlant_Grass::Free()
{
	__super::Free();
}