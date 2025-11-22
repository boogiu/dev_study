#pragma once
#include "Base.h"
NS_BEGIN(Engine)

class ENGINE_DLL CEffectInstance :
	public CBase
{
private:
	CEffectInstance();
	virtual ~CEffectInstance() override DEFAULT;

public:
	void Play(class CEffectData* data, const EffectRequestPacket& req);
	void Update(_float dt);
	void Render();

public:
	_bool IsAlive();

private:
	void Update_Transform();

private:
	class CEffectData* m_Data = { nullptr };                  // 어떤 이펙트인가? (설계도)
	EffectRequestPacket m_Request = {};
	_float m_fLifeTime = {};													  // 현재 진행 시간
	_float m_fDuration = { 0.f };											  // 총  시간
	_bool m_bAlive = false;                   

	vector<class CEmitter*> m_Emitters;             // 실제 뿌리는 분출기
	EffectTransform m_Transform = {};					//경량된 트랜스폼

	class CGameObject* m_Parent = nullptr;       
	string m_SocketBone;                   

public:
	static CEffectInstance* Create();
	void Free() override;
};
NS_END
