#pragma once
#include "LevelObject.h"

NS_BEGIN(Client)

class CEventSystem :
    public CLevelObject
{
public:
	struct CallbackBase {
		CGameObject* pOwner;   // 리스너 객체 주소
		virtual bool IsOwner(void* ptr) const { return pOwner == ptr; }
	};

	template<typename T> /*템플릿 특수화된 콜백*/
	struct Callback : CallbackBase {
		function<void(const T&)> func;
	};

	/*타입 세이프라는 구조체를 상속받는 내부 벡터를 들고 있는 구조체 */
	struct TypeSafeStruct { 
		virtual ~TypeSafeStruct() = default; 
		virtual void RemoveByOwner(CGameObject* pOwner) PURE;
	};
	template<typename T>
	struct TypeSafeVector : TypeSafeStruct {
		vector<Callback<T>> callbacks;
		virtual void RemoveByOwner(CGameObject* pOwner) override;
	};

private:
	CEventSystem();
	CEventSystem(const CEventSystem& rhs);
	~CEventSystem();

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(INIT_DESC* pArg) override;
	void Priority_Update(_float dt)override;
	void Update(_float dt)override;
	void Late_Update(_float dt)override;

public:
	template<typename Object, typename T>
	void Add_Listner(Object* owner, void(Object::* method)(const T&));
	template<typename T>
	void OnBroadCast(const T& data);
	void UnregisterAll(CGameObject* pObj);

private:
	template<typename T>
	vector<Callback<T>>& GetList();

private:

	unordered_map<type_index, TypeSafeStruct*> m_Callbacks;

public:
	static CEventSystem* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	void Free() override;
};

template<typename Object, typename T>
inline void CEventSystem::Add_Listner(Object* owner, void(Object::* method)(const T&))
{
	Callback<T> cb;
	cb.pOwner = owner;
	cb.func = [owner, method](const T& evt) {(owner->*method)(evt);};
	auto& list = GetList<T>();
	list.push_back(move(cb));
}

template<typename T>
inline void CEventSystem::OnBroadCast( const T& data)
{
	auto& list = this->template GetList<T>();

	for (auto& listener : list)
		listener.func(data); 
}

template<typename T>//(이벤트 시스템 안에 있는 템플릿이라고 알려줘야함)
inline vector<typename CEventSystem::template Callback<T>>& CEventSystem::GetList()
{
	const type_index idx = type_index(typeid(T)); //타입 인덱스로 가져와서 

	auto it = m_Callbacks.find(idx);
	if (it == m_Callbacks.end())
	{
		m_Callbacks[idx] = new TypeSafeVector<T>(); //아까 타입 세이프로 생성
		it = m_Callbacks.find(idx);
	}

	return static_cast<TypeSafeVector<T>*>(it->second)->callbacks; // 이터 세컨드에서 타입 캐스트. 어차피 안전함.
}
NS_END

template<typename T>
inline void CEventSystem::TypeSafeVector<T>::RemoveByOwner(CGameObject* pOwner)
{
	callbacks.erase(
		remove_if(
			callbacks.begin(),
			callbacks.end(),
			[&](const Callback<T>& callback) {
				return callback.pOwner == pOwner;   // 제거 조건
			}
		),
		callbacks.end()
	);
}
