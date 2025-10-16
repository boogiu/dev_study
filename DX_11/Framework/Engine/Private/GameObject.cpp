#include "GameObject.h"
#include "GameInstance.h"
#include "Builder.h"
#include "IRenderService.h"
#include "StaticModel.h"
#include "SkeletalModel.h"
#include "Material.h"
#include "Animator3D.h"
#include "ObjectContainer.h"
#include "Child.h"
#include "SkeletonFollower.h"
#include "IMeshProvider.h"
#include "DebugRender.h"

_uint CGameObject::s_NextID = 1;

CGameObject::CGameObject()
	:m_ObjectID(s_NextID++)
{
}

CGameObject::CGameObject(const CGameObject& rhs)
	:m_ObjectID(s_NextID++)
{
	/*트랜스폼은 가장 먼저.*/
	type_index transform = type_index(typeid(CTransform));

	auto iter = rhs.m_Components.find(transform);

	if (iter != rhs.m_Components.end()) {
		CComponent* myTransform = iter->second->Clone();
		myTransform->Set_Owner(this);
		m_Components.emplace(transform, myTransform);
	}


	for (auto& pair : rhs.m_Components) {

		if (pair.first == type_index(typeid(CTransform)))
			continue;

		if (pair.first == type_index(typeid(CModel)))
			continue;

		else {
			CComponent* comp = pair.second->Clone();
			comp->Set_Owner(this);
			m_Components.emplace(pair.first, comp);

			if (dynamic_cast<CModel*>(comp)) {
				m_Components.emplace(type_index(typeid(CModel)), comp);
				Safe_AddRef(comp);
			}
		}
	}

	m_pTransform = Get_Component<CTransform>();
	Safe_AddRef(m_pTransform);
}

HRESULT CGameObject::Initialize_Prototype()
{
	//원본 생성 시, 필요 초기화들 진행
	m_pTransform = Add_Component<CTransform>();
	Safe_AddRef(m_pTransform);

	return S_OK;
}

HRESULT CGameObject::Initialize(INIT_DESC* pArg)
{

	if (!m_pTransform) {
		m_pTransform = Add_Component<CTransform>();
		Safe_AddRef(m_pTransform);
	}

	if (pArg == nullptr)
		return S_OK;

	GAMEOBJECT_DESC* obj = static_cast<GAMEOBJECT_DESC*>(pArg);
	for (auto& pair : m_Components)
	{
		auto iter = obj->CompDesc.find(pair.first);
		/*각자 컴포넌트에 맞는 설명체 찾아서 넣어줌. 없으면 그냥 이니셜ㄹ라이즈*/
		if (iter == obj->CompDesc.end())
			pair.second->Initialize(nullptr);
		else
			pair.second->Initialize(iter->second);
	}

	m_InstanceName = obj->InstanceName;
	return S_OK;
}

void CGameObject::Pre_EngineUpdate(_float dt)
{
	if (CChild* pChild = Get_Component<CChild>()) {
		m_isRootObject = false;
	}
	else {
		m_isRootObject = true;
	}

	for (auto& child : Get_Children()) {
		child->Pre_EngineUpdate(dt);
	}
}

