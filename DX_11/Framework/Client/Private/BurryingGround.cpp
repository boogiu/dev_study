#include "Client_Defines.h"
#include "BurryingGround.h"

#include "DumpParts.h"
#include "DumpSmoke.h"
#include "RectModel.h"

#include "GameInstance.h"
#include "IResourceService.h"

#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"

#include "ObjectContainer.h"
#include "Helper_Func.h"
#include "StaticModel.h"
#include "Texture.h"

CBurryingGround::CBurryingGround()
{
}

CBurryingGround::CBurryingGround(const CBurryingGround& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CBurryingGround::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	return S_OK;
}

HRESULT CBurryingGround::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	_float radius = .2f;
	IResourceService* pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pRcsMgr->Add_ResourcePath("Effect_pltcolorgroundlump.png", "../../Resources/Effect/pltcolorgroundlump.png");

	for (size_t i = 0; i < 5; i++)
	{
		_float angle = XM_2PI * (float)i / 5.0f;
		_float x = cosf(angle) * radius;
		_float z = sinf(angle) * radius;

		CGameObject* pObj = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_GroundPart" })
			.Position({ x, -.5f, z }).Build("Effect_GroundPart");
		Get_Component<CObjectContainer>()->Add_Child(pObj, true);
		string name = "groundlump0" + to_string(i);
		pObj->Get_Component<CStaticModel>()->Link_Model("GamePlay_Level", name+".model");
		pObj->Get_Component<CMaterial>()->Link_Material("GamePlay_Level", name + ".mat");

		for (auto& instance : pObj->Get_Component<CMaterial>()->Get_Material_Instance()) {
			instance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
			instance->Set_Param("DiffuseTexture", { pRcsMgr->Load_Texture("GamePlay_Level", "Effect_pltcolorgroundlump.png")->Get_SRV(),"Texture2D", 0 });
			instance->Override_Pass("Rubble");
		}

		m_Parts.push_back(dynamic_cast<CBaseEffect*>(pObj));
	}
	
	Get_Component<CObjectContainer>()->Add_Child(m_pSmoke, true);
	return S_OK;
}

void CBurryingGround::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CBurryingGround::Update(_float dt)
{
	m_fLifeTime += dt;
	m_fTimeCheck += dt;
	
	if (m_fTimeCheck > 0.1f&& m_NowIndex<4) {
		m_Parts[++m_NowIndex]->Set_ReActive({});
		m_fTimeCheck = 0.f;
	}

	if (m_fLifeTime > 1.4f) {
		for (size_t i = 0; i < 5; i++)
		{
			m_Parts[i]->Set_DeActive();
		}
	}
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CBurryingGround::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CBurryingGround::Render_GUI()
{
}

_bool CBurryingGround::isEffectActive()
{
	return m_fLifeTime < 6.5f;
}

void CBurryingGround::Reset()
{
	for (size_t i = 0; i < 5; i++)
	{
		m_Parts[i]->Set_DeActive();
	}
	m_pTransform->Scale({ 1.f,1.f,1.f });
	m_fLifeTime = 0.f;
	m_fTimeCheck = 0.f;
	m_NowIndex = 0;
}

void CBurryingGround::Set_DeActive()
{
	m_isAlive = false;
}

void CBurryingGround::Set_ReActive(const EffectData& data)
{
	Reset();
	m_Parts[m_NowIndex]->Set_ReActive({});
	m_isAlive = true;
}

CBurryingGround* CBurryingGround::Create()
{
	CBurryingGround* instance = new CBurryingGround();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CBurryingGround");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CBurryingGround::Clone(INIT_DESC* pArg)
{
	CBurryingGround* instance = new CBurryingGround(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CBurryingGround");
		Safe_Release(instance);
	}

	return instance;
}

void CBurryingGround::Free()
{
	__super::Free();
}
