#pragma once
#include"Component.h"

#include"../AI/BT_Selector.h"
#include"../AI/BT_Sequence.h"
#include"../AI/BT_Task.h"
// Task : ���� ���( �̺�Ʈ �߻� )
// Selector : ���ʴ�� ���� Ž���� false��� �Լ� ������ Ž��.. �ϴٰ�
// true�� ��ȯ�ϸ� �������� �������´�.
// Sequence : �ڽĳ���� ��� true���߸� �������� �������´�.
class ENGINE_EXPORT BehaviorTreeComponent : public Component
{
private:
	std::unique_ptr<class BT_Root> m_Root;
	std::vector<std::unique_ptr<BT_Node>> m_vBTNodes;
public:
	BehaviorTreeComponent();
	virtual ~BehaviorTreeComponent();
	virtual void BeginComponent(World* world) override;
	virtual void MakeAITreeSpace();

	template<typename UserTask>
	UserTask* CreateTask(const std::wstring& nodeName);
	BT_Selector* CreateSelector(const std::wstring& nodeName);
	BT_Sequence* CreateSequence(const std::wstring& nodeName);
	BT_MiddleNode* GetRoot();
	/*
	Exp : �����̺�� Ʈ���� ���� ������Ʈ Tick�� ���� �ʰ� ���Ӱ� ����. */
	virtual void Tick(const float DeltaTime) override;

};
template<typename UserTask>
inline UserTask* BehaviorTreeComponent::CreateTask(const std::wstring& nodeName)
{
	std::unique_ptr<UserTask> task = make_unique<UserTask>();
	task->InitNode();
	task->RegisterName(nodeName);

	UserTask* result = task.get();

	m_vBTNodes.push_back(move(task));

	return result;
}
