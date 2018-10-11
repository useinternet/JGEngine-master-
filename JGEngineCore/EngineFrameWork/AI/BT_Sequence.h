#pragma once
#include"BT_MiddleNode.h"


// Task : ���� ���( �̺�Ʈ �߻� )
// Selector : ���ʴ�� ���� Ž���� false��� �Լ� ������ Ž��.. �ϴٰ�
// true�� ��ȯ�ϸ� �������� �������´�.
// Sequence : �ڽĳ���� ��� true���߸� �������� �������´�.

class ENGINE_EXPORT BT_Sequence : public BT_MiddleNode
{
private:
	EBTNodeType m_NodeType;
	size_t m_CurrentCount = 0;
	bool   m_bChildNodeAllPass = true;
public:
	BT_Sequence();
	virtual ~BT_Sequence();
	virtual void InitNode() override;
	virtual void Behavior(const float DeltaTime) override;
	virtual EBTNodeType GetNodeType() override;
};