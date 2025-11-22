#include "Shader.h"
#include "VIBuffer.h"
#include "SpriterEffectRenderer.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "Texture.h"
CSpriterEffectRenderer::CSpriterEffectRenderer()
{
}

CSpriterEffectRenderer::~CSpriterEffectRenderer()
{
}
 

HRESULT CSpriterEffectRenderer::Initialize(ID3D11Device* pDevice)
{
	HRESULT result = {};

	_uint instanceStride = sizeof(SpriteInstanceData);		// 인스턴스 구조체 크기 
		m_InstanceCount = {2048};     // 최대 인스턴스 개수
	_uint ElementCount = {};
	string ElementKey = {  };
	const D3D11_INPUT_ELEMENT_DESC* pElementDesc = { nullptr };

	D3D11_BUFFER_DESC           InstanceDesc{};
	InstanceDesc.ByteWidth = m_InstanceCount * instanceStride;
	InstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	InstanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	InstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	InstanceDesc.MiscFlags = 0;
	InstanceDesc.StructureByteStride = instanceStride;

	if (FAILED(pDevice->CreateBuffer(&InstanceDesc, nullptr, &m_InstanceBuffer)))
		result = E_FAIL;


	m_VBQuad = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_VIBuffer(G_GlobalLevelKey, "Engine_Default_Rect", BUFFER_TYPE::BASIC_RECT);
	Safe_AddRef(m_VBQuad);

	return S_OK;
}

void CSpriterEffectRenderer::Queue(const EffectSpriteDrawDesc& desc)
{
	m_DrawCmds.push_back(desc);
}

void CSpriterEffectRenderer::RenderAll(ID3D11DeviceContext* pContext)
{
	if (m_DrawCmds.empty())
		return;

	unordered_map<string, vector<EffectSpriteDrawDesc>> passGroup;
	for (auto& cmd : m_DrawCmds)
		passGroup[cmd.PassName].push_back(cmd);

	for (auto& [passName, list] : passGroup)
	{
		RenderPass(pContext, passName, list);
	}

	m_DrawCmds.clear();
}

void CSpriterEffectRenderer::RenderPass(ID3D11DeviceContext* pContext,const string& passName,vector<EffectSpriteDrawDesc>& list)
{
	if (list.empty())
		return;

	ID3D11InputLayout* pLayout = nullptr;
	CGameInstance::GetInstance()->Get_RenderSystem()
		->Get_BufferInputLayout(m_VBQuad, m_SpriteEffectShader, passName, &pLayout);

	pContext->IASetInputLayout(pLayout);

	UINT strides[2] = { m_VBQuad->Get_VertexStride(), sizeof(SpriteInstanceData) };
	UINT offsets[2] = { 0, 0 };
	ID3D11Buffer* buffers[2] = { m_VBQuad->Get_VertexBuffer(), m_InstanceBuffer };

	pContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	pContext->IASetIndexBuffer(m_VBQuad->Get_IndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	m_SpriteEffectShader->Apply(passName, pContext);

	size_t index = 0;
	while (index < list.size())
	{
		index = DrawBatch(pContext, list, index);
	}

}

size_t CSpriterEffectRenderer::DrawBatch(ID3D11DeviceContext* pContext, vector<EffectSpriteDrawDesc>& list,size_t startIndex)
{
	const string& texKey = list[startIndex].TextureKey;

	vector<SpriteInstanceData> batch;
	batch.reserve(128);

	size_t i = startIndex;
	while (i < list.size())
	{
		if (list[i].TextureKey != texKey)
			break;

		SpriteInstanceData data{};
		data.WorldMatrix = list[i].WorldMatrix;
		data.vColor = list[i].Color;
		data.TexInfo = {
			(float)list[i].Frame,
			(float)list[i].Cols,
			(float)list[i].Rows,
			0.0f
		};

		batch.push_back(data);
		++i;
	}

	D3D11_MAPPED_SUBRESOURCE sub;
	if (FAILED(pContext->Map(m_InstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub)))
		return;

	memcpy(sub.pData, batch.data(), sizeof(SpriteInstanceData) * batch.size());
	pContext->Unmap(m_InstanceBuffer, 0);

	auto pResMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	if (!pResMgr) return;

	CTexture* pTex = pResMgr->Load_Texture(G_GlobalLevelKey, texKey);
	if (!pTex) return;

	ID3D11ShaderResourceView* srv = pTex->Get_SRV());
	if (!srv) return;


	m_SpriteEffectShader->Bind_Value("SpriteTexture", { srv,"Texture2D",0 });

	pContext->DrawIndexedInstanced(
		m_VBQuad->Get_IndexCount(),       
		(_uint)batch.size(),							// instance 개수
		0, 0, 0);

	return i;  // 다음 배치 시작 인덱스 반환
}



CSpriterEffectRenderer* CSpriterEffectRenderer::Create(ID3D11Device* pDevice)
{
	CSpriterEffectRenderer* instance = new CSpriterEffectRenderer;
	if (FAILED(instance->Initialize(pDevice))) {
		Safe_Release(instance);
	}
	return instance;
}

void CSpriterEffectRenderer::Free()
{
	__super::Free();
	Safe_Release(m_VBQuad);
	Safe_Release(m_InstanceBuffer);
	Safe_Release(m_SpriteEffectShader);
}
