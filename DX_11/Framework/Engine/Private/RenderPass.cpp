#include "RenderPass.h"
#include "Model.h"
#include "Material.h"
#include "Transform.h"
#include "RenderSystem.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "Shader.h"
#include "Animator3D.h"

RenderPass::RenderPass(CRenderSystem* pRenderSystem)
	:m_pRenderSystem(pRenderSystem)
{
}

void RenderPass::BindConstant(ID3D11DeviceContext* pContext, CModel* pModel, CMaterial* pMaterial, _uint DrawIndex, _uint MaterialIndex)
{
	CPipeLine* pPipeLine = m_pRenderSystem->Get_Pipeline();
	pCurShader = pMaterial->Get_Shader(MaterialIndex);
	pCurShader->SetConstantBuffer("FrameBuffer", pPipeLine->Get_FrameBuffer());
	pCurShader->SetConstantBuffer("LightBuffer", pPipeLine->Get_LightBuffer());
	ID3D11InputLayout* pLayout;
	m_pRenderSystem->Get_InputLayout(pModel, pCurShader, DrawIndex, pMaterial->GetPassConstant(MaterialIndex), &pLayout);
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
		[](const OPAQUE_PACKET& a, const OPAQUE_PACKET& b) {
			return a.GetKey() < b.GetKey();
		});

	if (m_Packets.empty())
		return;

	/*패킷이 비어 있으면 리턴*/
	for (auto& packet : m_Packets)
	{
		if (packet.pMaterial->Get_Shader(packet.DrawIndex) != pCurShader) {
			BindConstant(pContext, packet.pModel, packet.pMaterial, packet.DrawIndex, packet.MaterialIndex);
		}

		pPipeLine->Update_ObjectBuffer(pContext, packet.pWorldMatrix);
		pCurShader->SetConstantBuffer("ObjectBuffer", pPipeLine->Get_ObjectBuffer());

		if (packet.bSkinning) {
			pPipeLine->Update_SkinningBuffer(pContext, packet.pAnimator);
			pCurShader->SetConstantBuffer("SkinningBuffer", pPipeLine->Get_SkinningBuffer());
		}

		packet.pMaterial->Apply_Material(pContext, packet.MaterialIndex);
		packet.pModel->Render_Model(pContext, packet.DrawIndex);
	}

	m_Packets.clear();
}

void OpaquePass::Submit(OPAQUE_PACKET packet)
{
	if (packet.pModel == nullptr || packet.pMaterial == nullptr) return;
	m_Packets.push_back(packet);
}

#pragma endregion

#pragma region UI_PASS
void UIPass::Execute(ID3D11DeviceContext* pContext)
{
	CPipeLine* pPipeLine = m_pRenderSystem->Get_Pipeline();
	pCurShader = { nullptr };
	
	if (!m_Packets.empty()) {
		UI_PACKET packet = m_Packets.front();
		BindConstant(pContext, packet.pModel, packet.pMaterial, packet.DrawIndex, packet.MaterialIndex);
	}
	
	for (auto& packet : m_Packets)
	{
		if (packet.pMaterial->Get_Shader(packet.DrawIndex) != pCurShader) {
			BindConstant(pContext, packet.pModel, packet.pMaterial, packet.DrawIndex, packet.MaterialIndex);
		}
	
		pPipeLine->Update_ObjectBuffer(pContext, packet.pWorldMatrix);
		pCurShader->SetConstantBuffer("ObjectBuffer", pPipeLine->Get_ObjectBuffer());
	
		packet.pMaterial->Apply_Material(pContext, packet.MaterialIndex);
		packet.pModel->Render_Model(pContext, packet.DrawIndex);
	}
	
	m_Packets.clear();
}

void UIPass::Submit(UI_PACKET packet)
{
	if (packet.pModel == nullptr || packet.pMaterial == nullptr) return;
	m_Packets.push_back(packet);
}
#pragma endregion
