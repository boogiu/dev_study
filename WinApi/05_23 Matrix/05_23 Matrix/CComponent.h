#pragma once

class CObject;

enum class ComponentType {
	Transform,
	Renderer,
	Collider,
};

class CComponent
{
public:
	CComponent();
	virtual ~CComponent();
public:
	virtual void Initialize() PURE;
	virtual void Update(float dt) PURE;
	virtual ComponentType GetType() const PURE;

public:
	CObject* m_pOwner;
};

template<typename T>
void RegisterOnSystem(T* component) {

}
template<typename T>
void ReleaseOnSystem(T* component) {

}
//특수화 해둘 것

//template<>
//void RegisterOnSystem<CRenderer>(CRenderer* renderer) {
//
//}
