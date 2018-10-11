#pragma once
#include"BT_MiddleNode.h"
// ���� ���带 ��� �ڽĳ�忡�� ������.

class ENGINE_EXPORT BT_Root : public BT_MiddleNode
{
private:
	EBTNodeType m_NodeType;
	size_t      m_CurrentCount = 0;
public:
	BT_Root();
	virtual ~BT_Root();
	virtual void Behavior(const float DeltaTime) override;
	virtual EBTNodeType GetNodeType() override;
};