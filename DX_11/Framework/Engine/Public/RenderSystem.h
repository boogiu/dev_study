#pragma once
#include "IRenderService.h"
#include "RenderPass.h"

NS_BEGIN(Engine)

class CRenderSystem final : public IRenderService
{
private:
	CRenderSystem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderSystem();
	HRESULT Initialize();

public: 
	virtual HRESULT Render() override;
	virtual void Submit_Opaque(const OPAQUE_PACKET& packet) override { m_pOpaquePass->Submit(packet); };
	virtual void Submit_UI(const UI_PACKET& packet) override {m_pUIPass->Submit(packet);};

public:
	HRESULT Get_InputLayout(class CModel* pModel, class CShader* pShader, const string& passConstant, ID3D11InputLayout** ppInputLayout);
	class CPipeLine* Get_Pipeline() { return m_pPipeLine; }

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = {nullptr};
	class CPipeLine* m_pPipeLine = { nullptr };

	unordered_map<string, ID3D11InputLayout*> m_InputLayouts;
	OpaquePass* m_pOpaquePass = { nullptr};
	UIPass* m_pUIPass = { nullptr };

public:
	static CRenderSystem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END