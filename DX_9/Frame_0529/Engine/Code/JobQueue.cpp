#include "Engine_Define.h"
#include "JobQueue.h"

JobQueue::JobQueue()
{
}

JobQueue::~JobQueue()
{
}

JobQueue* JobQueue::Create()
{
	JobQueue* instance = new JobQueue;

	if (FAILED(instance->Ready_Queue())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

void JobQueue::Push(function<void()> job)
{
	EnterCriticalSection(&m_Crt); //메인쓰레드가 푸시하는 동안 워커 쓰레드가 팝하면 오류나니까
	m_Queue.push(std::move(job));
	LeaveCriticalSection(&m_Crt);
}

_bool JobQueue::Pop(function<void()>& job)
{
	EnterCriticalSection(&m_Crt);

	if (m_Queue.empty())
	{
		LeaveCriticalSection(&m_Crt);
		return false;
	}
	else
	{
		job = move(m_Queue.front());
		m_Queue.pop();
		LeaveCriticalSection(&m_Crt);
		return true;
	}
}

HRESULT JobQueue::Ready_Queue()
{
	InitializeCriticalSection(&m_Crt);
	return S_OK;
}

void JobQueue::Free()
{
	DeleteCriticalSection(&m_Crt);
}
