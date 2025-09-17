#pragma once
#include "Level.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Loader)
class CModelLevel final:
	public CLevel
{
private:
	CModelLevel(const string& LevelKey);
	virtual ~CModelLevel() DEFAULT;

public:
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual HRESULT Render()override;

public:
	static void PreLoad_Level();

private:
	CGameInstance* m_pGameInstance = { nullptr };
public:
	static CModelLevel* Create(const string& LevelKey);
	virtual void Free() override;
};
NS_END

