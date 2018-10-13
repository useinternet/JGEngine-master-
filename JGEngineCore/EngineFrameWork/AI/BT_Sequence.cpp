#include"BT_Sequence.h"

BT_Sequence::BT_Sequence()
{
	m_NodeType = EBTNodeType::Sequence;
}

BT_Sequence::~BT_Sequence()
{
}

void BT_Sequence::InitNode()
{
	BT_MiddleNode::InitNode();
	m_CurrentCount = 0;
	m_bChildNodeAllPass = true;
}

void BT_Sequence::Behavior(const float DeltaTime)
{
	BT_MiddleNode::Behavior(DeltaTime);
	if (GetChildNodeList_p()->empty())
	{
		Pass();
		return;
	}
	NoPass();


	auto ExcuteNode = GetChildNodeList_p()->begin();
	advance(ExcuteNode, m_CurrentCount);
	// �ش� ��ġ�� ��� �ൿ ����..
	(*ExcuteNode)->Behavior(DeltaTime);

	// ���� ��尡 false�� ��ȯ�ߴٸ�..
	// ��� �ڽĳ�尡 true�� ��ȯ ���� �ʾ����Ƿ� �ϴ� üũ�صд�.
	if (!(*ExcuteNode)->IsPass())
	{
		m_bChildNodeAllPass = false;
	}
	//  ������ġ�� ������ġ�� �ű��..
	m_CurrentCount++;

	// ������ üũ
	if (m_CurrentCount >= GetListSize())
	{
		// �ڽĳ�尡 ��� true�� ��ȯ������ pass �ƴϸ� nopass
		m_bChildNodeAllPass ? Pass() : NoPass();
		InitNode();
	}
}

EBTNodeType BT_Sequence::GetNodeType()
{
	return m_NodeType;
}
