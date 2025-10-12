#include "Loader_Defines.h"
#include "ModelObject.h"

#include "Helper_Func.h"
#include "GameInstance.h"
#include "IInputService.h"
#include "IObjectService.h"

#include "PartsObject.h"

#include "ObjectContainer.h"
#include "SkeletonFollower.h"
#include "LoadAnimator3D.h"
#include "LoadStaticModel.h"
#include "LoadSkeletalModel.h"
#include "Animator3D.h"

#include "LoadMaterial.h"
#include "AIMaterial.h"

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
	Add_Component<CObjectContainer>();

	return S_OK;
}

void CModelObject::Priority_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Priority_UpdateChild(dt);
}

void CModelObject::Update(_float dt)
{
	if (m_pAnimator) {
		m_pAnimator->Update_Animation(dt);
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_LEFT))
			m_fMeshAngle += dt * 45;
		if (CGameInstance::GetInstance()->Get_InputDev()->Key_Down(VK_RIGHT))
			m_fMeshAngle -= dt * 45;

		_matrix RotateMat = XMMatrixIdentity();
		RotateMat = XMMatrixRotationX(XMConvertToRadians(m_fMeshAngle));

		//m_pAnimator->Control_Bone("Armature_Spine_2", RotateMat);
		m_pAnimator->Control_Bone("Armature_Spine_2", RotateMat);
		//m_pAnimator->Control_Bone("Armature_Spine_3", RotateMat);
	}
	Get_Component<CObjectContainer>()->UpdateChild(dt);

}

void CModelObject::Late_Update(_float dt)
{
	Get_Component<CObjectContainer>()->Late_UpdateChild(dt);
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


	if (ImGui::Button("Add Part")) {
		Add_Part();
	}
	__super::Render_GUI();
}

HRESULT CModelObject::Load_AIScene(const string& filePath)
{
	m_Importer.FreeScene();
	ReleasPrevModel();

	unsigned int iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	m_pAIScene = m_Importer.ReadFile(filePath.c_str(), iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	string fileName = Helper::GetFileNameWithOutExtension(filePath);

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
	CLoadStaticModel* pModel = Add_Component< CLoadStaticModel>();
	pModel->Load_Model(m_pAIScene, fileName);
	return S_OK;
}

HRESULT CModelObject::Load_Animated(const string& fileName)
{
	CLoadSkeletalModel* pModel = Add_Component< CLoadSkeletalModel>();

	CLoadAnimator3D* pAnimator = CLoadAnimator3D::Create();
	pAnimator->Set_Owner(this);
	m_Components.emplace(type_index(typeid(CAnimator3D)), pAnimator);

	m_pAnimator = pAnimator;

	pModel->Load_Model(m_pAIScene, fileName);
	pAnimator->Set_Data(pModel->Get_ModelData());

	return S_OK;
}

HRESULT CModelObject::Save_AIScene()
{
	HRESULT hr = {};

	if (HasBones()) {
		CLoadSkeletalModel* pModel = dynamic_cast<CLoadSkeletalModel*>(Get_Component<CModel>());
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

void CModelObject::ReleasPrevModel()
{
	Remove_Component<CLoadStaticModel>();
	Remove_Component<CLoadSkeletalModel>();
	Remove_Component<CAnimator3D>();
	Remove_Component<CMaterial>();
	Remove_Component<CModel>();
	 
	m_pAnimator = nullptr;
}

void CModelObject::Add_Part()
{
	IObjectService* pObjMgr = CGameInstance::GetInstance()->Get_ObjectMgr();

	CGameObject* ModelPart = Builder::Create_Object({ "Model_Level" ,"Proto_GameObject_Part" })
		.Build("Parts");
	Get_Component<CObjectContainer>()->Add_Child(ModelPart);

	pObjMgr->Add_Object(ModelPart, { "Model_Level","Model_Layer" });

	dynamic_cast<CPartsObject*>(ModelPart)->Inject_Master(
		Get_Component<CLoadSkeletalModel>(),
		Get_Component<CAnimator3D>()
	);
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
