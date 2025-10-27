#include "Client_Defines.h"
#include "PlayerHFSM.h"
#include "State.h"
#include "Player.h"
#include "Animator3D.h"

CPlayerHFSM::CPlayerHFSM(class CPlayer* pPlayer)
	:m_pPlayer{pPlayer}
{
}

HRESULT CPlayerHFSM::Initialize()
{
	return S_OK;
}

void CPlayerHFSM::Excute(CState* rootState)
{
	m_pRoot = rootState;
	m_pCurrent = rootState;
	m_pCurrent->OnEnter();
}

void CPlayerHFSM::Update(_float dt)
{
	CState* next = nullptr;
	CState* state = m_pCurrent;

	while (state && !next)
	{
		next = state->HandleTransition(); // 리프부터 부모까지 검사
		state = state->GetParent();
	}

	if (next && next != m_pCurrent)
	{
		
		m_pCurrent->OnExit();
		m_pCurrent = next;
	
		m_pCurrent->OnEnter();

	}

	if (m_pCurrent->GetParent())
		m_pCurrent->GetParent()->OnUpdate(dt);

	m_pCurrent->OnUpdate(dt);
}

void CPlayerHFSM::Request_ChangeState(const string& NextState)
{
	auto iter = m_States.find(NextState);
	if (iter == m_States.end())
		return;

	if (CanTransition(m_pCurrent, m_States[NextState])) {
		m_pCurrent->OnExit();
		m_pCurrent = m_States[NextState];
		m_pCurrent->OnEnter();
	}

	return;
}

_bool CPlayerHFSM::CanTransition(CState* from, CState* To)
{
	return true;
}

CState* CPlayerHFSM::Get_State(const string& name)
{
	auto iter = m_States.find(name);
	if (iter == m_States.end()) {
		return nullptr;
	}

	return iter->second;
}

void CPlayerHFSM::Render_State(CPlayer* pPlayer)
{
	ImGui::SeparatorText(m_pCurrent->GetName().c_str());
	ImGui::SeparatorText(m_pPlayer->Get_Component<CAnimator3D>()->Get_CurrentAnimName().c_str());
	if (m_pCurrent->GetParent())
		m_pCurrent->GetParent()->Render_State();

	m_pCurrent->Render_State();
}

CPlayerHFSM* CPlayerHFSM::Create(class CPlayer* pPlayer)
{
	CPlayerHFSM* instance = new CPlayerHFSM(pPlayer);
	if (FAILED(instance->Initialize())) {
		Safe_Release(instance);
	}
	return instance;
}

void CPlayerHFSM::Free()
{
	__super::Free();

	for (auto& States : m_States) {
		Safe_Release(States.second);
	}
	m_States.clear();
}
