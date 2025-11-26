#include "SpriteEmitter.h"
#include "GameInstance.h"

CSpriteEmitter::CSpriteEmitter()
{
}

HRESULT CSpriteEmitter::Initialize(SpriteEmitterData Data)
{
	m_Data = Data;
	m_Time = 0.f;
	m_CurrentFrame = 0.f;
	m_Alive = true;

	return S_OK;
}

void CSpriteEmitter::Update(float dt, const EffectTransform& transform)
{
	if (!m_Alive) return;

	m_Transform = transform;
	m_Time += dt;

	// Index 계산
	_uint maxFrame = m_Data.totalFrames;
	m_CurrentFrame = static_cast<_uint>(m_Time / m_Data.frameTime); /*지금 프레임 타임*/

	if (m_CurrentFrame >= maxFrame)
	{
		if (m_Data.loop)
			m_CurrentFrame %= maxFrame;
		else
			m_Alive = false;
	}
}


void CSpriteEmitter::Render()
{
	if (!m_Alive) return;
	auto pFxSystem = CGameInstance::GetInstance()->Get_EffectSystem();
	pFxSystem->QueingSpriteEffect({
		 m_CurrentFrame,
		 m_Data.cols,
		 m_Data.rows,
		_float4{1.f,1.f,1.f,1.f},
		m_Transform.WorldMatrix,
		m_Data.textureName,
		 m_Data.passName,
		});

}

CSpriteEmitter* CSpriteEmitter::Create(SpriteEmitterData Data)
{
	CSpriteEmitter* instance = new CSpriteEmitter;

	if (FAILED(instance->Initialize(Data))) {
		Safe_Release(instance);
	}

	return instance;
}

void CSpriteEmitter::Free()
{
	__super::Free();
}
