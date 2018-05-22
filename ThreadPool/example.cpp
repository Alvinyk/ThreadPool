#include "CThreadPool.h"

class CMyTask : public CTask
{
public:
	CMyTask(){}
	int Run()
	{
		printf("任务 %d 正在运行。。。\n",m_Data);
		Sleep(100);
		return 0;
	}
};


void main()
{
	CThreadPool threadPool(10);
	
	for(int i = 0 ;i<1000;i++)
	{
		CTask* task = new CMyTask();;
		task->setData(i);
		threadPool.AddTask(task);
	}
	int count = 0;
	while(1)
	{
		//printf("there are still %d tasks need to handle\n",threadPool.getTaskCount());
		if(threadPool.getTaskCount() == 0)
		{
			if(count++ < 10)
			{
				Sleep(1000);
				for(int i = 0 ;i<1000;i++)
				{
					CTask* task = new CMyTask();;
					task->setData(i);
					threadPool.AddTask(task);
				}
				continue;
			}
			threadPool.StopAllThread();
			printf("now i will exit from main\n");
			break;
		}
	}

	getchar();
}