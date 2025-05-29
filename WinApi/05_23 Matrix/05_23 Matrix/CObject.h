#pragma once
#include "CComponent.h"


class CObject
{
public:
	CObject();
	virtual ~CObject();

public:
	virtual void Initialize() PURE;
	virtual void Update(float dt) PURE;
	virtual void Late_Update(float dt) PURE;
	virtual void Release() PURE;

public:
	virtual ObjectType GetType() PURE;

public:
	void Set_Active(bool _active) { m_bActive = _active; }
	bool Get_Active() { return m_bActive; }

protected:
	void Update_Component(float dt);
	void Late_Update_Component(float dt);

// 컴포넌트 가져오는 템플릿들
public:
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args) {
		if (GetComponent<T>())
			return nullptr;
		ComponentType type = T::StaticType();
		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		component->m_pOwner = this; //컴포넌트에게 소유자 전달
		T* rawPointer = component.get();
		RegisterOnSystem<T>(rawPointer);

		componentMap.insert({ type, rawPointer });
		components.push_back(std::move(component));
		return rawPointer;
	}

	template<typename T>
	T* GetComponent() {
		ComponentType type = T::StaticType();
		auto iter = componentMap.find(type);

		if (iter != componentMap.end()) {
			return static_cast<T*>(iter->second);
		}
		else {
			return nullptr;
		}
	}

	template<typename T>
	void RemoveComponent() {
		ComponentType type = T::StaticType();
		auto it = componentMap.find(type);
		if (it == componentMap.end()) return;
		CComponent* target = it->second;
		ReleaseOnSystem<T>(target);

		auto vecIter = std::remove_if(components.begin(), components.end(),
			[target](const std::unique_ptr<CComponent>& comp)->bool {
				return comp.get() == target;
			});
		components.erase(vecIter, components.end());
		componentMap.erase(it);
	}

private:
	bool m_bActive;
	std::vector<std::unique_ptr<CComponent>> components;								//객체 생성 & 해제 용도
	std::unordered_map<ComponentType, CComponent*> componentMap; //객체 접근 용도
};

