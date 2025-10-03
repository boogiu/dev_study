#pragma once
#include "Base.h"
#include "Editor_Defines.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(MapEditor)
class CEditorMain : public CBase
{
private:
	CEditorMain();
	virtual ~CEditorMain();

public:
	HRESULT Initialize();
	void Update(_float dt);
	HRESULT Render();

public:
	void Set_Levels();

private:
	CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };

	class CEditorSystem* m_pSystem = {nullptr};

public:
	static CEditorMain* Create();
	virtual void Free() override;
};

NS_END