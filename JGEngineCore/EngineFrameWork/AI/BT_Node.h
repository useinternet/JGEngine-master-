#pragma once
#include"../../EngineStatics/Engine.h"

enum class ENGINE_EXPORT EBTNodeType
{
	None,
	Task,
	Root,
	Sequence,
	Selector
};

// Task ������� �ʿ��Ѱ�      �̸�, Ÿ��, �������̼� ����, �θ� ���
class ENGINE_EXPORT BT_Node
{
private:
	std::wstring        m_NodeName = TT("None");

	class BT_MiddleNode* m_ParentNode = nullptr;
protected:

	bool m_IsPass = false;
public:
	BT_Node();
	virtual ~BT_Node();
	virtual void InitNode();
	virtual void Behavior(const float DeltaTime);
	void RegisterName(const std::wstring& name);           // �̸� ���


	bool IsPass();
	void Pass();
	void NoPass();

	BT_MiddleNode* GetParentNode();                              // �θ��� ��������
	virtual EBTNodeType GetNodeType();
	const std::wstring& GetNodeName();      
	void ThisIsMyParent(BT_MiddleNode* node);                    // �θ� ��� ����
};
