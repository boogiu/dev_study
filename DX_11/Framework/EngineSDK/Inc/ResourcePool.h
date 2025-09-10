#pragma once
#include "Engine_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)

template<typename T>
class ResourcePool {
public:
	HRESULT Add_Resource(const string& key, T* resource) {
		if (m_Resources.count(key)) {
			string err = key + "is Already Exsist. ";
			MessageBoxA(nullptr, err.c_str(), "ResourcePool", MB_OK);
			return E_FAIL;
		}
		
		m_Resources.emplace(key, resource);
	}

	T* Find(const string& key) {

		auto iter = m_Resources.find(key);

		if (iter != m_Resources.end())
			return iter->second;
		else
			return nullptr;
	}

	void Clear() {
		for (auto& pair : m_Resources)
			Safe_Release(pair.second);

		m_Resources.clear();
	}

private:
	unordered_map<string, T*> m_Resources;
};

template<>
class ResourcePool<string> {
public:
	HRESULT Add_Resource(const string& key, string resource) {
		if (m_Resources.count(key)) {
			string err = key + "is Already Exsist. ";
			MessageBoxA(nullptr, err.c_str(), "ResourcePool", MB_OK);
			return E_FAIL;
		}

		m_Resources.emplace(key, resource);
	}

	string Find(const string& key) {

		auto iter = m_Resources.find(key);

		if (iter != m_Resources.end())
			return iter->second;
		else
			return string();
	}

	void Clear() {
		m_Resources.clear();
	}

private:
	unordered_map<string, string> m_Resources;
};

NS_END