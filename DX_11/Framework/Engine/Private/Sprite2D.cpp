#include "GameInstance.h"
#include "Sprite2D.h"
#include "Shader.h"
#include "Texture.h"
#include "IResourceService.h"
#include "VI_Point.h"

CSprite2D::CSprite2D()
{
}

CSprite2D::CSprite2D(const CSprite2D& rhs)
	:CComponent(rhs)
{
}

CSprite2D::~CSprite2D()
{
}

HRESULT CSprite2D::Initialize_Prototype()
{
	
	return S_OK;
}


HRESULT CSprite2D::Initialize(COMPONENT_DESC* pArg)
{
	m_pPoint = CVI_Point::Create(CGameInstance::GetInstance()->Get_Device(), "Sprite2D");
	if (m_pPoint == nullptr)
		return E_FAIL;

	return S_OK;
}

void CSprite2D::Apply_Shader(ID3D11DeviceContext* pContext)
{
	if (m_pShader == nullptr) return;
	if (m_pTextures.empty()) return;
	if (m_pTextures[m_iDrawIndex] == nullptr) return;

	SHADER_PARAM param = {};
	param.typeName = "Texture2D";
	param.iSize = 0;
	param.pData = m_pTextures[m_iDrawIndex]->Get_SRV();

	m_pShader->Bind_Value("SpriteTexture", param);
	m_pShader->Apply(m_PassConstant, pContext);
}

void CSprite2D::Draw_Sprite(ID3D11DeviceContext* pContext)
{
	if (m_pPoint)
	{
		m_pPoint->Bind_Buffer(pContext);
		m_pPoint->Render(pContext);
	}
}

CVIBuffer* CSprite2D::Get_Buffer()
{
	return m_pPoint;
}

HRESULT CSprite2D::Add_Texture(const string& levelKey, const string& TextureKey)
{
	CTexture* pTexture = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Texture(levelKey, TextureKey);

	if (!pTexture)
		return E_FAIL;
	m_pTextures.push_back(pTexture);

	Safe_AddRef(pTexture);
	return S_OK;
}

HRESULT CSprite2D::Link_Shader(const string& levelKey, const string& shaderKey)
{
	Safe_Release(m_pShader);
	m_pShader = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Shader(levelKey, shaderKey);

	if (!m_pShader)
		return E_FAIL;

	Safe_AddRef(m_pShader);
	return S_OK;
}


_bool CSprite2D::IsValid()
{
	return (m_pPoint&&m_pShader&&!m_pTextures.empty());
}

void CSprite2D::Render_GUI()
{
}

CSprite2D* CSprite2D::Create()
{
	CSprite2D* instance = new CSprite2D();
	if (FAILED(instance->Initialize_Prototype()))
	{
		Safe_Release(instance);
		MSG_BOX("Sprite2D Comp Failed To Create : CSprite2D");
	}
	return instance;
}

CComponent* CSprite2D::Clone()
{
	return new CSprite2D(*this);
}

void CSprite2D::Free()
{
	__super::Free();
	Safe_Release(m_pPoint);
	Safe_Release(m_pShader);

	for (auto& texture : m_pTextures)
	{
		Safe_Release(texture);
	}
}
