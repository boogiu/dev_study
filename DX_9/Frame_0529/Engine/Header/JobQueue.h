#pragma once
#include "CBase.h"
BEGIN(Engine)

class ENGINE_DLL JobQueue :
    public CBase
{
private:
    explicit JobQueue();
    virtual ~JobQueue();
public:
    static JobQueue* Create();
    void Push(function<void()> job);
    _bool Pop(function<void()>& job);
private:
    HRESULT Ready_Queue();

private:
    CRITICAL_SECTION m_Crt;
    queue<function<void()>> m_Queue;
private:
    void Free();
};

END