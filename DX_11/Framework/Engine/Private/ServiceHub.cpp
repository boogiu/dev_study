#include "ServiceHub.h"

void CServiceHub::Service_Free()
{
	for (auto& pair : m_Services) {
		Safe_Release(pair.second);
	}
	m_Services.clear();
}
