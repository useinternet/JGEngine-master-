#pragma once
#include"common.h"
#include<thread>
#include<functional>
#include<vector>
#include<atomic>
#include<mutex>
#include<queue>
#include<condition_variable>
/*
Static Class : JThreadManager
@Class ThreadSafe_Que : ���� �����忡 ȣȯ�Ǵ� queue Ŭ����

@m const unsigned short     WorkerCount  : �����ϴ� ������ ����
@m ThreadSafe_Que<Business> m_Businesses : �۾� �Լ� ť
@m std::vector<std::thread> m_vWorker    : ���� ������ �迭
@m std::atomic<bool>        m_IsWorking  : Ŭ���� ���� ����
*/
class  JThreadManager
{
private:
	/*
	Class : ThreadSafe_Que
	@m  std::queue<Task>        m_TaskQue : �Լ� ť
	@m	std::mutex              m_Mutex   : ���ؽ�(��);
	@m	std::condition_variable m_cdVar   : ���� ����
	*/
	template< typename Task >
	class ThreadSafe_Que
	{
	private:
		friend JThreadManager;
	private:
		std::queue<Task>        m_TaskQue;
		std::mutex              m_Mutex;
		std::condition_variable m_cdVar;
	public:
		void  push(const Task& task);
		bool  pop(Task& task);
	};
	typedef std::function<void()> Business;
private:
	static const unsigned short WorkerCount;
private:
	static ThreadSafe_Que<Business> m_Businesses;
	static std::vector<std::thread> m_vWorker;
	static std::atomic<bool>        m_IsWorking;

private:
	JThreadManager() {}
	~JThreadManager() {}
	static void Init();

	static void Work();
public:
	/*
	Exp : JThreadManager�� �ν��Ͻ��� ����Ѵ�. */
	static JThreadManager* GetInstance();
	/*
	Exp : JThreadManager�� �ν��Ͻ��� �ı��Ѵ�. */
	static void Destroy();
	/*
	Exp : �۾� �Լ� ť�� �Լ��� �߰��Ѵ�. 
	@param const Business& Task : �߰��� �۾� �Լ� */
	static void AddTask(const Business& Task);
};

