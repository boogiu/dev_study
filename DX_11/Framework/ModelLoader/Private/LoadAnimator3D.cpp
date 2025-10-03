#include "Loader_Defines.h"
#include "LoadAnimator3D.h"
#include "AIAnimationClip.h"
#include "Helper_Func.h"
#include "ModelData.h"

CLoadAnimator3D::CLoadAnimator3D()
{
}

CLoadAnimator3D::CLoadAnimator3D(const CLoadAnimator3D& rhs)
	:CAnimator3D(rhs)
{
}

HRESULT CLoadAnimator3D::Initialize()
{

	return S_OK;
}


void CLoadAnimator3D::Render_GUI()
{
	ImGui::SeparatorText("Animator 3D");
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight * 5) + (ImGui::GetStyle().WindowPadding.y * 2);
	
	if (ImGui::Button("Add_Clip",ImVec2 { childWidth*0.45f, 0 }))
	{
		string path = Helper::OpenFile_Dialogue();
		Add_AIAnimation(path);
	}

	ImGui::SameLine();
	if (ImGui::Button("Save Clips", ImVec2{ childWidth* 0.45f, 0 })) {
		Save_Animations();
	}


	ImGui::BeginChild("##Animator 3DChild", ImVec2{ 0, childHeight }, true);
	for (size_t i = 0; i < m_pAnimClips.size(); i++)
	{
		bool isSelected = (m_iCurrentClipIndex == i);
		ImGui::PushID((int)i); 
		string key = "Anim_" + to_string(i);
		if (ImGui::Selectable(key.c_str(), isSelected,0, ImVec2{ childWidth *0.50f, textLineHeight }))
		{
			Chane_Animation(i);
		}
		ImGui::PopID();
		ImGui::SameLine();

		ImGui::PushID(("##" + key + "Loop").c_str());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 0));
		if (ImGui::Button(string(m_pAnimClips[i]->isLoop() ? "Once" : "Loop").c_str(), ImVec2{childWidth * 0.25f, textLineHeight+4}))
			static_cast<CAIAnimationClip*>(m_pAnimClips[i])->Change_Loop();
		ImGui::PopStyleVar();
		ImGui::PopID();


		if (isSelected) {
			ImGui::SetItemDefaultFocus(); // 선택된 항목에 포커스
		}
	}
	ImGui::EndChild();

	if (!m_pAnimClips.empty()) {
		if (ImGui::Button("Show Now Bones", ImVec2{ childWidth, 0 })) {
			m_bShowAnimatorSkeletons = !m_bShowAnimatorSkeletons;
		}
	}
	if (m_bShowAnimatorSkeletons && !m_pAnimClips.empty()) {
		ImGui::SetNextWindowSize(ImVec2(500, 400));
		if (ImGui::Begin("AnimationBones", &m_bShowAnimatorSkeletons, ImGuiWindowFlags_NoCollapse))
		{
			m_pAnimClips[m_iCurrentClipIndex]->Render_GUI();
		}
		ImGui::End();
	}
}

void CLoadAnimator3D::Set_Data(CModelData* pData)
{
	m_pData = pData;
	Safe_AddRef(m_pData);
	_float4x4 IdentityMatrix;
	XMStoreFloat4x4(&IdentityMatrix, XMMatrixIdentity());
	m_TransfromationMatrices.resize(m_pData->Get_BoneCount(), IdentityMatrix);
	m_CombinedMatrices.resize(m_pData->Get_BoneCount(), IdentityMatrix);
	m_FinalMatices.resize(m_pData->Get_BoneCount(), IdentityMatrix);
	m_ManipulateMatrices.resize(m_pData->Get_BoneCount(), IdentityMatrix);

	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		m_TransfromationMatrices[i] = m_pData->Get_TransformMatrix(i);
	}

	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		int parent = m_pData->Get_BoneParentIndex(i);

		if (parent == -1) {
			m_CombinedMatrices[i] = m_TransfromationMatrices[i];
		}
		else {
			_matrix ParentCombine = XMLoadFloat4x4(&m_CombinedMatrices[parent]);
			_matrix MyTransformation = XMLoadFloat4x4(&m_TransfromationMatrices[i]);
			XMStoreFloat4x4(&m_CombinedMatrices[i], MyTransformation * ParentCombine);
		}
	}

	for (size_t i = 0; i < m_pData->Get_BoneCount(); i++)
	{
		XMStoreFloat4x4(&m_FinalMatices[i], m_pData->Get_OffsetMatrix(i) * XMLoadFloat4x4(&m_CombinedMatrices[i]));
	}
}


HRESULT CLoadAnimator3D::Add_AIAnimation(const string& filePath)
{
	m_Importer.FreeScene();

	unsigned int iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	m_pAIScene = m_Importer.ReadFile(filePath.c_str(), iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	_uint ClipNum = m_pAIScene->mNumAnimations;
	for (size_t i = 0; i < ClipNum; i++)
	{
		CAIAnimationClip* pClip = CAIAnimationClip::Create(m_pAIScene->mAnimations[i], m_pData);
		m_pAnimClips.push_back(pClip);
		pClip->Set_ClipName(Helper::GetFileNameWithOutExtension(filePath));
	}
	
}

void CLoadAnimator3D::Release_Data()
{
}

void CLoadAnimator3D::Save_Animations()
{
	string Directory_Path = Helper::OpenFolder_Dialogue();
	filesystem::path directory(Directory_Path);

	for (size_t i = 0; i < m_pAnimClips.size(); i++)
	{
		static_cast<CAIAnimationClip*>(m_pAnimClips[i])->Save_AnimationClip(Directory_Path);
	}
}

CLoadAnimator3D* CLoadAnimator3D::Create()
{
	CLoadAnimator3D* instance = new CLoadAnimator3D();

	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CLoadAnimator3D::Clone()
{
	return new CLoadAnimator3D(*this);
}

void CLoadAnimator3D::Free()
{
	__super::Free();
	Safe_Release(m_pData);
	m_Importer.FreeScene();
}
