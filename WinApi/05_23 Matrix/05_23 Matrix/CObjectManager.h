#pragma once
class CObject;

class CObjectManager
{
private:
	CObjectManager();
	~CObjectManager();

public:
	static CObjectManager& Get_Instance() {
		static CObjectManager instance;
		return instance;
	}

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Release();

public:
	template<typename T, typename ...Args>
	T* Create_Object(Args&& ...args) {
		ObjectType type = T::StaticType();

		auto& objectPool = m_PoolContainer[type]; //  참조로 받아야 함

		for (auto& obj : objectPool) {
			if (!obj->Get_Active()) {
				obj->Set_Active(true);
				m_ActiveContainer.push_back(std::move(obj));
				return static_cast<T*>(m_ActiveContainer.back().get());
			}
		}

		auto obj = std::make_unique<T>(<Args>(args)...);
		T* raw = obj.get();
		m_ActiveObjects.push_back(std::move(obj));
		return raw;
	}

	template<typename T>
	void Destroy_Object(T* obj) {
		if (!obj) return;

		obj->SetActive(false); // 비활성화
		ObjectType type = T::StaticType();

		auto iter = std::find_if(m_ActiveObjects.begin(), m_ActiveObjects.end(),
			[obj](const std::unique_ptr<CObject>& ptr) {
				return ptr.get() == obj;
			});

		if (iter == m_ActiveContainer.end()) return;
		std::unique_ptr<CObject> extracted = std::move(*iter);
		m_PoolContainer[type].push_back(std::move(extracted);

		m_ActiveObjects.erase(iter);
	}

private:
	std::unordered_map <ObjectType, std::vector<std::unique_ptr<CObject>>> m_PoolContainer;
	std::vector<std::unique_ptr<CObject>> m_ActiveContainer;
};

