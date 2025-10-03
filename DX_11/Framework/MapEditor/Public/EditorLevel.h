#pragma once
#include "Level.h"
NS_BEGIN(Engine)
	class CGameInstance;
NS_END

NS_BEGIN(MapEditor)
class CEditorLevel final :
    public CLevel
{
private:
	CEditorLevel(const string& LevelKey);
	virtual ~CEditorLevel() DEFAULT;

public:
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual HRESULT Render()override;

public:
	static void PreLoad_Level();

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CEditorLevel* Create(const string& LevelKey);
	virtual void Free() override;
};
NS_END
