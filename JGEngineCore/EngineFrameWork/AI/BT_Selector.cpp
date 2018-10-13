#include"BT_Selector.h"
using namespace std;
BT_Selector::BT_Selector()
{
	m_NodeType = EBTNodeType::Selector;
}

BT_Selector::~BT_Selector()
{

}
void BT_Selector::InitNode()
{
	BT_MiddleNode::InitNode();
	m_CurrentCount = 0;
}
void BT_Selector::Behavior(const float DeltaTime)
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

	// ��� �ൿ
	(*ExcuteNode)->Behavior(DeltaTime);
	// �ش� ��尡 true �ϰ�쿡��
	if ((*ExcuteNode)->IsPass())
	{
		InitNode();
		Pass(); // �н� = true
	}
	// �ش� ��尡 false�� ��쿡��
	else
	{
		// �������� ���� ��带 ���� ��Ű��.. 
		m_CurrentCount++;
		// ���� ��ġ�� ��尡 ����Ʈ�� ����� �������� �ٽ� ó������ ��带 ����..
		if (m_CurrentCount >= GetListSize()) 
		{
			m_CurrentCount = 0;
		}
		// Pass = false;
		NoPass();
	}
}
EBTNodeType BT_Selector::GetNodeType()
{
	return m_NodeType;
}
