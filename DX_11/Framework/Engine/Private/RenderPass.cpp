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
{}
void RenderPass::Free()
{}

#pragma region OPAQUE_PASS
void OpaquePass::Execute(ID3D11DeviceContext* pContext)
{
	CPipeLine* pPipeLine = m_pRenderSystem->Get_Pipeline();
	pCurShader = {nullptr};

	sort(m_Packets.begin(), m_Packets.end(),
		[](const OPAQUE_PACKET& a, const OPAQUE_PACKET& b) -> bool
		{
			if (a.pMaterial->Get_Shader() != b.pMaterial->Get_Shader())
				return a.pMaterial->Get_Shader() < b.pMaterial->Get_Shader();
			if (a.pMaterial != b.pMaterial)
				return a.pMaterial < b.pMaterial;
			return false;
		});

	if (!m_Packets.empty()) {
		pCurShader = m_Packets.front().pMaterial->Get_Shader();
		pCurShader->SetConstantBuffer("FrameBuffer", pPipeLine->Get_FrameBuffer());
		pCurShader->SetConstantBuffer("LightBuffer", pPipeLine->Get_LightBuffer());
	}

	for (auto& packet : m_Packets)
	{ 
		if (packet.pMaterial->Get_Shader() != pCurShader) {
			pCurShader = packet.pMaterial->Get_Shader();
			pCurShader->SetConstantBuffer("FrameBuffer", pPipeLine->Get_FrameBuffer());
		}

		pPipeLine->Update_ObjectBuffer(pContext,packet.pWorldMatrix);
		pCurShader->SetConstantBuffer("ObjectBuffer", pPipeLine->Get_ObjectBuffer());

		packet.pMaterial->Apply_Material("Opaque", pContext);
		packet.pModel->Bind_Model(pContext);


		/*레이아웃 생성*/
		ID3D11InputLayout* pLayout;
		m_pRenderSystem->Get_InputLayout(packet.pModel, (packet.pMaterial)->Get_Shader(), "Opaque", &pLayout);
		pContext->IASetInputLayout(pLayout);

		/*그리기*/
		packet.pModel->Render_Model(pContext);
	}

	m_Packets.clear();
}

void OpaquePass::Submit(OPAQUE_PACKET packet)
{
	if (packet.pModel == nullptr || packet.pMaterial == nullptr) return;
	m_Packets.push_back(packet);
	/*패킷 정렬 필요함*/
}

#pragma endregion

#pragma region UI_PASS
void UIPass::Execute(ID3D11DeviceContext* pContext)
{
	CPipeLine* pPipeLine = m_pRenderSystem->Get_Pipeline();
	pCurShader = { nullptr };

	if (!m_Packets.empty()) {
		pCurShader = m_Packets.front().pMaterial->Get_Shader();
		pCurShader->SetConstantBuffer("FrameBuffer", pPipeLine->Get_FrameBuffer());
	}
	
	for (auto& packet : m_Packets)
	{
		if (packet.pMaterial->Get_Shader() != pCurShader) {
			pCurShader = packet.pMaterial->Get_Shader();
			pCurShader->SetConstantBuffer("FrameBuffer", pPipeLine->Get_FrameBuffer());
		}

		pPipeLine->Update_ObjectBuffer(pContext, packet.pWorldMatrix);
		pCurShader->SetConstantBuffer("ObjectBuffer", pPipeLine->Get_ObjectBuffer());

		packet.pMaterial->Apply_Material("UI", pContext);
		packet.pModel->Bind_Model(pContext);

		/*레이아웃 생성*/
		ID3D11InputLayout* pLayout;
		m_pRenderSystem->Get_InputLayout(packet.pModel, (packet.pMaterial)->Get_Shader(), "UI", &pLayout);
		pContext->IASetInputLayout(pLayout);

		/*그리기*/
		packet.pModel->Render_Model(pContext);
	}

	m_Packets.clear();
}
#pragma endregion
