#pragma once
#include <Windows.h>
#include <list>
#include "CTask.h"
#include "ThreadItem.h"



class CThreadPool
{
public:
	CThreadPool();
	CThreadPool(int nThreadCount);
	virtual ~CThreadPool();

	static DWORD WINAPI ThreadFunc(LPVOID lpParameter = NULL);

	void AddTask(CTask* task);
	list<ThreadItem*>* getThreadItemList() {return m_pThreadItemList;}
	list<CTask*>*	getTaskList(){return m_pTaskList;}

	void initThreadItem();
	void StopAllThread();

	int getTaskCount();
private:
	int m_iThreadCount;
	list<ThreadItem*>* m_pThreadItemList;
	list<CTask*>* m_pTaskList;
	CRITICAL_SECTION	m_csThreadQueue;
	CRITICAL_SECTION	m_csTaskQueue;
	HANDLE m_TaskEvent;
};