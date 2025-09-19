#include "Loader_Defines.h"
#include "ModelObject.h"
#include "LoadStaticModel.h"
#include "LoadAnimatedModel.h"
#include "LoadMaterial.h"
#include "AIMaterial.h"
#include "Helper_Func.h"
#include "LoadAnimator3D.h"
#include "Animator3D.h"

CModelObject::CModelObject()
{
}

CModelObject::CModelObject(const CModelObject& rhs)
	:CGameObject(rhs)
{
}

HRESULT CModelObject::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CModelObject::Initialize(INIT_DESC* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CModelObject::Priority_Update(_float dt)
{
}

void CModelObject::Update(_float dt)
{
	if (m_pAnimator)
		m_pAnimator->Update_Animation(dt);
}

void CModelObject::Late_Update(_float dt)
{
}

void CModelObject::Render_GUI()
{
	float childWidth = ImGui::GetContentRegionAvail().x;
	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float childHeight = (textLineHeight + 2) + (ImGui::GetStyle().WindowPadding.y * 2);

	ImGui::SeparatorText("Model Load & Save");
	ImGui::BeginChild("##Loaded OBJECT BTN", ImVec2{ 0, childHeight }, true);

	if (ImGui::Button("Model Load")) {
		string path = Helper::OpenFile_Dialogue();
		Load_AIScene(path);
	}
	ImGui::SameLine();

	if (ImGui::Button("Model Save")) {
		Save_AIScene();
	}
	ImGui::EndChild();

	__super::Render_GUI();
}

HRESULT CModelObject::Load_AIScene(const string& filePath)
{
	m_Importer.FreeScene();

	unsigned int iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	m_pAIScene = m_Importer.ReadFile(filePath.c_str(), iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	string fileName = Helper::GetFileNameWithExtension(filePath);

	CLoadMaterial* pMaterial = CLoadMaterial::Create();
	pMaterial->Set_Owner(this);
	if (HasBones()) {
		Load_Animated(fileName);
		m_Components.emplace(type_index(typeid(CMaterial)), pMaterial);
		_uint NumMaterial = m_pAIScene->mNumMaterials;
		pMaterial->Load_Material(NumMaterial, m_pAIScene->mMaterials, filePath);
		pMaterial->LinkShader("VTX_SkinMesh.hlsl");
	}
	else {
		Load_Static(fileName);
		m_Components.emplace(type_index(typeid(CMaterial)), pMaterial);
		_uint NumMaterial = m_pAIScene->mNumMaterials;
		pMaterial->Load_Material(NumMaterial, m_pAIScene->mMaterials, filePath);
		pMaterial->LinkShader("VTX_Mesh.hlsl");
	}

	return S_OK;
}

HRESULT CModelObject::Load_Static(const string& fileName)
{
	RealesPrevModel();
	CLoadStaticModel* pModel = CLoadStaticModel::Create();
	pModel->Set_Owner(this);
	m_Components.emplace(type_index(typeid(CStaticModel)), pModel);

	pModel->Load_Model(m_pAIScene, fileName);
	return S_OK;
}

HRESULT CModelObject::Load_Animated(const string& fileName)
{
	RealesPrevModel();
	CLoadAnimatedModel* pModel = CLoadAnimatedModel::Create();
	pModel->Set_Owner(this);
	m_Components.emplace(type_index(typeid(CAnimatedModel)), pModel);
	CLoadAnimator3D* pAnimator = CLoadAnimator3D::Create();
	pAnimator->Set_Owner(this);
	m_Components.emplace(type_index(typeid(CAnimator3D)), pAnimator);
	m_pAnimator = pAnimator;

	pModel->Load_Model(m_pAIScene, fileName);
	pAnimator->Set_Data(pModel->Get_Data());

	return S_OK;
}


HRESULT CModelObject::Save_AIScene()
{
	HRESULT hr = {};

	if (HasBones()) {
		CLoadAnimatedModel* pModel = dynamic_cast<CLoadAnimatedModel*>(Get_Component<CAnimatedModel>());
		hr = pModel->Save_Model();
	}
	else {
		CLoadStaticModel* pModel = dynamic_cast<CLoadStaticModel*>(Get_Component<CStaticModel>());
		hr = pModel->Save_Model();
	}
	CLoadMaterial* pMaterial = dynamic_cast<CLoadMaterial*>(Get_Component<CMaterial>());

	hr = pMaterial->Save_Material();

	return S_OK;
}

_bool CModelObject::HasBones()
{
	if (nullptr == m_pAIScene)
		return false;

	for (size_t i = 0; i < m_pAIScene->mNumMeshes; ++i)
	{
		if (m_pAIScene->mMeshes[i]->HasBones())
			return true; 
	}

	return false;
}

void CModelObject::RealesPrevModel()
{
	Remove_Component<CStaticModel>();
	Remove_Component<CAnimatedModel>();
	Remove_Component<CAnimator3D>();
	Remove_Component<CMaterial>();
	m_pAnimator = nullptr;
}

CModelObject* CModelObject::Create()
{
	CModelObject* instance = new CModelObject();
	if (FAILED(instance->Initialize_Prototype()))
	{
		MSG_BOX("Object Create Failed : CModelObject");
		Safe_Release(instance);
	}

	return instance;
}

CGameObject* CModelObject::Clone(INIT_DESC* pArg)
{
	CModelObject* instance = new CModelObject(*this);

	if (FAILED(instance->Initialize(pArg)))
	{
		MSG_BOX("Object Clone Failed : CModelObject");
		Safe_Release(instance);
	}

	return instance;
}

void CModelObject::Free()
{
	__super::Free();
	m_Importer.FreeScene();
}
