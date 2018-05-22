#include "CThreadPool.h"

CThreadPool::CThreadPool()
{
	CThreadPool(5);
}

CThreadPool::CThreadPool(int nThreadCount)
{
	m_iThreadCount = nThreadCount;
	InitializeCriticalSection(&m_csThreadQueue);
	InitializeCriticalSection(&m_csTaskQueue);

	m_pThreadItemList = new list<ThreadItem*>;
	m_pTaskList = new list<CTask*>;
	initThreadItem();
}

void CThreadPool::initThreadItem()
{
	m_TaskEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	for (int i = 0;i<m_iThreadCount;i++)
	{
		ThreadItem* pItem = new ThreadItem();
		if(pItem)
		{
			pItem->pThreadPool = this;
			pItem->bThreadFlag = TRUE;
			pItem->addTaskEvent(m_TaskEvent);
			pItem->hThreadHandle = CreateThread(NULL,0,ThreadFunc,(LPVOID)pItem,0,&pItem->dwThreadID);
			
			EnterCriticalSection(&m_csThreadQueue);
			m_pThreadItemList->push_back(pItem);
			LeaveCriticalSection(&m_csThreadQueue);
		}
	}
}

CThreadPool::~CThreadPool()
{
	StopAllThread();
	if(m_TaskEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_TaskEvent);
	}
	DeleteCriticalSection(&m_csThreadQueue);
	DeleteCriticalSection(&m_csTaskQueue);
	m_pThreadItemList->clear();
	m_pThreadItemList->clear();
	delete m_pTaskList;
	delete m_pThreadItemList;

}


DWORD WINAPI CThreadPool::ThreadFunc(LPVOID lpParameter /* = NULL */)
{
	ThreadItem* pItem = (ThreadItem*)lpParameter;
	if(pItem)
	{
		while(pItem->bThreadFlag)
		{
			CTask* pTask = NULL;
			EnterCriticalSection(&pItem->pThreadPool->m_csTaskQueue);
			list<CTask*>* pTaskList = pItem->pThreadPool->getTaskList();
			if(pTaskList->empty())
			{
				LeaveCriticalSection(&pItem->pThreadPool->m_csTaskQueue);
				printf("暂时无任务需要处理，线程id %d 进入休息状态\n",pItem->dwThreadID);
				DWORD result = WaitForMultipleObjects(EM_EVENT_BUTT,pItem->hWaitEvents,FALSE,INFINITE);
				continue;
			}
			pTask = pTaskList->front();
			pTaskList->pop_front();
			LeaveCriticalSection(&pItem->pThreadPool->m_csTaskQueue);
			if(pTask)
			{
				printf("线程id %d 处理第 %d 个任务！ \n",pItem->dwThreadID,pTask->getData());
				pTask->Run();
				delete pTask;
				continue;
			}
		}
	}

	return 0;
}

void CThreadPool::StopAllThread()
{
	EnterCriticalSection(&m_csThreadQueue);
	while(!m_pThreadItemList->empty())
	{
		ThreadItem* pThread = m_pThreadItemList->front();
		if(pThread)
		{
			printf("结束线程 %d \n",pThread->dwThreadID);
			pThread->bThreadFlag = FALSE;
			SetEvent(pThread->hWaitEvents[EM_EVENT_STOP]);
			WaitForSingleObject(pThread->hThreadHandle,INFINITE);
			m_pThreadItemList->pop_front();
			delete(pThread);
		}
	}
	LeaveCriticalSection(&m_csThreadQueue);
}

void CThreadPool::AddTask(CTask* task)
{
	if(m_pTaskList)
	{
		EnterCriticalSection(&m_csTaskQueue);
		m_pTaskList->push_back(task);
		LeaveCriticalSection(&m_csTaskQueue);
		SetEvent(m_TaskEvent);
	}
}

int CThreadPool::getTaskCount()
{
	int count = 0;
	EnterCriticalSection(&m_csTaskQueue);
	count = m_pTaskList->size();
	LeaveCriticalSection(&m_csTaskQueue);
	return count;
}