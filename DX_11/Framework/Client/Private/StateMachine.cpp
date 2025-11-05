#include "Client_Defines.h"
#include "StateMachine.h"
#include "LayerState.h"

CStateMachine::CStateMachine()
{
}

void CStateMachine::OnCollisionEnter(COLLISION_CONTEXT context)
{
	for (auto& layers : m_LayerStates) {
		if (layers.second)
			layers.second->OnCollisionEnter(context);
	}
}

void CStateMachine::OnCollisionStay(COLLISION_CONTEXT context)
{
	for (auto& layers : m_LayerStates) {
		if (layers.second)
			layers.second->OnCollisionStay(context);
	}
}

void CStateMachine::OnCollisionExit(COLLISION_CONTEXT context)
{
	for (auto& layers : m_LayerStates) {
		if (layers.second)
			layers.second->OnCollisionExit(context);
	}
}
void CStateMachine::Free()
{
	__super::Free();
	for (auto& layers : m_LayerStates) {
		Safe_Release(layers.second);
	}
	m_LayerStates.clear();
}
