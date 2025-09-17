#include "RenderPass.h"
#include "Model.h"
#include "Material.h"
#include "Transform.h"
#include "RenderSystem.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "Shader.h"

RenderPass::RenderPass(CRenderSystem* pRenderSystem)
	:m_pRenderSystem(pRenderSystem)
{
}

void RenderPass::BindConstant(ID3D11DeviceContext* pContext, CModel* pModel, CMaterial* pMaterial, _uint DrawIndex)
{
	CPipeLine* pPipeLine = m_pRenderSystem->Get_Pipeline();
	pCurShader = pMaterial->Get_Shader(pModel->Get_MaterialIndex(DrawIndex));
	pCurShader->SetConstantBuffer("FrameBuffer", pPipeLine->Get_FrameBuffer());
	pCurShader->SetConstantBuffer("LightBuffer", pPipeLine->Get_LightBuffer());
	ID3D11InputLayout* pLayout;
	m_pRenderSystem->Get_InputLayout(pModel, pMaterial, DrawIndex, &pLayout);
	pContext->IASetInputLayout(pLayout);
}


void RenderPass::Free()
{
}

#pragma region OPAQUE_PASS
void OpaquePass::Execute(ID3D11DeviceContext* pContext)
{
	CPipeLine* pPipeLine = m_pRenderSystem->Get_Pipeline();
	pCurShader = { nullptr };

	sort(m_Packets.begin(), m_Packets.end(),
		[](const OPAQUE_PACKET& a, const OPAQUE_PACKET& b) -> bool
		{
			if (a.pMaterial->Get_ShaderID(a.DrawIndex) != b.pMaterial->Get_ShaderID(b.DrawIndex))
				return a.pMaterial->Get_ShaderID(a.DrawIndex) < b.pMaterial->Get_ShaderID(b.DrawIndex);
			if (a.pMaterial->Get_MaterialDataID(a.DrawIndex) != b.pMaterial->Get_MaterialDataID(b.DrawIndex))
				return a.pMaterial->Get_MaterialDataID(a.DrawIndex) < b.pMaterial->Get_MaterialDataID(b.DrawIndex);
			/*여기에 모델별 정렬*/
			return false;
		});

	if (!m_Packets.empty()) {
		OPAQUE_PACKET pack = m_Packets.front();
		BindConstant(pContext, pack.pModel, pack.pMaterial, pack.DrawIndex);
	}

	for (auto& packet : m_Packets)
	{
		if (packet.pMaterial->Get_Shader(packet.DrawIndex) != pCurShader) {
			BindConstant(pContext, packet.pModel, packet.pMaterial, packet.DrawIndex);
		}

		pPipeLine->Update_ObjectBuffer(pContext, packet.pWorldMatrix);
		pCurShader->SetConstantBuffer("ObjectBuffer", pPipeLine->Get_ObjectBuffer());

		if (packet.bSkinning) {
			pPipeLine->Update_SkinningBuffer(pContext, packet.pModel, packet.DrawIndex);
			pCurShader->SetConstantBuffer("SkinningBuffer", pPipeLine->Get_SkinningBuffer());
		}

		packet.pMaterial->Apply_Material(pContext, packet.pModel->Get_MaterialIndex(packet.DrawIndex));
		packet.pModel->Render_Mesh(pContext, packet.DrawIndex);
	}

	m_Packets.clear();
}

void OpaquePass::Submit(OPAQUE_PACKET packet)
{
	if (packet.pModel == nullptr || packet.pMaterial == nullptr || !packet.pModel->hasBuffer()) return;
	m_Packets.push_back(packet);
}

#pragma endregion

#pragma region UI_PASS
void UIPass::Execute(ID3D11DeviceContext* pContext)
{
	CPipeLine* pPipeLine = m_pRenderSystem->Get_Pipeline();
	pCurShader = { nullptr };

	if (!m_Packets.empty()) {
		UI_PACKET pack = m_Packets.front();
		BindConstant(pContext, pack.pModel, pack.pMaterial, pack.DrawIndex);
	}

	for (auto& packet : m_Packets)
	{
		if (packet.pMaterial->Get_Shader(packet.DrawIndex) != pCurShader) {
			BindConstant(pContext, packet.pModel, packet.pMaterial, packet.DrawIndex);
		}

		pPipeLine->Update_ObjectBuffer(pContext, packet.pWorldMatrix);
		pCurShader->SetConstantBuffer("ObjectBuffer", pPipeLine->Get_ObjectBuffer());

		packet.pMaterial->Apply_Material(pContext, packet.pModel->Get_MaterialIndex(packet.DrawIndex));
		packet.pModel->Render_Mesh(pContext, packet.DrawIndex);
	}

	m_Packets.clear();
}

void UIPass::Submit(UI_PACKET packet)
{
	if (packet.pModel == nullptr || packet.pMaterial == nullptr || !packet.pModel->hasBuffer()) return;
	m_Packets.push_back(packet);
}
#pragma endregion
