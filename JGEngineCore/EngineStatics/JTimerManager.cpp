#include"JTimerManager.h"

using namespace std::chrono;

static JMainTimer* Instance = nullptr;

//
// JTimerManager Class
//

JMainTimer* JMainTimer::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = new JMainTimer();
		atexit(Destroy);
	}
	return Instance;
}
void JMainTimer::Destroy()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}
void  JMainTimer::Start()
{
	m_bStop = false;
	m_TimerTime = Clock::now();
}
void  JMainTimer::Stop()
{
	m_bStop = true;
}
void  JMainTimer::Tick()
{
	Clock::time_point CurrentTime = Clock::now();


	// Ÿ�̸Ӱ� ���� ���� ���
	if (m_bStop)
	{
		// ���̽ð� : ����ð� - ���ð�(Ÿ�̸Ӱ� �������ִ� �ð�)
		Duration deltaTime = CurrentTime - m_TimerTime;

		m_TimerTime = CurrentTime;
		m_StopTime += deltaTime.count();
		m_DeltaTime = 0.0f;
		return;
	}


	// ���̽ð� : ����ð� - ���ð�(Ÿ�̸Ӱ� �������ִ� �ð�)
	Duration deltaTime = CurrentTime - m_TimerTime;

	m_TimerTime = CurrentTime;
	m_DeltaTime = deltaTime.count();



	// ��ü �帥 �ð� ���ϱ�
	m_ActiveTime += m_DeltaTime;
}
const float JMainTimer::GetDeltaTime()  { return m_DeltaTime; }
const float JMainTimer::GetActiveTime() { return m_ActiveTime; }
const float JMainTimer::GetStopTime()   { return m_StopTime; }
const float JMainTimer::GetTotalTime()  { return m_ActiveTime + m_StopTime; }

//
// JTimerEventManager Class
//
// Static Memeber Param Init
std::forward_list<JTimerHandle*> JTimerEventManager::m_flTimerHandles;
void JTimerEventManager::Tick(const float DeltaTime)
{
	// ���� �̺�Ʈ�� �߻��� Ÿ�̸� �ڵ� ����
	m_flTimerHandles.remove_if([](JTimerHandle* handle)->bool
	{
		if (handle->IsDestory())
		{
			return true;
		}
		return false;
	});

	// �̺�Ʈ�� ������Ʈ�Ѵ�.
	EventUpdate(DeltaTime);
}
void JTimerEventManager::CreateTimerEvent(JTimerHandle** handle, const TimerEvent& Event, EHandleType HandleType, const float Delay,
	const float GapTime, const size_t Count)
{
	// �ʱ�ȭ�� �ѹ��߰� ���� Ÿ�̸� �ڵ� �̺�Ʈ�� ������ �ʴ� �����̶��
	// �Լ��� �������´�.
	if ((*handle) != nullptr && (*handle)->m_bInit && !(*handle)->m_bEnd)
	{
#ifdef _DEBUG
		// ����� ���� ����ó��
#endif
		return;
	}
	// ó�� �ʱ�ȭ�� �ڵ��� ���� �����Ѵ�.
	if ((*handle) == nullptr)
	{
		(*handle) = new JTimerHandle;
		(*handle)->m_bInit = true;
	}
	// �� �ΰ��� ��ξƴ϶�� �ڵ� �̺�Ʈ�Լ��� ��γ����� �߰������Ҽ��ִ�.
	else
	{
		(*handle)->m_bEnd = false;
	}
	(*handle)->m_HandleDesc = {
		Event,
		HandleType,
		Delay,
		GapTime,
		Count };
	//
	m_flTimerHandles.push_front((*handle));
}
void JTimerEventManager::EventUpdate(const float DeltaTime)
{
	for (auto& iter : m_flTimerHandles)
	{
		// �̺�Ʈ�� ���� Ÿ�̸� �ڵ��� �����Ѵ�.
		if (iter->m_bEnd)
		{
			continue;
		}
		TimerHandleDesc& desc = iter->m_HandleDesc;
		switch (desc.HandleType)
		{
		case EHandleType::EDefault:
			if (desc.Delay <= 0)
			{
				// Delay�ʸ�ŭ ���� �ڵ��� �Լ� ����
				desc.Event();
				iter->m_bEnd = true;
			}
			desc.Delay -= DeltaTime;
			break;
		case EHandleType::ERepeat:
			if (desc.Count > 0 && desc.Delay <= 0 && iter->DummyTime <=0)
			{
				desc.Event();
				// �̺�Ʈ ���� ���� ī��Ʈ ���� / ������ ����
				desc.Count--;
				iter->DummyTime = desc.GapTime;
			}
			// GapTime ����
			if (desc.Delay <= 0)
			{
				iter->DummyTime -= DeltaTime;
			}
			// �ݺ�Ƚ���� ��ä���� ��� �̺�Ʈ ����
			if (desc.Count <= 0)
			{
				iter->m_bEnd = true;
			}
			desc.Delay -= DeltaTime;
			break;
		default:
			break;
		}

	}
}




//
// JTimerHandle Class
//
bool JTimerHandle::IsEnd()    { return m_bEnd; }
bool JTimerHandle::IsDestory(){ return m_bDestroy; }
void JTimerHandle::Destory()  { m_bDestroy = true; }

