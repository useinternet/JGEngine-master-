#include"JThreadManager.h"
using namespace std;
const unsigned short JThreadManager::WorkerCount = 2;
JThreadManager::ThreadSafe_Que<JThreadManager::Business> JThreadManager::m_Businesses;
std::vector<std::thread> JThreadManager::m_vWorker;
std::atomic<bool>        JThreadManager::m_IsWorking = true;
static JThreadManager* Instance = nullptr;


// ThreadSafe_Que
template< typename Task >
void  JThreadManager::ThreadSafe_Que<Task>::push(const Task& task)
{
	lock_guard<mutex> lock(m_Mutex);
	m_TaskQue.push(move(task));
	m_cdVar.notify_one();
}
template< typename Task >
bool JThreadManager::ThreadSafe_Que<Task>::pop(Task& task)
{
	lock_guard<mutex> lock(m_Mutex);
	if (m_TaskQue.empty())
	{
		return false;
	}
	task = move(m_TaskQue.front());
	m_TaskQue.pop();
	return true;
}


// JThreadManager
void JThreadManager::Init()
{
	// ������ ����
	for (unsigned short i = 0; i < WorkerCount; ++i)
	{
		m_vWorker.push_back(thread(JThreadManager::Work));
	}
}

void JThreadManager::Work()
{
	while (m_IsWorking)
	{
		Business task;
		if (m_Businesses.pop(task))
		{
			task();
		}
		else
		{
			std::this_thread::yield();
		}
	}
}

JThreadManager * JThreadManager::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = new JThreadManager;
		Init();
		atexit(Destroy);
	}
	return Instance;
}
void JThreadManager::Destroy()
{
	if (Instance)
	{
		// �۾��� ���߰� �����尡 ��� �Ϸ�ɵ����� ��ٸ���.
		m_IsWorking = false;

		for (unsigned short i = 0; i < WorkerCount; ++i)
		{
			if (m_vWorker[i].joinable())
			{
				m_vWorker[i].join();
			}
		}
		// �ν��Ͻ� ����
		delete Instance;
		Instance = nullptr;
	}
}
void JThreadManager::AddTask(const Business& Task)
{
	// �۾� �߰�
	m_Businesses.push(Task);
}





