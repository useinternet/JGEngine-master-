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
@m std::function<void()> m_BindEvent : ���ε��� �̺�Ʈ
@m bool m_bKeyVariance : Ű���¿� ��ȭ�� �־����� ����
@m bool m_PrevPressed  : ���� Ű�������� */
class ENGINE_EXPORT JGKeyCommand : public JGCommand
{
private:
	EKeyState m_KeyState;
	EKeyState m_EventTriggerKeyState;
	std::function<void()> m_BindEvent;
	bool m_bKeyVariance = false;
	bool m_PrevPressed  = false;
public:
	JGKeyCommand(const JGKeyCommand& copy) = delete;
	JGKeyCommand();
	virtual ~JGKeyCommand();
	virtual void Tick(const float DeltaTime) override;

	/*
	Exp : JGKeyCommand�� Ű���¿� �̺�Ʈ(�Լ�)�� ���ε��Ѵ�. (�Լ��� ������ = void() �����̴�.)
	@param : const EKeyState KeyState : ���ε��� Ʈ���ſ� Ű����
	@param : const function<void()>& Event : void() �� �Լ� (Ŭ������������ϰ�쿡�� std::bind �Լ��� �̿����� ) */
	void BindKeyCommand(const EKeyState KeyState, const std::function<void()>& Event);
};