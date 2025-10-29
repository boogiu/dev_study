#include "Client_Defines.h"
#include "StateMachine.h"
#include "LayerState.h"

CStateMachine::CStateMachine()
{
}

void CStateMachine::Free()
{
	__super::Free();
	for (auto& layers : m_LayerStates) {
		Safe_Release(layers.second);
	}
	m_LayerStates.clear();
}
