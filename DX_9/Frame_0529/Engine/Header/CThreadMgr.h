#pragma once
#include "CBase.h"
#include "JobQueue.h"  

BEGIN(Engine)

class JobQueue;

typedef struct tagStreamWorker {
    JobQueue* worker;
    HANDLE handle;
    bool active = true;

    tagStreamWorker() {
        worker = JobQueue::Create();
    };

    ~tagStreamWorker() {
        if (worker) {
            Safe_Release(worker);
        }
    }

}StreamWorker;

class ENGINE_DLL CThreadMgr :
    public CBase
{
    DECLARE_SINGLETON(CThreadMgr)

private:
    explicit CThreadMgr();
   virtual ~CThreadMgr();

public:
    HRESULT Ready_Thread();
    void PushStream(const wstring& key, function<void()>job);
    void StopAll();

private:
    static _uint __stdcall ThreadProc(void* pArg);
    static _uint __stdcall  ThreadWorker(void* pArg);

private:
    mutable CRITICAL_SECTION m_Crt; //const 함수에서 내부 데이터를 읽을 때 lock을 걸고 읽으려면 mutable이어야 함
    unordered_map<wstring, StreamWorker* > m_StreamingThread;

private:
    void Free();
};

END
