#pragma once
#include"Engine.h"


class JTimerHandle;
typedef std::function<void()>        TimerEvent;
typedef std::chrono::system_clock    Clock;
typedef std::chrono::duration<float> Duration;

/*
Name : EHandleType
@m ERepeat  : ������ ī��Ʈ��ŭ �ݺ��Ѵ�.
@m EDefault : �ѹ� �����Ѵ�.
*/
enum class ENGINE_EXPORT EHandleType
{
	ERepeat, EDefault
};

/*
struct : TimerHandleDesc
@m TimerEvent   Event      : ������ �̺�Ʈ(void() �� �Լ�)
@m EHandleType HandleType  : ���� Ÿ��
@m float        Delay      : ó�� �����ҋ� ���� �ð�
@m float        GapTime    : �ݺ� �����ҋ� ���� �ð�
@m size_t       Count      : �ݺ� Ƚ��
*/
typedef struct ENGINE_EXPORT TimerHandleDesc
{
	TimerEvent   Event;
	EHandleType HandleType;

	float Delay = 0.0f;
	float GapTime = 0.0f;
	size_t Count = 0;
}TimerHandleDesc;


/*
Class : JMainTimer
@m Clock::time_point m_TimerTime  : �ð� ���� ����
@m float             m_DeltaTime  : 1 �����Ӵ� �ð�
@m float             m_ActiveTime : Ÿ�̸Ӱ� Ȱ��ȭ�Ǿ��־��� �ð�
@m float             m_StopTime   : Ÿ�̸Ӱ� �����־��� �ð�
@m bool              m_bStop      : ���� ����
*/
class ENGINE_EXPORT JMainTimer
{
private:
	JMainTimer() {}
	JMainTimer(const JMainTimer& copy) {};
	~JMainTimer() {}
private:
	Clock::time_point   m_TimerTime;

	float m_DeltaTime   = 0.0f;
	float m_ActiveTime  = 0.0f;
	float m_StopTime    = 0.0f;
	bool  m_bStop       = false;

public:
	/*
	Exp : JMainTimer�� �ν��Ͻ��� ����Ѵ�. */
	static JMainTimer* GetInstance();
	/*
	Exp : JMainTimer�� �ν��Ͻ��� �ı��Ѵ�. */
	static void Destroy();
	/*
	Exp : Ÿ�̸� ���� */
	void  Start();
	/*
	Exp : Ÿ�̸� ���� */
	void  Stop();
	/*
	Exp : 1 ������ ( 1 ������ �ֱ�� Ÿ�̸� ������Ʈ ) */
	void  Tick();

	/*
	Exp : 1 �����Ӵ� ���� �ð�  */
	const float GetDeltaTime();
	/*
	Exp : ���� Ÿ�̸Ӱ� Ȱ���� �ð�*/
	const float GetActiveTime();
	/*
	Exp : ���� Ÿ�̸Ӱ� �ح��� �ð�*/
	const float GetStopTime();
	/*
	Exp : �� �ð� */
	const float GetTotalTime();


};

/*
Static Class : JTimerEventManager
@m std::forward_list<JTimerHandle*> m_flTimerHandles : Ÿ�̸� �ڵ� �迭 ����Ʈ
*/
class ENGINE_EXPORT JTimerEventManager
{
private:
	friend JMainTimer;
	static std::forward_list<JTimerHandle*> m_flTimerHandles;
public:
	/*
	Exp : 1�����Ӹ��� ȣ��Ǵ� �Լ� 
	@param const float DeltaTime : JMainTimer�� 1�����Ӵ� �ð� */
	static void Tick(const float DeltaTime);
	/*
	Exp : Ÿ�̸� �̺�Ʈ�� �����Ѵ�.
	@param JTimerHandle** handle            : Ÿ�̸� �̺�Ʈ ������ �������ִ� HandleŬ���� �ּ�
	@param const          TimerEvent& Event : void()�� �Լ� �̺�Ʈ
	@param EHandleType    HandleType        : �ڵ� Ÿ��( �ѹ� ȣ��, �ݺ� ȣ�� ���..)
	@param const float    Delay             : ���� ���� ������ �ð�
	@param const float    GapTime           : �̺�Ʈ �ݺ� ȣ�� ���� ������ �ð�
	@param const size_t   Count             : �̺�Ʈ �ݺ� Ƚ��(-1�̸� ���� �ݺ�)
	*/
	static void CreateTimerEvent(JTimerHandle** handle,const TimerEvent& Event, EHandleType HandleType,
		const float Delay, const float GapTime, const size_t Count);
private:
	static void EventUpdate(const float DeltaTime);
	static void Destroy();
};

/*
Class : JTimerHandle
@m 	TimerHandleDesc m_HandleDesc : Ÿ�̸� �ڵ� ���� ���
@m	bool  m_bInit                : �ʱ�ȭ ����
@m	bool  m_bEnd                 : �̺�Ʈ ���� ����
@m	bool  m_bDestroy             : Ÿ�̸� �ڵ� �ı� ����
*/
class ENGINE_EXPORT JTimerHandle
{
private:
	friend class JTimerEventManager;
private:
	JTimerHandle() {}
	~JTimerHandle() { Destory(); }
	TimerHandleDesc m_HandleDesc;
	bool  m_bInit    = false;
	bool  m_bEnd     = false;
	bool  m_bDestroy = false;

	// ������ ����� ���� ���� ����
	float DummyTime = 0.0f;
public:
	/*
	Exp : �̺�Ʈ ���� ����*/
	bool IsEnd();
	/*
	Exp : Ÿ�̸� �ڵ� �ı� ����*/
	bool IsDestory();
	/*
	Exp : Ÿ�̸� �ڵ� �ı�*/
	void Destory();
};
