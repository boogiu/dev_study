#pragma once
#include "Engine_Defines.h"
#include "IService.h"

NS_BEGIN(Engine)

class CServiceHub
{
public:
	template<typename Service, typename Manager, typename ...Args>
	Service* Add_Service(Args&& ...args);

	template<typename Service>
	Service* Get_Service();

public:
	void Service_Free();
private:
	unordered_map<type_index, IService*> m_Services;
};

NS_END

template<typename Service, typename Manager, typename ...Args>
inline Service* CServiceHub::Add_Service(Args && ...args)
{
	Service* pService;

	if (pService = Get_Service<Service>())
		return pService;

	//부모 자식 관계인지 확인
	static_assert(is_base_of<Service, Manager>::value,"Manager must inherit from Service");
	static_assert(is_base_of<IService, Service>::value,"Service must inherit from Service Interface");

	pService =  Manager::Create(forward<Args>(args)...);
	m_Services[type_index(typeid(Service))] = pService;

	return pService;
}

template<typename Service>
inline Service* CServiceHub::Get_Service()
{
	auto iter = m_Services.find(type_index(typeid(Service)));
	if (iter == m_Services.end())
		return nullptr;

	else
		return static_cast<Service*>(iter->second);
}
