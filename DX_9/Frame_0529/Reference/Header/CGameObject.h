#pragma once
#include "CBase.h"
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject :
	public CBase
{
protected:
	explicit CGameObject();
	virtual ~CGameObject() override;
public:
	CGameObject(const CGameObject&) = delete;
	CGameObject& operator=(const CGameObject&) = delete;

public:
	virtual HRESULT Ready_GameObject() = 0;
	virtual void Update_GameObject(float& dt) = 0;
	virtual void LateUpdate_GameObject(float& dt) = 0;

protected:
	void Update_Component(float& dt);
	void LateUpdate_Component(float& dt);
	void Release_Component();

public:
	template<typename T, typename ...Args>
	T* Add_Component(Args&& ...args);

	template<typename T>
	T* Get_Component();

	template<typename T>
	void Remove_Component();

private:
	vector<CComponent*> m_DynamicComponent;
	unordered_map<COM_TYPE, CComponent*> m_ComponentMap;
private:
	virtual void Free() = 0;
};

template<typename T, typename ...Args>
inline T* CGameObject::Add_Component(Args && ...args)
{
	if (Get_Component<T>() != nullptr) //우선 가진 컴포넌트 검색
		return nullptr;

	COM_TYPE Type = T::Get_StaticType();

	T* comPtr = T::Create(forward<Args>(args)...);

	comPtr->m_pOwner = this;
	RegisterOnSystem<T>(comPtr);

	if (comPtr->m_eUpdate == COM_UPDATE::DYNAMIC)
		m_DynamicComponent.push_back(comPtr);

	m_ComponentMap.insert({ Type,comPtr });
	return comPtr;
}

template<typename T>
inline T* CGameObject::Get_Component()
{
	auto iter = m_ComponentMap.find(T::Get_StaticType());

	if (iter == m_ComponentMap.end())
		return nullptr;

	return static_cast<T*>(iter->second);
}

template<typename T>
inline void CGameObject::Remove_Component()
{
	auto iter = m_ComponentMap.find(T::Get_StaticType());

	if (iter == m_ComponentMap.end())
		return;

	T* target = iter->second;
	ReleaseOnSystem<T>(target);

	if (target->m_eUpdate == COM_UPDATE::DYNAMIC) {

		auto vecIter = remove_if(m_DynamicComponent.begin(), m_DynamicComponent.end(),
			[&target](auto unique)->bool {
				return target == unique.get();
			});

		if (vecIter != m_DynamicComponent.end())
			m_DynamicComponent.erase(vecIter, m_DynamicComponent.end());
	}

	m_ComponentMap.erase(iter);

	Safe_Release(target);
}

END