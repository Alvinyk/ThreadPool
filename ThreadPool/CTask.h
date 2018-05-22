#pragma once
#include <iostream>
using namespace std;
class CTask
{
protected:
	string m_strTaskName;
	int m_Data;
public:
	CTask(){}
	CTask(string taskname)
	{
		m_strTaskName = taskname;
		m_Data = 0;
	}

	void setData(int data){m_Data = data;}
	int getData(){return m_Data;}
	virtual int Run()=0;
	virtual ~CTask(){}
};