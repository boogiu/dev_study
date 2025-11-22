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
	virtual void Submit_Shadow(const OPAQUE_PACKET& packet) override { m_pShadowPass->Submit(packet); };
	virtual void Submit_Shadow(const INSTANCE_PACKET& packet) override { m_pShadowPass->SubmitInstance(packet); };
	virtual void Submit_Instance(const INSTANCE_PACKET& packet) override { m_pInstancePass->Submit(packet); };
	virtual void Submit_Priority(const OPAQUE_PACKET& packet) override { m_pPriorityPass->Submit(packet); };
	virtual void Submit_UI(const SPRITE_PACKET& packet) override {m_pUIPass->Submit(packet);};
	virtual void Submit_Debug(const DEBUG_PACKET& packet) override { m_pDebugPass->Submit(packet); };
	virtual HRESULT Add_Palette(const string& ConstantName, class CTexture* pTexture) override;

public:
	HRESULT Render_LightAcc();
	HRESULT Render_Combined();
#ifdef _USING_GUI
	void Render_GUI();
#endif // _USING_GUI

#pragma region RenderTarget
public:
	virtual HRESULT Create_RenderTarget(const RenderTargetDesc& desc) override;
	virtual void Add_RenderCommand(const RENDER_COMMAND& command) override;
	virtual void DrawTo(const string& targetKey, function<void(ID3D11DeviceContext*)> drawCall) override;
	virtual ID3D11ShaderResourceView* Get_TargetSRV(const string strTag) override;

private:
	HRESULT Ready_GBuffer();
	void Process_RenderCommand();
#pragma endregion

public:
	virtual HRESULT Get_InputLayout(class CModel* pModel, class CShader* pShader, _uint DrawIndex, 
		const string& passConstant,  ID3D11InputLayout** ppInputLayout) override;
	virtual  HRESULT Get_BufferInputLayout(class CVIBuffer* pBuffer, class CShader* pShader,
		const string& passConstant,  ID3D11InputLayout** ppInputLayout)override;
	class CPipeLine* Get_Pipeline() { return m_pPipeLine; }

private:
	void Render_Shadow();
	HRESULT Change_Viewport(_uint iWidth, _uint iHeight);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = {nullptr};
	unordered_map<string, ID3D11InputLayout*> m_InputLayouts;

	/*PipeLine*/
	class CPipeLine* m_pPipeLine = { nullptr };

	/*RendetTarget*/
	class CTarget_Manager* m_pTargetManager = { nullptr };
	class CVIBuffer* m_pVIBuffer = { nullptr };
	class CShader* m_pShader = { nullptr };
	_float4x4				m_WorldMatrix;

	/*Pass*/
	PriorityPass* m_pPriorityPass = { nullptr};
	OpaquePass* m_pOpaquePass = { nullptr};
	ShadowPass* m_pShadowPass = { nullptr};
	InstancePass* m_pInstancePass = { nullptr};
	UIPass* m_pUIPass = { nullptr };
	DebugPass* m_pDebugPass = { nullptr };

	vector<RENDER_COMMAND> m_RenderCommands;
public:
	static CRenderSystem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END