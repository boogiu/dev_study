#pragma once
#include "Level.h"

NS_BEGIN(Engine)
class CGameInstance;
class IProtoService;
class IObjectService;
NS_END

NS_BEGIN(Client)
class CGamePlayLevel :
	public CLevel
{
private:
	CGamePlayLevel(const string& LevelKey);
	virtual ~CGamePlayLevel() DEFAULT;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Awake() override;
	virtual void Update() override;
	virtual HRESULT Render()override;
	HRESULT Render(ID3D11DeviceContext* pContext)override;

public:
	static void PreLoad_Level();

private:
	CGameInstance* m_pGameInstance = { nullptr };
	IProtoService* m_pProtoManager = { nullptr };
	IObjectService* m_pObjectManager = { nullptr };

public:
	static CGamePlayLevel* Create(const string& LevelKey);
	virtual void Free() override;

};

NS_END