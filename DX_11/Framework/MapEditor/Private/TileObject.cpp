#include "Editor_Defines.h"
#include "TileObject.h"
#include "TileBlock.h"
#include "Model.h"
#include "SkeletalModel.h"
#include "StaticModel.h"
#include "Material.h"
#include "MaterialData.h"
#include "DebugRender.h"
#include "MaterialInstance.h"
#include "RayReceiver.h"
#include "Animator3D.h"
#include "GameInstance.h"
#include "IInputService.h"

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
	//Add_Component<CStaticModel>();
	Add_Component<CSkeletalModel>();
	//Add_Component<CAnimator3D>();
	Add_Component<CDebugRender>();
	Add_Component<CTileBlock>();
	Add_Component<CRayReceiver>();

	return S_OK;
}

HRESULT CTileObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CTileObject::Priority_Update(_float dt)
{

}

void CTileObject::Update(_float dt)
{
	if (CAnimator3D* pAnimator = Get_Component<CAnimator3D>()) 
		pAnimator->Update_Animation(dt);
}

void CTileObject::Late_Update(_float dt)
{

}

void CTileObject::Object_OnGrid(_uint x, _uint y, _uint z)
{
	Get_Component<CDebugRender>()->Set_DebugBounding(Get_Component<CModel>()->Get_LocalBoundingBox());
	Get_Component<CTileBlock>()->Set_TilePostion(x, y, z);

}

void CTileObject::Set_Selected(_bool selected)
{
	if (selected) {
		CMaterial* pMaterial = Get_Component<CMaterial>();
		for (auto& instance : pMaterial->Get_Material_Instance()) {
			instance->Override_Pass("ForceBlend");
		}
	}
	else {
		CMaterial* pMaterial = Get_Component<CMaterial>();
		for (auto& instance : pMaterial->Get_Material_Instance()) {
			instance->Reset_Pass();
		}
	}
}


void CTileObject::Render_GUI()
{
	__super::Render_GUI();
	_uint x, y, z = {};
	Get_Component<CTileBlock>()->Get_TilePostion(&x,&y,&z);

	if (ImGui::ArrowButton("##up", ImGuiDir::ImGuiDir_Up))
	{
		Get_Component<CTileBlock>()->Set_TilePostion(x,y+1,z);
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("##down", ImGuiDir::ImGuiDir_Down))
	{
		Get_Component<CTileBlock>()->Set_TilePostion(x, y, z+1);
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("##right", ImGuiDir::ImGuiDir_Right))
	{
		Get_Component<CTileBlock>()->Set_TilePostion(x+1, y, z);
	}
}

CTileObject* CTileObject::Create()
{
	CTileObject* instance = new CTileObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CTileObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CTileObject::Clone(INIT_DESC * pArg)
{
	CTileObject* instance = new CTileObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CTileObject");
		Safe_Release(instance);
	}

	return instance;
}

void CTileObject::Free()
{
	__super::Free();
}
