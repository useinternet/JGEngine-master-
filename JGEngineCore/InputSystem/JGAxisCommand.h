#pragma once
#include"JGCommand.h"

enum class EKey;

/*
Class : JGAxisCommand 
@m function<void(const float)> m_BindEvent : void(const float) �� ���ε��� �̺�Ʈ �Լ�
@m bool bIsBindEvent :  �̺�Ʈ�� ���ε� �Ǿ����� ���� */
class ENGINE_EXPORT JGAxisCommand : public JGCommand
{
private:
	std::function<void(const float)> m_BindEvent;
	bool bIsBindEvent = false;
public:
	JGAxisCommand();
	virtual ~JGAxisCommand();

	virtual void Tick();
	/*
	Exp : JGAxisCommand�� void(const float) ������ �Լ��� ���ε��Ѵ�. */
	void BindAxisCommand(const std::function<void(const float)>& Event);

	void KillCommand();
};