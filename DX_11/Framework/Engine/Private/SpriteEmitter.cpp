#include "SpriteEmitter.h"

CSpriteEmitter::CSpriteEmitter()
{
}

HRESULT CSpriteEmitter::Initialize(EmitterTemplate* pData)
{
	if (pData->eType != EmitterType::Sprite)
		return E_FAIL;

	SpriteEmitterData* pSpriteEmitterData = static_cast<SpriteEmitterData*>(pData);
	m_Data.eType = pSpriteEmitterData->eType;
	m_Data.frameCount = pSpriteEmitterData->frameCount;
	m_Data.frameTime = pSpriteEmitterData->frameTime;
	m_Data.lifetime = pSpriteEmitterData->lifetime;
	m_Data.loop = pSpriteEmitterData->loop;
	m_Data.textureName = pSpriteEmitterData->textureName;

	Safe_Delete(pData);

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

	m_CurrentFrame = static_cast<int>(m_Time / m_Data.frameTime);

	if (m_CurrentFrame >= m_Data.frameCount)
	{
		if (m_Data.loop)
			m_CurrentFrame %= m_Data.frameCount; // 다시 0부터
		else
			m_Alive = false; // 소멸
	}
}

void CSpriteEmitter::Render()
{
	if (!m_Alive) return;

	//		RenderingAPI->DrawSpriteFrame(
	//			m_Data.textureName,
	//			m_CurrentFrame,
	//			m_Data.frameCount,
	//			m_Transform.Get_WorldMatrix()
	//		);
}

CSpriteEmitter* CSpriteEmitter::Create(EmitterTemplate* pData)
{
	CSpriteEmitter* instance = new CSpriteEmitter();
	if (FAILED(instance->Initialize(pData))) {
		Safe_Release(instance);
	}

	return instance;
}

void CSpriteEmitter::Free()
{
	__super::Free();
}
