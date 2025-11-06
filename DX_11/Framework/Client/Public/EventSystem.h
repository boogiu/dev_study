#pragma once
#include "LevelObject.h"

NS_BEGIN(Client)

class CEventSystem :
    public CLevelObject
{
public:
	template<typename T> /*템플릿 특수화된 콜백*/
	using Callback = function<void(const T&)>;

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
	template<typename T>
	void Add_Listner(Callback<T> function);

	template<typename T>
	void OnBroadCast(const T& data);

private:
	template<typename T>
	vector<Callback<T>>& GetList();

private:
	/*타입 세이프라는 구조체를 상속받는 내부 벡터를 들고 있는 구조체 */
	struct TypeSafeStruct { virtual ~TypeSafeStruct() = default; };
	template<typename T>
	struct TypeSafeVector : TypeSafeStruct {
		vector<Callback<T>> callbacks;
	};

	unordered_map<type_index, TypeSafeStruct*> m_Callbacks;

public:
	static CEventSystem* Create();
	CGameObject* Clone(INIT_DESC* pArg) override;
	void Free() override;
};


template<typename T>
inline void CEventSystem::Add_Listner(Callback<T> function)
{
	auto& list = GetList<T>();
	list.push_back(move(function));
}

template<typename T>
inline void CEventSystem::OnBroadCast( const T& data)
{
	auto& list = GetList<T>();
	for (auto& Listner : list)
		Listner(data);
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