void CGameObject::Post_EngineUpdate(_float dt)
{
	/*패킷은 용도별로 따로 만든다.*/
	OPAQUE_PACKET packet;
	packet.pModel = { nullptr };
	packet.bSkinning = false;
	packet.pMaterial = Get_Component<CMaterial>();
	packet.pWorldMatrix = m_pTransform->Get_WorldMatrix();

	if (FAILED(Make_OpaquePacket(packet))) return;


	for (size_t i = 0; i < packet.pModel->Get_MeshCount(); i++)
	{
		if (!packet.pModel->isDrawable(i)) continue;
		packet.DrawIndex = i;
		packet.MaterialIndex = packet.pModel->Get_MaterialIndex(i);
		CGameInstance::GetInstance()->Get_RenderSystem()->Submit_Opaque(packet);
	}


#ifdef _DEBUG
	DEBUG_PACKET debugPacket = {};
	debugPacket.pModel = Get_Component<CModel>();
	debugPacket.pDebug = Get_Component<CDebugRender>();
	debugPacket.pWorldMatrix = m_pTransform->Get_WorldMatrix();
	if (debugPacket.pDebug) {
		for (size_t i = 0; i < debugPacket.pDebug->Get_DebugBoxCount(); i++)
		{
			if (!debugPacket.pModel->isDrawable(i)) continue;
			debugPacket.DrawIndex = i;
			CGameInstance::GetInstance()->Get_RenderSystem()->Submit_Debug(debugPacket);
		}
	}
	
#endif // _DEBUG

	for (auto& child : Get_Children()) {
		child->Post_EngineUpdate(dt);
	}
}

void CGameObject::Render_GUI()
{
	m_pTransform->Render_GUI();
	for (auto& pair : m_Components) {
		if (pair.first == type_index(typeid(CTransform))) continue;
		if (pair.first == type_index(typeid(CModel))) continue;
		pair.second->Render_GUI();
	}
}

void CGameObject::RenderHierarchy(CGameObject*& SelectedObject, bool isSelected)
{
	ImGui::PushID((int)m_ObjectID);

	const vector<CGameObject*>& Children = Get_Children();

	ImGuiTreeNodeFlags flags = 
		ImGuiTreeNodeFlags_Framed|
		ImGuiTreeNodeFlags_OpenOnArrow |
		//ImGuiTreeNodeFlags_SpanFullWidth |
		(isSelected ? ImGuiTreeNodeFlags_Selected : 0) |
		(Children.empty() ? (ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen) : 0);

	string TreeNodeName = m_InstanceName + " (" + to_string(Children.empty()? 0 : Children.size()) + ")";
	bool opened = ImGui::TreeNodeEx(TreeNodeName.c_str(), flags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		SelectedObject = this;
	if (opened && !Children.empty()) {  
		for (auto& childObject : Children) {
			if (!childObject) continue;
			bool childSelected = (SelectedObject == childObject);
			childObject->RenderHierarchy(SelectedObject, childSelected);
		}

		ImGui::TreePop();
	}

	ImGui::PopID();
}

void CGameObject::Set_Layer(CLayer* pLayer)
{
	m_pLayer = pLayer;
}

const vector<CGameObject*> CGameObject::Get_Children()
{
	vector<CGameObject*> empty;

	CObjectContainer* pContainer = Get_Component<CObjectContainer>();
	if (pContainer) {
		return pContainer->Get_Children();
	}
	else {
		return empty;
	}
}


_float4x4* CGameObject::Get_WorldMatrix()
{
	return m_pTransform->Get_WorldMatrix();
}

_float4 CGameObject::Get_Position()
{
	_float4 pos;
	XMStoreFloat4(&pos, m_pTransform->Get_Pos());
	return pos;
}

HRESULT CGameObject::Make_OpaquePacket(OPAQUE_PACKET& packet)
{
	packet.pModel = Get_Component<CModel>();
	if (packet.pModel&&!packet.pModel->isReadyToDraw()) return E_FAIL;
	packet.bSkinning = dynamic_cast<CSkeletalModel*>(packet.pModel) ? true : false;

	if (auto Animator = Get_Component<CAnimator3D>()) {
		packet.pPayLoad = Animator;
	}
	else if (auto Follower = Get_Component<CSkeletonFollower>()) {
		packet.pPayLoad = Follower;
	}
	else {
		packet.pPayLoad = monostate{};
	}

	if (packet.pModel == nullptr) {
		return E_FAIL;
	}
	return S_OK;
}

void CGameObject::Free()
{
	__super::Free();
	for (auto& pair : m_Components) {
		pair.second->Set_Active(false);
		Safe_Release(pair.second);
	}

	Safe_Release(m_pTransform);
}
