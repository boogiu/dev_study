#include "Client_Defines.h"
#include "NpcRco.h"

#include "SkeletalModel.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialData.h"
#include "MaterialAnimator.h"
#include "Texture.h"
#include "Animator3D.h"
#include "ObjectContainer.h"

#include "NpcState_Machine.h"

CNpcRco::CNpcRco()
{
}

CNpcRco::CNpcRco(const CNpcRco& rhs)
	:CNonPlayer(rhs)
{
}

HRESULT CNpcRco::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNpcRco::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	m_InstanceName = "Racoon";

	HRESULT hr = Get_Component<CSkeletalModel>()->Link_Model("GamePlay_Level", "NpcSpRco.model");
	hr = Get_Component<CMaterial>()->Link_Material("GamePlay_Level", "NpcSpRco.mat");
	Get_Component<CAnimator3D>()->LinkAnimate_Model("GamePlay_Level", "NpcSpRco.model");

	
	return S_OK;
}

void CNpcRco::Priority_Update(_float dt)
{
}

void CNpcRco::Update(_float dt)
{
	Update_Movement(dt);
	Update_TileInfo(dt);

	m_pMachine->Update(dt);
}

void CNpcRco::Late_Update(_float dt)
{
}

void CNpcRco::Render_GUI()
{
	//if(ImGui::Button("Move"))
		//m_pMachine->Request_ChangeState()
	__super::Render_GUI();
}

void CNpcRco::Find_Path()
{


}

CNpcRco* CNpcRco::Create()
{
	CNpcRco* instance = new CNpcRco();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CNpcRco");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CNpcRco::Clone(INIT_DESC* pArg)
{
	CNpcRco* instance = new CNpcRco(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CNpcRco");
		Safe_Release(instance);
	}

	return instance;
}

void CNpcRco::Free()
{
	__super::Free();
}
