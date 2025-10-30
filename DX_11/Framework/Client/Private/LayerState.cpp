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

	next = state->HandleTransition(); // 리프부터 부모까지 검사

	if (next && next != m_pCurrent)
	{
		m_pCurrent->OnExit();

		m_pCurrent = next;

		m_pCurrent->OnEnter();
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
	m_pCurrent = m_States[NextState];
	m_pCurrent->OnEnter();

	return;
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

void CLayerState::Render_State()
{
	ImGui::SeparatorText(m_pCurrent->GetName().c_str());
	m_pCurrent->Render_State();
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
