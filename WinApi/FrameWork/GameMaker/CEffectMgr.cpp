#include "pch.h"
#include "CEffectMgr.h"
CEffectMgr* CEffectMgr::m_pInstance = nullptr;

CEffectMgr::CEffectMgr()
{
}

CEffectMgr::~CEffectMgr()
{
	Release();
}

void CEffectMgr::Initialize()
{
}

void CEffectMgr::Update()
{
	for (int i = 0; i < int(MAX_EFFECT); i++)
	{
		for (CObject* effect : m_activeEffect[(EFFECT_TAG)i]) {
			if (effect->Is_Activate()) {
				effect->Update();
			}
		}
	}
}

void CEffectMgr::Late_Update()
{
	for (int i = 0; i < int(MAX_EFFECT); i++)
	{
		auto iter = m_activeEffect[(EFFECT_TAG)i].begin();
		for (; iter != m_activeEffect[(EFFECT_TAG)i].end(); ) {
			if (!(*iter)->Is_Activate()) {
				m_poolEffect[(EFFECT_TAG)i].push_back(*iter);
				iter = m_activeEffect[(EFFECT_TAG)i].erase(iter);
			}
			else {
				(*iter)->Late_Update();
				++iter;
			}
		}
	}
}

void CEffectMgr::PostRender(HDC _hDC)
{
	for (int i = 0; i < int(MAX_EFFECT); i++)
	{
		for (auto& effect : m_activeEffect[(EFFECT_TAG)i]) {
			if (effect->Is_Activate()) {
				if (static_cast<CBaseEffect*>(effect)->Get_Layer() == POST) {
					effect->Render(_hDC);
				}
			}
		}
	}
}

void CEffectMgr::Render(HDC _hDC)
{
	for (int i = 0; i < int(MAX_EFFECT); i++)
	{
		for (CObject* effect : m_activeEffect[(EFFECT_TAG)i]) {
			if (effect->Is_Activate()) {
				if (static_cast<CBaseEffect*>(effect)->Get_Layer() == AFTER) {
					effect->Render(_hDC);
				}
			}
		}
	}
}

void CEffectMgr::Release()
{
	for (int i = 0; i < (int)OBJ_TAG::MAX_OBJECT; ++i) {
		for (auto& obj : m_activeEffect[EFFECT_TAG(i)]) {
			Safe_Delete<CObject*>(obj);
		}
		for (auto& obj : m_poolEffect[EFFECT_TAG(i)]) {
			Safe_Delete<CObject*>(obj);
		}
	}
	m_activeEffect.clear();
	m_poolEffect.clear();
}
