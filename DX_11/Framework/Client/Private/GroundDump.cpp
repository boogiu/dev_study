#include "Client_Defines.h"
#include "GRoundDump.h"

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

CGroundDump::CGroundDump()
{
}

CGroundDump::CGroundDump(const CGroundDump& rhs)
	:CBaseEffect(rhs)
{
}

HRESULT CGroundDump::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	Add_Component<CObjectContainer>();
	Add_Component<CStaticModel>();
	Add_Component<CMaterial>(); 
	return S_OK;
}

HRESULT CGroundDump::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	Get_Component<CStaticModel>()->Link_Model("GamePlay_Level","gmakeshavestone.model");
	Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "gmakeshavestone.mat");
	
	for (auto& instance : Get_Component<CMaterial>()->Get_Material_Instance()) {
		instance->Set_Param("fElapsedTime", { &m_fLifeTime,"float",sizeof(float) });
		instance->Set_Param("vGradation", { &m_PaletteIndex,"float2", sizeof(_float2)});
		instance->Override_Pass("Palette");
	}

	m_PaletteIndex = { 0.3,0.4 };

	for (size_t i = 0; i < 15; i++)
	{
		CGameObject* pObj = Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_DumpParts" }).Build("Effect_RagDolStone");
		Get_Component<CObjectContainer>()->Add_Child(pObj, true);
		m_Parts.push_back(dynamic_cast<CDumpParts*>(pObj));
	}
	
	m_pSmoke = dynamic_cast<CBaseEffect*>(Builder::Create_Object({ "GamePlay_Level","GamePlay_Effect_DumpSmoke" })
		.Build("Effect_DumpSmoke"));
	
	Get_Component<CObjectContainer>()->Add_Child(m_pSmoke, true);
	return S_OK;
}

void CGroundDump::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CGroundDump::Update(_float dt)
{
	m_fLifeTime += dt*3;

	m_pTransform->AddScale({ 0,dt,0 });
	
	if (m_fLifeTime > 1.f&& !m_bPartsSplashed) {
		m_bPartsSplashed = true;
		Get_Component<CStaticModel>()->Set_CompActive(false);
		_int count = Helper::Get_Random_Int(8, 15);
		for (size_t i = 0; i < count; i++)
		{
			m_Parts[i]->Set_ReActive({});
		}
		m_pSmoke->Set_ReActive({});
	}
	Get_Component<CObjectContainer>()->UpdateChild(dt);
}

void CGroundDump::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
}

void CGroundDump::Render_GUI()
{
	ImGui::DragFloat2("Index", &m_PaletteIndex.x, 0.1f);
}

_bool CGroundDump::isEffectActive()
{
	return m_fLifeTime < 6.f;
}

void CGroundDump::Reset()
{
	for (size_t i = 0; i < 15; i++)
	{
		m_Parts[i]->Set_DeActive();
	}
	m_pSmoke->Set_DeActive();
	Get_Component<CStaticModel>()->Set_CompActive(true);
	m_pTransform->Scale({ 1.f,0.8f,1.f });
	m_fLifeTime = 0.f;
	m_bPartsSplashed = false;
}

void CGroundDump::Set_DeActive()
{
	m_isAlive = false;
}

void CGroundDump::Set_ReActive(const EffectData& data)
{
	Reset();
	m_isAlive = true;
}

CGroundDump* CGroundDump::Create()
{
	CGroundDump* instance = new CGroundDump();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CGroundDump");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CGroundDump::Clone(INIT_DESC* pArg)
{
	CGroundDump* instance = new CGroundDump(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CGroundDump");
		Safe_Release(instance);
	}

	return instance;
}

void CGroundDump::Free()
{
	__super::Free();
}
