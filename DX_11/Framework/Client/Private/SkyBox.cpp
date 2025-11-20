#include "Client_Defines.h"
#include "SkyBox.h"
#include "StaticModel.h"
#include "Material.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "MaterialInstance.h"
#include "Texture.h"

CSkyBox::CSkyBox()
{
}

CSkyBox::CSkyBox(const CSkyBox& rhs)
	:CGameObject(rhs)
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>();
	return S_OK;
}

HRESULT CSkyBox::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CStaticModel>()->Link_Model("GamePlay_Level","cloud_vr.model");
	Get_Component<CStaticModel>()->Set_RenderType(RENDER_PASS_TYPE::PRIORITY);
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "cloud_vr.mat");

	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcsMgr->Add_ResourcePath("Cloud_m_cloud_indirect.png","../../Resources/Models/Env/Cloud/m_cloud_indirect.png");
	pRcsMgr->Add_ResourcePath("Cloud_VRCloudCumulonimbus.png","../../Resources/Models/Env/Cloud/VRCloudCumulonimbus.png");
	pRcsMgr->Add_ResourcePath("Cloud_VRCloudCommon.png","../../Resources/Models/Env/Cloud/VRCloudCommon.png");
	pRcsMgr->Add_ResourcePath("Cloud_VRCloudCumulonimbus.png","../../Resources/Models/Env/Cloud/VRCloudCumulonimbus.png");
	pRcsMgr->Add_ResourcePath("Cloud_VRCloudCirrostratus.dds","../../Resources/Models/Env/Cloud/VRCloudCirrostratus.dds");
	//pRcsMgr->Add_ResourcePath("Cloud_003.dds","../../Resources/Models/Env/Cloud/003.dds");

	auto vector = Get_Component<CMaterial>()->Get_Material_Instance();
	for (auto& instance : vector)
	{
		instance->Set_Param("fWindTime", { &m_ElapsedTime, "float", sizeof(_float)});
		instance->Set_Param("PatternTexture", { pRcsMgr->Load_Texture("GamePlay_Level","Cloud_VRCloudCirrostratus.dds")->Get_SRV(), "Texture2D", 0 });
		instance->Set_Param("IndirectTexture", { pRcsMgr->Load_Texture("GamePlay_Level","Cloud_m_cloud_indirect.png")->Get_SRV(), "Texture2D", 0 });
		instance->Set_Param("CloudTexture", { pRcsMgr->Load_Texture("GamePlay_Level","Cloud_VRCloudCumulonimbus.png")->Get_SRV(), "Texture2D", 0 });
		instance->Set_Param("CommonTexture", { pRcsMgr->Load_Texture("GamePlay_Level","Cloud_VRCloudCommon.png")->Get_SRV(), "Texture2D", 0 });
	}
	return S_OK;
}

void CSkyBox::Priority_Update(_float dt)
{
}

void CSkyBox::Update(_float dt)
{
	m_ElapsedTime += dt;
}

void CSkyBox::Late_Update(_float dt)
{
}

void CSkyBox::Render_GUI()
{
}

 
CSkyBox* CSkyBox::Create()
{
	CSkyBox* instance = new CSkyBox();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CSkyBox");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CSkyBox::Clone(INIT_DESC* pArg)
{
	CSkyBox* instance = new CSkyBox(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CSkyBox");
		Safe_Release(instance);
	}

	return instance;
}
void CSkyBox::Free()
{
	__super::Free();
}
