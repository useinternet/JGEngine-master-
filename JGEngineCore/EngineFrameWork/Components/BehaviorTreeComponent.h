#pragma once
#include"Component.h"
class BT_Root;
// Task : ���� ���( �̺�Ʈ �߻� )
// Selector : ���ʴ�� ���� Ž���� false��� �Լ� ������ Ž��.. �ϴٰ�
// true�� ��ȯ�ϸ� �������� �������´�.
// Sequence : �ڽĳ���� ��� true���߸� �������� �������´�.
class ENGINE_EXPORT BehaviorTreeComponent : public Component
{
private:
	std::unique_ptr<BT_Root> m_TreeRoot;

public:
	BehaviorTreeComponent();
	virtual ~BehaviorTreeComponent();

	/*
	Exp : �����̺�� Ʈ���� ���� ������Ʈ Tick�� ���� �ʰ� ���Ӱ� ����. */
	virtual void Tick(const float DeltaTime) override;

};