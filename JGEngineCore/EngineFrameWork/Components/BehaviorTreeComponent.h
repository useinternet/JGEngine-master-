#pragma once
#include"Component.h"
#include"../AI/BT_InformationBoard.h"
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
	std::map<const std::wstring, std::unique_ptr<BT_InformationBoard>> m_mBTBoard;
	std::map<const std::wstring, std::unique_ptr<BT_Node>> m_mBTNodes;
public:
	BehaviorTreeComponent();
	virtual ~BehaviorTreeComponent();
	virtual void BeginComponent(World* world) override;
protected:
	virtual void InformationBoardInitSpace();
	virtual void MakeAITreeSpace();

	template<typename UserBoard>
	UserBoard* CreateBoard(const std::wstring& TaskName);

	template<typename UserTask>
	UserTask* CreateTask(const std::wstring& nodeName);
	BT_Selector* CreateSelector(const std::wstring& nodeName);
	BT_Sequence* CreateSequence(const std::wstring& nodeName);
	BT_MiddleNode* GetRoot();
	/*
	Exp : �����̺�� Ʈ���� ���� ������Ʈ Tick�� ���� �ʰ� ���Ӱ� ����. */
	virtual void Tick(const float DeltaTime) override;

};

template<typename UserBoard>
inline UserBoard* BehaviorTreeComponent::CreateBoard(const std::wstring& TaskName)
{
	std::unique_ptr<UserBoard> board = std::make_unique<UserBoard>();
	UserBoard* result = board.get();

	auto iter = m_mBTNodes.find(TaskName);
	if (iter == m_mBTNodes.end())
	{
		return nullptr;
	}
	// ���� ���� �ֱ�
	BT_Task* task = dynamic_cast<BT_Task*>(m_mBTNodes[TaskName].get());
	task->SetBoard(result);

	m_mBTBoard.insert(
		std::pair<const std::wstring, std::unique_ptr<BT_InformationBoard>>(TaskName, std::move(board)));
	return result;
}
template<typename UserTask>
inline UserTask* BehaviorTreeComponent::CreateTask(const std::wstring& nodeName)
{
	std::unique_ptr<UserTask> task = std::make_unique<UserTask>();
	task->InitNode();
	task->RegisterName(nodeName);
	task->BeginTask();
	UserTask* result = task.get();
	m_mBTNodes.insert(
		std::pair<const std::wstring, std::unique_ptr<BT_Node>>(nodeName, std::move(task)));
	return result;
}


