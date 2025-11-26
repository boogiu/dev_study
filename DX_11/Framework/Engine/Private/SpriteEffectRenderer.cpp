#include "Shader.h"
#include "VIBuffer.h"
#include "SpriteEffectRenderer.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "Texture.h"

CSpriteEffectRenderer::CSpriteEffectRenderer()
{
}

CSpriteEffectRenderer::~CSpriteEffectRenderer()
{
}
 

HRESULT CSpriteEffectRenderer::Initialize(ID3D11Device* pDevice)
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

	auto pRcsMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	m_VBQuad = pRcsMgr->Load_VIBuffer(G_GlobalLevelKey, "Engine_Default_Rect", BUFFER_TYPE::BASIC_RECT);
	Safe_AddRef(m_VBQuad);

	m_SpriteEffectShader=	pRcsMgr->Load_Shader(G_GlobalLevelKey,"SpriteEffectShader.hlsl");
	Safe_AddRef(m_SpriteEffectShader);

	return S_OK;
}

void CSpriteEffectRenderer::Queue(const EffectSpriteDrawDesc& desc)
{
	m_DrawCmds.push_back(desc);
}

void CSpriteEffectRenderer::RenderAll(ID3D11DeviceContext* pContext)
{
	if (m_DrawCmds.empty())
		return;

	unordered_map<string, vector<EffectSpriteDrawDesc>> passGroup;

	for (auto& cmd : m_DrawCmds)
		passGroup[cmd.PassName].push_back(cmd);

	for (auto& [passName, list] : passGroup)
	{
		sort(list.begin(), list.end(),
			[](auto& a, auto& b) { return a.TextureKey < b.TextureKey; });

		RenderPass(pContext, passName, list);
	}

	m_DrawCmds.clear();
}

void CSpriteEffectRenderer::RenderPass(ID3D11DeviceContext* pContext,const string& passName,vector<EffectSpriteDrawDesc>& list)
{
	/*패스별로 진행.*/
	if (list.empty())
		return;

	ID3D11InputLayout* pLayout = Get_InputLayout(passName);
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

size_t CSpriteEffectRenderer::DrawBatch(ID3D11DeviceContext* pContext, vector<EffectSpriteDrawDesc>& list,size_t startIndex)
{
	const string& texKey = list[startIndex].TextureKey;

	auto pResMgr = CGameInstance::GetInstance()->Get_ResourceMgr();
	if (!pResMgr) return 0 ;

	CTexture* pTex = pResMgr->Load_Texture(G_GlobalLevelKey, texKey);
	if (!pTex) return 0;

	ID3D11ShaderResourceView* srv = pTex->Get_SRV();
	if (!srv)return 0;

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
		return 0;

	memcpy(sub.pData, batch.data(), sizeof(SpriteInstanceData) * batch.size());
	pContext->Unmap(m_InstanceBuffer, 0);

	m_SpriteEffectShader->Bind_Value("SpriteTexture", { srv,"Texture2D",0 });
	pContext->DrawIndexedInstanced(m_VBQuad->Get_IndexCount(),       (_uint)batch.size(),	0, 0, 0);
	
	return i;  // 다음 배치 시작 인덱스 반환
}

ID3D11InputLayout* CSpriteEffectRenderer::Get_InputLayout(string passName)
{
	if (m_InputLayoutCache.count(passName))
		return m_InputLayoutCache[passName];

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// Quad Vertex
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		// Instance Data (World Matrix 4x4)
		{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

		// Instance Color
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

		// Instance TexInfo
		{ "TEXINFO", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	D3DX11_PASS_DESC passDesc = {};
	m_SpriteEffectShader->GetPassSignature(passName, &passDesc);

	ID3D11InputLayout* layoutOut = nullptr;
	CGameInstance::GetInstance()->Get_Device()->CreateInputLayout(
		layout, ARRAYSIZE(layout),
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
		&layoutOut);

	m_InputLayoutCache[passName] = layoutOut;
	return layoutOut;
}

CSpriteEffectRenderer* CSpriteEffectRenderer::Create(ID3D11Device* pDevice)
{
	CSpriteEffectRenderer* instance = new CSpriteEffectRenderer;
	if (FAILED(instance->Initialize(pDevice))) {
		Safe_Release(instance);
	}
	return instance;
}

void CSpriteEffectRenderer::Free()
{
	__super::Free();
	Safe_Release(m_VBQuad);
	Safe_Release(m_InstanceBuffer);
	Safe_Release(m_SpriteEffectShader);
}
