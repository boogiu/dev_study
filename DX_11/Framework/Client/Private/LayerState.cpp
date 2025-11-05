#include "Client_Defines.h"
#include "LayerState.h"

#include "State.h"
#include "Animator3D.h"


CLayerState::CLayerState()
{
}

void CLayerState::Excute(CState* rootState)
{
	m_pCurrent = rootState;
	m_pCurrent->OnEnter();
}
void CLayerState::Update(_float dt)
{
	CState* next = nullptr;
	CState* state = m_pCurrent;

	next = state->HandleTransition();

	if (next && next != m_pCurrent)
	{
		HRESULT exit = m_pCurrent->OnExit();

		m_pCurrent = next;

		HRESULT enter = m_pCurrent->OnEnter();
	}

	m_pCurrent->OnUpdate(dt);
}

void CLayerState::Request_ChangeState(const string& NextState)
{
	auto iter = m_States.find(NextState);
	if (iter == m_States.end())
		return;

	if (m_pCurrent->GetName() == NextState)
		return;

	m_pCurrent->OnExit();
	m_pCurrent = iter->second;
	m_pCurrent->OnEnter();
}

CState* CLayerState::Get_State(const string& name)
{
	auto iter = m_States.find(name);
	if (iter == m_States.end()) {
		return nullptr;
	}

	return iter->second;
}

_uint CLayerState::Get_CurrentMask()
{
	if (m_pCurrent)
		return m_pCurrent->Get_InputMask();
	else
		return 0xfffffff;
}

void CLayerState::OnCollisionEnter(COLLISION_CONTEXT context)
{
	if (m_pCurrent)
		m_pCurrent->OnCollisionEnter(context);
}

void CLayerState::OnCollisionStay(COLLISION_CONTEXT context)
{
	if (m_pCurrent)
		m_pCurrent->OnCollisionStay(context);
}

void CLayerState::OnCollisionExit(COLLISION_CONTEXT context)
{
	if (m_pCurrent)
		m_pCurrent->OnCollisionExit(context);
}

void CLayerState::Render_State()
{
	ImGui::SeparatorText(m_pCurrent->GetName().c_str());
}

CLayerState* CLayerState::Create()
{
	CLayerState* instance = new CLayerState();
	return instance;
}

void CLayerState::Free()
{
	__super::Free();

	for (auto& States : m_States) {
		Safe_Release(States.second);
	}
	m_States.clear();
}
