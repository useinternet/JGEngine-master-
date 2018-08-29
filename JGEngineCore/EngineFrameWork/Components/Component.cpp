#include "Component.h"
#include"../../RenderSystem/JGDeviceD.h"
#include"../../RenderSystem/JGHLSLShaderDevice/JGHLSLShaderDevice.h"
using namespace std;
Component::Component()
{
	RegisterComponentID(typeid(this));

}

Component::~Component()
{
}

void Component::Receive(const JGComponentMessage& Message)
{
	for (auto& iter : m_vChildComponent)
	{
		iter->Receive(Message);
	}
}

void Component::BeginComponent()
{
	for (auto& iter : m_vChildComponent)
	{
		iter->BeginComponent();
	}
}

void Component::Tick(const float DeltaTime)
{
	for (auto& iter : m_vChildComponent)
	{
		iter->Tick(DeltaTime);
	}
}

void Component::Render()
{
	for (auto& iter : m_vChildComponent)
	{
		iter->Render();
	}
}

void Component::DestoryComponent()
{

}


void Component::RegisterName(const wstring& Name)
{
	m_ComponentName = Name;
}
const wstring& Component::GetComponentName()
{
	return m_ComponentName;
}

void Component::AddChild(Component* Child)
{
	Component* ParentOfChild = Child->GetParent();
	// ���� �߰��ؾ��� �ڽ� ������Ʈ�� �θ� ������Ʈ�� �ִٸ�.. 
	if (ParentOfChild)
	{
		Component* TempChild = Child;
		// �θ� ������Ʈ���ִ� �ڽĹ迭���� �����Ѵ�. ����..
		for (auto iter = ParentOfChild->GetChildArray().begin();
			      iter < ParentOfChild->GetChildArray().end(); )
		{
			if (Child == *iter)
			{
				iter = ParentOfChild->GetChildArray().erase(iter);
			}
			else
			{
				++iter;
			}
		}
		// ���� �ڱ� �ڽ� ������Ʈ �迭�� �� �ڽ� ������Ʈ�� �߰��ϰ�
		m_vChildComponent.push_back(TempChild);
		// �ڽ�������Ʈ�� �θ� ������Ʈ�� ���� �ȴ�.
		TempChild->SetParent(this);
	}
	// �ƴϸ� �׳� �߰�..
	else
	{
		m_vChildComponent.push_back(Child);
		Child->SetParent(this);
	}
}

Component* Component::GetParent()
{
	return m_ParentComponent;
}
std::vector<Component*>& Component::GetChildArray()
{
	return m_vChildComponent;
}
std::vector<Component*> Component::GetChildArray(const char* ComponentID)
{
	vector<Component*> result;
	for (auto& iter : m_vChildComponent)
	{
		if (iter->GetID() == ComponentID)
		{
			result.push_back(iter);
		}
	}
	return result;
}
Component* Component::GetChild(const char* ComponentID)
{
	for (auto& iter : m_vChildComponent)
	{
		if (ComponentID == iter->GetID())
		{
			return iter;
		}
	}
	return nullptr;
}
Component* Component::GetChild(const std::wstring& ComponentName)
{
	for (auto& iter : m_vChildComponent)
	{
		if (ComponentName == iter->GetComponentName())
		{
			return iter;
		}
	}
	return nullptr;
}
Component* Component::GetChild(const size_t index)
{
	if (m_vChildComponent.size() <= index)
	{
		return nullptr;
	}
	return m_vChildComponent[index];
}

void Component::SetParent(Component* Parent)
{
	m_ParentComponent = Parent;
}
