#pragma once
#include"JGCommand.h"


/*
EnumClass : EKeyState 
@e Down : Ű�� ���ȴ�.!
@e Up   : Ű�� �H��!!
@e None : �����!! */
enum class ENGINE_EXPORT EKeyState
{
	Down,
	Up,
	None
};
/*
Class : JGKeyCommand 
@m EKeyState m_KeyState : Ű ����
@m EKeyState m_EventTriggerKeyState : �̺�Ʈ�� �߻���ų Ű ����..
@m function<void()> m_BindKeyDownEvent : Ű�� �������� �̺�Ʈ
@m function<void()> m_BIndKeyUpEvent : Ű �H���� �̺�Ʈ
@m bool m_bKeyVariance : Ű���¿� ��ȭ�� �־����� ����
@m bool m_PrevPressed  : ���� Ű�������� 
@m bool m_bIsBindKeyDownEvent : Ű ���� �̺�Ʈ�� ���ε� ����� ���� 
@m bool m_bIsBindKeyUpEvent : Ű �E �̺�Ʈ�� ���ε� ����� ���� */
class ENGINE_EXPORT JGKeyCommand : public JGCommand
{
private:
	EKeyState m_KeyState;
	std::function<void()> m_BindKeyDownEvent;
	std::function<void()> m_BIndKeyUpEvent;
	bool m_bKeyVariance = false;
	bool m_PrevPressed  = false;
	bool m_bIsBindKeyDownEvent = false;
	bool m_bIsBindKeyUpEvent = false;
public:
	JGKeyCommand(const JGKeyCommand& copy) = delete;
	JGKeyCommand();
	virtual ~JGKeyCommand();
	virtual void Tick() override;

	/*
	Exp : JGKeyCommand�� Ű���¿� �̺�Ʈ(�Լ�)�� ���ε��Ѵ�. (�Լ��� ������ = void() �����̴�.)
	@param : const EKeyState KeyState : ���ε��� Ʈ���ſ� Ű����
	@param : const function<void()>& Event : void() �� �Լ� (Ŭ������������ϰ�쿡�� std::bind �Լ��� �̿����� ) */
	void BindKeyCommand(const EKeyState KeyState, const std::function<void()>& Event);
};