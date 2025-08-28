#pragma once
#include "Level.h"

NS_BEGIN(Engine)
    class CGameInstance;
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
        virtual void Update() override;
        virtual HRESULT Render()override;

    private:
        CGameInstance* m_pGameInstance = { nullptr };
    public:
        static CGamePlayLevel* Create(const string& LevelKey);
        virtual void Free() override;

};

NS_END