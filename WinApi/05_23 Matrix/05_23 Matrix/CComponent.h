#pragma once
class CObject;
class CRenderer;

class CComponent
{
public:
	CComponent();
	virtual ~CComponent();
public:
	virtual void Initialize() PURE;
	virtual void Update(float dt) PURE;
	virtual void Late_Update(float dt) PURE;
	virtual ComponentType GetType() const PURE;

public:
	CObject* m_pOwner;
};

template<typename T>
void RegisterOnSystem(T* component) {}

template<typename T>
void ReleaseOnSystem(T* component) {}

// 특수화는 선언만 남김

template<>
void RegisterOnSystem<CRenderer>(CRenderer* renderer);

template<>
void ReleaseOnSystem<CRenderer>(CRenderer* renderer);

