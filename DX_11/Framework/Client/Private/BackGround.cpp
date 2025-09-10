#include "Client_Defines.h"
#include "BackGround.h"
#include "GameInstance.h"
#include "IRenderService.h"
#include "IInputService.h"
#include "GameObject.h"
#include "Model.h"
#include "Material.h"

CBackGround::CBackGround()
{
}

CBackGround::CBackGround(const CBackGround& rhs)
	: CUI_Object(rhs)
{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	//여기서 컴포넌트 추가 ...//
	__super::Initialize_Prototype();

	CModel* model = Get_Component<CModel>();
	CMaterial* material = Get_Component<CMaterial>();

	model->Link_Buffer(G_GlobalLevelKey, "Engine_Default_Rect", BUFFER_TYPE::BASIC_RECT);
	model->Link_Element(VTXPOSTEX::Elements, VTXPOSTEX::iElementCount);

	material->Link_Shader(G_GlobalLevelKey, "VTX_TexPos.hlsl");
	material->Link_Texture("Logo_Level", "Default_TexUI", "g_DiffuseTexture");

	return S_OK;
}

HRESULT CBackGround::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}


void CBackGround::Priority_Update(_float dt)
{
	__super::Priority_Update(dt);

	if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_RIGHT)) {
		Rotate_Left(XMConvertToRadians(80) * dt);
	}
}

void CBackGround::Update(_float dt)
{
	__super::Update(dt);

}

void CBackGround::Late_Update(_float dt)
{
	__super::Late_Update(dt);

	
}

CBackGround* CBackGround::Create()
{
	CBackGround* instance = new CBackGround();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CBackGround");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CBackGround::Clone(INIT_DESC* pArg)
{
	CBackGround* instance = new CBackGround(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CBackGround");
		Safe_Release(instance);
	}

	return instance;
}

void CBackGround::Free()
{
	__super::Free();
}
