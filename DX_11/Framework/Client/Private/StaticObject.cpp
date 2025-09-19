#include "StaticObject.h"
#include "Client_Defines.h"
#include "StaticModel.h"
#include "Material.h"
#include "GameInstance.h"
#include "IRenderService.h"
#include "IInputService.h"
#include "IResourceService.h"
#include "AudioSource.h"

CStaticObject::CStaticObject()
{
}

CStaticObject::CStaticObject(const CStaticObject& rhs)
	:CGameObject(rhs)
{
}

CStaticObject::~CStaticObject()
{
}

HRESULT CStaticObject::Initialize_Prototype()
{
	IResourceService* pResourceMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	pResourceMgr->Add_ResourcePath("RacoonModel", "../Bin/Resources/Models/new2/NpcSpRco.model");
	pResourceMgr->Add_ResourcePath("RacoonMaterial", "../Bin/Resources/Models/new2/NpcSpRco.mat");

	__super::Initialize_Prototype();

	CStaticModel* model = Add_Component<CStaticModel>();
	model->Link_Model("Logo_Level", "RacoonModel");

	CMaterial* material = Add_Component<CMaterial>();
	material->Link_Material("Logo_Level", "RacoonMaterial");

	return S_OK;
}

HRESULT CStaticObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CStaticObject::Priority_Update(_float dt)
{

}

void CStaticObject::Update(_float dt)
{
}

void CStaticObject::Late_Update(_float dt)
{
	
}

void CStaticObject::Render_GUI()
{
	__super::Render_GUI();
}

CStaticObject* CStaticObject::Create()
{
	CStaticObject* instance = new CStaticObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CStaticObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CStaticObject::Clone(INIT_DESC* pArg)
{
	CStaticObject* instance = new CStaticObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CStaticObject");
		Safe_Release(instance);
	}

	return instance;
}

void CStaticObject::Free()
{
	__super::Free();
}
