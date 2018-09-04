#include"JGKeyCommand.h"

JGKeyCommand::JGKeyCommand()
{
	m_KeyState = EKeyState::None;
}

JGKeyCommand::~JGKeyCommand()
{
}

void JGKeyCommand::Tick(const float DeltaTime)
{
	JGCommand::Tick(DeltaTime);

	if (GetIsPressed() != m_PrevPressed)
	{
		m_bKeyVariance = true;
		m_PrevPressed = GetIsPressed();
	}
	// Ű�� ��������
	if (m_bKeyVariance && GetIsPressed())
	{
		m_KeyState = EKeyState::Down;
	}
	// Ű�� �H����
	else if (m_bKeyVariance && !GetIsPressed())
	{
		m_KeyState = EKeyState::Up;
		m_bKeyVariance = false;
	}
	else if (!m_bKeyVariance)
	{
		m_KeyState = EKeyState::None;
	}

	// �̺�Ʈ �߻��� Ʈ���� Ű���¿� ���� Ű���°� ������ �̺�Ʈ ����.
	if (m_EventTriggerKeyState == m_KeyState)
	{
		m_BindEvent();
	}
}

void JGKeyCommand::BindKeyCommand(const EKeyState KeyState, const std::function<void()>& Event)
{
	m_EventTriggerKeyState = KeyState;
	m_BindEvent = Event;
}
