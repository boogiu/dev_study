#pragma once
#include "IService.h"
NS_BEGIN(Engine)
class ENGINE_DLL IEffectService abstract :
public IService
{
protected:
	virtual  ~IEffectService() DEFAULT;
public:
	virtual void Update(_float dt) PURE;
	virtual void Render(ID3D11DeviceContext* pContext) PURE;
	virtual void Spawn(class CEffectData* data, const EffectRequestPacket& desc) PURE;
	virtual void QueingSpriteEffect(const EffectSpriteDrawDesc& desc) PURE;
	virtual void Spawn_Preset(const EffectDataPreset& preset, const EffectRequestPacket& desc)PURE;
};
NS_END
