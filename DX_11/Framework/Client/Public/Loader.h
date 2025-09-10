#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)
class CLoader final :  public CBase
{
private:
    CLoader();
    virtual ~CLoader();

public:
    HRESULT Initialize(const string& nextLV);
    HRESULT Loading();
    static  _uint WINAPI  LoadingThread(void* pArg);
    _bool isFinished() const {return m_isFinished; }

private:
    void Set_Finished() { m_isFinished = true; };

private:
    _bool						m_isFinished = { false };
    string						m_sNextLevel;
    HANDLE						m_hThread = {};
    CRITICAL_SECTION			m_CriticalSection = {};

public:
    static CLoader* Create(const string& nextLV);
    virtual void Free() override;

};

NS_END