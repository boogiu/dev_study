#include "Engine_Define.h"
#include "CThreadMgr.h"
#include "JobQueue.h"

IMPLEMENT_SINGLETON	(CThreadMgr)

CThreadMgr::CThreadMgr()
{
}

CThreadMgr::~CThreadMgr()
{
	Free();
}

HRESULT CThreadMgr::Ready_Thread()
{
	InitializeCriticalSection(&m_Crt);

    StreamWorker* FileLoader = new StreamWorker; 
    FileLoader->handle = (HANDLE)_beginthreadex(nullptr, 0, ThreadWorker, FileLoader, 0, nullptr);


    StreamWorker* CollisionChecker = new StreamWorker;
    CollisionChecker->handle = (HANDLE)_beginthreadex(nullptr, 0, ThreadWorker, CollisionChecker, 0, nullptr);

    m_StreamingThread.insert({ L"파일 로더",FileLoader });
    m_StreamingThread.insert({ L"충돌 체커",CollisionChecker });

	return S_OK;
}

void CThreadMgr::PushStream(const wstring& key, function<void()>job)
{
    EnterCriticalSection(&m_Crt);

    auto it = m_StreamingThread.find(key);

    if (it != m_StreamingThread.end()) {
        it->second->worker->Push(job);
    }

    LeaveCriticalSection(&m_Crt);
}

_uint  __stdcall CThreadMgr::ThreadProc(void* pArg)
{
	return 0;
}

_uint __stdcall CThreadMgr::ThreadWorker(void* pArg)
{
    StreamWorker* worker = static_cast<StreamWorker*>(pArg);

    while (worker->active) {
        function<void()> job;

        if (worker->worker->Pop(job)) {
            job();  // 작업 실행
        }
        else {
            Sleep(1); // idle 시 휴식
        }
    }

    _endthreadex(0);
    return 0;
}

void CThreadMgr::Free()
{
    EnterCriticalSection(&m_Crt);

    // 1. 모든 워커에게 종료 플래그 설정
    for (auto& pair : m_StreamingThread)
    {
        pair.second->active = false;
    }

    // 2. 모든 워커 핸들을 배열에 저장
    std::vector<HANDLE> handles;
    for (auto& pair : m_StreamingThread)
    {
        handles.push_back(pair.second->handle);
    }

    // 3. 모든 스레드 종료 대기
    if (!handles.empty())
    {
        WaitForMultipleObjects(
            static_cast<DWORD>(handles.size()),
            handles.data(),
            TRUE, // 모든 스레드가 끝날 때까지 기다림
            INFINITE);
    }

    // 4. 핸들 Close
    for (auto& handle : handles)
        CloseHandle(handle);

    // 5. JobQueue 삭제 등 자원 해제
    for (auto& pair : m_StreamingThread)
    {
        delete pair.second; //안에서 세이프 릴리즈 됨
    }

    m_StreamingThread.clear();
    // 6. 크리티컬 섹션 해제
    LeaveCriticalSection(&m_Crt);
    DeleteCriticalSection(&m_Crt);
}
