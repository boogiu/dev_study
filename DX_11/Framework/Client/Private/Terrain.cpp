#include "Terrain.h"
#include "Client_Defines.h"
#include "Model.h"
#include "Material.h"
#include "GameInstance.h"
#include "IRenderService.h"
#include "IInputService.h"
#include "AudioSource.h"

CTerrain::CTerrain()
{
}

CTerrain::CTerrain(const CTerrain& rhs)
	:CGameObject(rhs)
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	////여기서 컴포넌트 추가 ...//
	//__super::Initialize_Prototype();
	//
	//CModel* model = Add_Component<CModel>();
	//model->Link_Buffer("Logo_Level", "Default_Terrain", BUFFER_TYPE::TERRAIN);
	//model->Link_Element(VTXNORMTEX::Elements, VTXNORMTEX::iElementCount);
 	//
	//CMaterial* material = Add_Component<CMaterial>();
	//material->Link_Shader(G_GlobalLevelKey, "VTX_NorTex.hlsl");
	//material->Link_Texture("Logo_Level", "Default_Tex", "g_DiffuseTexture");
	//
	//CAudioSource* audio = Add_Component<CAudioSource>();
	//audio->Add_Slot("Logo_Level", "Test_Sound", "Bump");
	return S_OK;
}

HRESULT CTerrain::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CTerrain::Priority_Update(_float dt)
{

}

void CTerrain::Update(_float dt)
{
}

void CTerrain::Late_Update(_float dt)
{
	
}

void CTerrain::Render_GUI()
{
	__super::Render_GUI();
}

CTerrain* CTerrain::Create()
{
	CTerrain* instance = new CTerrain();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CTerrain");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CTerrain::Clone(INIT_DESC* pArg)
{
	CTerrain* instance = new CTerrain(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CTerrain");
		Safe_Release(instance);
	}

	return instance;
}

void CTerrain::Free()
{
	__super::Free();
}
