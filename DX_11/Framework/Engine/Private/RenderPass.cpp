#include "RenderPass.h"
#include "RenderSystem.h"
#include "PipeLine.h"
#include "Model.h"
#include "Material.h"
#include "Transform.h"
#include "Shader.h"
#include "Animator3D.h"
#include "SkeletonFollower.h"
#include "GameInstance.h"
#include "IResourceService.h"
#include "DebugRender.h"
#include "SkeletalModel.h"

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
	/*이건 전역적으로 셰이더에 값 넣어주는 역할*/
	CPipeLine* pPipeLine = m_pRenderSystem->Get_Pipeline();
	pCurShader = { nullptr };

	/*같은 셰이더, 같은 머티리얼, 같은 모델끼리 정렬 */
	sort(m_Packets.begin(), m_Packets.end(),
		[](const OPAQUE_PACKET& a, const OPAQUE_PACKET& b) {
			return a.GetKey() < b.GetKey();
		});

	/*패킷이 비어 있으면 리턴*/
	if (m_Packets.empty())
		return;

	for (auto& packet : m_Packets)
	{
		/*셰이더 바뀌면 업데이트 한번*/
		if (packet.pMaterial->Get_Shader(packet.MaterialIndex) != pCurShader) {
			BindConstant(pContext, packet.pModel, packet.pMaterial, packet.DrawIndex, packet.MaterialIndex);
		}

		pPipeLine->Update_ObjectBuffer(pContext, packet.pWorldMatrix);
		pCurShader->SetConstantBuffer("ObjectBuffer", pPipeLine->Get_ObjectBuffer());

		/*가지고 있는 거 있으면 꺼내봐*/
		if (packet.bSkinning) {
			if (holds_alternative<CAnimator3D*>(packet.pPayLoad)) {
				auto animator = get<CAnimator3D*>(packet.pPayLoad);
				pPipeLine->Update_SkinningBuffer(pContext, animator->Get_BoneMatrices());
			}
			else if (holds_alternative<CSkeletonFollower*>(packet.pPayLoad)) {
				auto follower = get<CSkeletonFollower*>(packet.pPayLoad);
				pPipeLine->Update_SkinningBuffer(pContext, follower->Get_BoneMatrices());
			}
			else {
				pPipeLine->Update_SkinningBuffer(pContext, dynamic_cast<CSkeletalModel*>(packet.pModel)->Get_BoneMatrices());
			}
			pCurShader->SetConstantBuffer("SkinningBuffer", pPipeLine->Get_SkinningBuffer());
		}

		/*이제 그려*/
		packet.pMaterial->Apply_Material(pContext, packet.MaterialIndex);
		packet.pModel->Draw(pContext, packet.DrawIndex);
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
		if (packet.pMaterial->Get_Shader(packet.MaterialIndex) != pCurShader) {
			BindConstant(pContext, packet.pModel, packet.pMaterial, packet.DrawIndex, packet.MaterialIndex);
		}

		pPipeLine->Update_ObjectBuffer(pContext, packet.pWorldMatrix);
		pCurShader->SetConstantBuffer("ObjectBuffer", pPipeLine->Get_ObjectBuffer());

		packet.pMaterial->Apply_Material(pContext, packet.MaterialIndex);
		packet.pModel->Draw(pContext, packet.DrawIndex);
	}

	m_Packets.clear();
}

void UIPass::Submit(UI_PACKET packet)
{
	if (packet.pModel == nullptr || packet.pMaterial == nullptr) return;
	m_Packets.push_back(packet);
}
#pragma endregion

void DebugPass::Execute(ID3D11DeviceContext* pContext)
{
	CPipeLine* pPipeLine = m_pRenderSystem->Get_Pipeline();
	if (pCurShader == nullptr) {
		pCurShader = CGameInstance::GetInstance()->Get_ResourceMgr()->Load_Shader(G_GlobalLevelKey, "VTX_Debug.hlsl");
	}

	if (m_Packets.empty())
		return;

	pCurShader->SetConstantBuffer("FrameBuffer", pPipeLine->Get_FrameBuffer());
	pCurShader->SetConstantBuffer("LightBuffer", pPipeLine->Get_LightBuffer());

	CModel* pCurModel = { nullptr };

	for (auto& packet : m_Packets)
	{
		if (packet.pModel != pCurModel) {
			pPipeLine->Update_ObjectBuffer(pContext, packet.pWorldMatrix);
			ID3D11InputLayout* pLayout;
			m_pRenderSystem->Get_InputLayout(packet.pModel, pCurShader,
				0, "Debug", &pLayout);
			pContext->IASetInputLayout(pLayout);
			pCurModel = packet.pModel;
		}
	
		packet.pDebug->Render_DebugBox(pContext, packet.pModel);
	}
	m_Packets.clear();
}

void DebugPass::Submit(DEBUG_PACKET packet)
{
	if (packet.pModel == nullptr || packet.pDebug == nullptr) return;
	m_Packets.push_back(packet);
}
