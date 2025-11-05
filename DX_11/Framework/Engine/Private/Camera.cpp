#include "Camera.h"
#include "GameObject.h"

CCamera::CCamera()
{
}

CCamera::CCamera(const CCamera& rhs)
	:CComponent(rhs)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(COMPONENT_DESC* pArg)
{
	if (pArg == nullptr)
		return S_OK;

	CAMERA_DESC* Camera = static_cast<CAMERA_DESC*>(pArg);
	m_pTransform = m_pOwner->Get_Component<CTransform>();
	Safe_AddRef(m_pTransform); 

	m_fFov = Camera->fFov;
	m_fFar = Camera->fFar;
	m_fNear = Camera->fNear;
	m_fAspect = Camera->fAspect;
	
	return S_OK;
}

_float4x4 CCamera::Get_ViewMatrix()
{
	return m_pTransform->Get_InverseWorldMatrix();
}

_matrix CCamera::Get_ProjMatrix()
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fFov), m_fAspect,m_fNear,m_fFar);
}

void CCamera::Lerp_FOV(_float dst, _float dt)
{
	m_fFov= m_fFov + (dst - m_fFov) * dt;
}

void CCamera::Render_GUI()
{
	ImGui::SeparatorText("Camera");
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight * 8) + (ImGui::GetStyle().WindowPadding.y * 2);

	ImGui::BeginChild("##CameraChild", ImVec2{ 0, childHeight }, true);
	ImGui::Text("Field of View");
	ImGui::InputFloat("##FoV", &m_fFov, 1.0f, 0.0f, "%.1f");

	ImGui::Text("Near Plane");
	ImGui::InputFloat("##Near", &m_fNear, 1.0f, 0.0f, "%.1f");

	ImGui::Text("Far Plane");
	ImGui::InputFloat("##Far", &m_fFar, 1.0f, 0.0f, "%.1f");

	ImGui::EndChild();
}

CCamera* CCamera::Create()
{
	CCamera* instance = new CCamera();
	if (FAILED(instance->Initialize_Prototype())) {
		MSG_BOX("Camera Create Failed : CCamera");
		Safe_Release(instance);
	}
	return instance;
}

CComponent* CCamera::Clone()
{
	CCamera* instance = new CCamera(*this);
	return instance;
}

void CCamera::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
}
