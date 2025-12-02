#include "Client_Defines.h"
#include "ScreenFX.h"
#include "Sprite2D.h"

CScreenFX::CScreenFX()
{
}

CScreenFX::CScreenFX(const CScreenFX& rhs)
	:CUI_Object(rhs)
{
}

HRESULT CScreenFX::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CScreenFX::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	
	Get_Component<CSprite2D>()->Link_Shader(G_GlobalLevelKey, "VTX_UI.hlsl");
	Get_Component<CSprite2D>()->Add_Texture("GamePlay_Level", "Effect_glow00.png");
	Get_Component<CSprite2D>()->ChangePass("Screen_FX");
	Get_Component<CSprite2D>()->Set_Param("fElapsedTime",{&m_fLifeTime, "float", sizeof(float)});

	return S_OK;
}

void CScreenFX::Priority_Update(_float dt)
{
}

void CScreenFX::Update(_float dt)
{
	if (IsActive) 
		m_fLifeTime += dt*1.5;
	else
		m_fLifeTime -= dt * 1.5;

}

void CScreenFX::Late_Update(_float dt)
{
}

void CScreenFX::UI_Active(void* pArg)
{
	IsActive = !IsActive;
	m_fLifeTime = m_fLifeTime / fabs(m_fLifeTime);
}

void CScreenFX::UI_DeActive(void* pArg)
{
}

void CScreenFX::Render_GUI()
{
}

CScreenFX* CScreenFX::Create()
{
	CScreenFX* instance = new CScreenFX();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CScreenFX");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CScreenFX::Clone(INIT_DESC* pArg)
{
	CScreenFX* instance = new CScreenFX(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CScreenFX");
		Safe_Release(instance);
	}

	return instance;
}

void CScreenFX::Free()
{
	__super::Free();
}
