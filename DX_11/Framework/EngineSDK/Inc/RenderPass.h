#pragma once
#include "Engine_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)

class RenderPass abstract : public CBase {
protected:
	RenderPass(class CRenderSystem* pRenderSystem);
	virtual ~RenderPass() DEFAULT;
public:
	virtual void Execute(ID3D11DeviceContext* pContext) PURE;
protected:
	class CRenderSystem* m_pRenderSystem = { nullptr };
	class CShader* pCurShader = { nullptr };

public:
	virtual void Free();
};

#pragma region OPAQUE_PASS
class OpaquePass final : public RenderPass {
private:
	OpaquePass(class CRenderSystem* pRenderSystem) :RenderPass{ pRenderSystem } {};
	virtual ~OpaquePass() DEFAULT;
public:
	void Execute(ID3D11DeviceContext* pContext) override;
	void Submit(OPAQUE_PACKET packet);
private:
	vector<OPAQUE_PACKET> m_Packets;
public:
	static OpaquePass* Create(class CRenderSystem* pRenderSystem) { return new OpaquePass(pRenderSystem); }
	virtual void Free() override {__super::Free(); m_Packets.clear();};
};
#pragma endregion

#pragma region UI_PASS
class UIPass final : public RenderPass {
private:
	UIPass(class CRenderSystem* pRenderSystem) :RenderPass{ pRenderSystem } {};
	virtual ~UIPass() DEFAULT;
public:
	void Execute(ID3D11DeviceContext* pContext) override;
	void Submit(UI_PACKET packet) { m_Packets.push_back(packet); };
private:
	vector<UI_PACKET> m_Packets;
public:
	static UIPass* Create(class CRenderSystem* pRenderSystem) { return new UIPass(pRenderSystem); }
	virtual void Free() override { __super::Free(); m_Packets.clear(); };
};
#pragma endregion

NS_END