#include"JGAxisCommand.h"
#include"JGPressManager.h"
JGAxisCommand::JGAxisCommand()
{
}

JGAxisCommand::~JGAxisCommand()
{
}

void JGAxisCommand::Tick()
{
	// �̺�Ʈ�� ���ε� �Ǹ� �ش� ������ �����Ѵ�. 
	if (bIsBindEvent)
	{
		m_BindEvent(GetPressManager()->GetAxisKey(GetKey()));
	}
}

void JGAxisCommand::BindAxisCommand(const std::function<void(const float)>& Event)
{
	bIsBindEvent = true;
	m_BindEvent = Event;
}
