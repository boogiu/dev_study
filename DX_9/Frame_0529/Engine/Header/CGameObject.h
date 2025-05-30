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
	virtual HRESULT Ready_GameObject() = 0;
	virtual void Update_GameObject(float dt) = 0;
	virtual void LateUpdate_GameObject(float dt) = 0;

protected:
	void Update_Component(float dt);
	void LateUpdate_Component(float dt);
public:
	template<typename T, typename ...Args>
	T* Add_Component(Args&& ...args);

	template<typename T>
	T* Get_Component();

	template<typename T>
	void Remove_Component();

private:
	vector<unique_ptr<CComponent>> m_Component;
	unordered_map<COM_TYPE, CComponent*> m_ComponentMap;
private:
	virtual void Free() override;
};

template<typename T, typename ...Args>
inline T* CGameObject::Add_Component(Args && ...args)
{
	if (Get_Component<T>() != nullptr)
		return nullptr;

	COM_TYPE Type = T::Get_StaticType();
	
	T* rawPtr = T::Create(forward<Args>(args)...);
	rawPtr->pOwner = this;
	RegisterOnSystem<T>(rawPtr);

	unique_ptr<T> comp(rawPtr);
	
	m_Component.push_back(move(comp));
	m_ComponentMap.insert({ Type,rawPtr });

	return rawPtr;
}

template<typename T>
inline T* CGameObject::Get_Component()
{
	auto iter = m_ComponentMap.find(T::Get_StaticType());
	
	if(iter == m_ComponentMap.end())
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

	auto vecIter = remove_if(m_Component.begin(), m_Component.end(),
		[&target](auto unique)->bool {
			return target == unique.get();
		});

	if(vecIter != m_Component.end())
		m_Component.erase(vecIter, m_Component.end()); 
	
	m_ComponentMap.erase(iter);
}
END