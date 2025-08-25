#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class ENGINE_DLL CGameInstance :
	public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	explicit CGameInstance();
	virtual ~CGameInstance();

public:
	_bool Init_Engine(const ENGINE_DESC& engine);
	void Update_Engine(_float dt);
	void Release_Engine();

public:
	void Notify_LevelSet();
	_bool HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT Draw_Begin(_float4* pColor);
	HRESULT Draw();
	HRESULT Draw_End();

public:
	ID3D11Device* Get_Device() { return m_pDevice; };
	ID3D11DeviceContext* Get_Context() { return m_pDeviceContext; };

#pragma region Game_Managers
public:
	class IGraphicService* Get_GraphicDev() { return m_pGraphicDevice; }
	class ITimeService* Get_TimeMgr() { return m_pTimeManager; }
	class IInputService* Get_InputDev() { return m_pInputDevice; }
	class ISoundService* Get_SoundDev() { return m_pSoundDevice; }
	class ILevelService* Get_LevelMgr() { return m_pLevelManager; }
	class IProtoService* Get_PrototypeMgr() { return m_pPrototypeManager; }
	class IObjectService* Get_ObjectMgr() { return m_pObjectManager; }

private:
	class IGraphicService* m_pGraphicDevice = { nullptr };
	class ITimeService* m_pTimeManager = { nullptr };
	class IInputService* m_pInputDevice = { nullptr };
	class ISoundService* m_pSoundDevice = { nullptr };
	class ILevelService* m_pLevelManager = { nullptr };
	class IProtoService* m_pPrototypeManager = { nullptr };
	class IObjectService* m_pObjectManager = { nullptr };
#pragma endregion

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };

public:
	virtual void Free() override;
};

NS_END
