#pragma once

typedef enum EventDef_tag
{
	EM_EVENT_TASK = 0,
	EM_EVENT_STOP = 1,
	EM_EVENT_BUTT
}EVENTDEF;

class CThreadPool;
typedef struct ThreadItem
{
	HANDLE hThreadHandle;
	DWORD  dwThreadID;
	BOOL   bThreadFlag;
	CThreadPool* pThreadPool;
	HANDLE hWaitEvents[EM_EVENT_BUTT];
	ThreadItem()
	{
		hThreadHandle = NULL;
		dwThreadID = 0;
		bThreadFlag = FALSE;
		pThreadPool = NULL;
		hWaitEvents[EM_EVENT_STOP] = CreateEvent(NULL,FALSE,FALSE,NULL);
	}

	~ThreadItem()
	{
		if(hWaitEvents[EM_EVENT_STOP] != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hWaitEvents[EM_EVENT_STOP]);
		}
	}
	void addTaskEvent(HANDLE taskEvent)
	{
		if(taskEvent != INVALID_HANDLE_VALUE)
		{
			hWaitEvents[EM_EVENT_TASK] = taskEvent;
		}
	}
}ThreadItem;