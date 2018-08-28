#include"Object.h"
#include"../Components/Component.h"
#include"../Components/MotivatedComponent.h"

Object::Object()
{
	RegisterObjectID(typeid(this));
	std::unique_ptr<Component> RootComponent;
}

Object::~Object()
{

}

void Object::BeginObject()
{

}

void Object::Send(JGComponentMessage & Message)
{

}

void Object::Tick(const float DeltaTime)
{

}

void Object::Render(JGDevice* Device, JGHLSLShaderDevice* HlslDevice)
{

}

void Object::DestroyObject()
{

}

template<typename ComponentType>
typename ComponentType* Object::RegisterComponentInObject(std::wstring& ComponentName)
{
	// ���� ó�� ����ϴ� ������Ʈ���..
	if (m_bIsFirst)
	{
		m_bIsFirst = false;
		// ��Ʈ ������Ʈ�� �����.
		std::unique_ptr<Component> RootComponent = make_unique<Component>();
		m_RootComponent = RootComponent.get();
		m_vComponents.push_back(move(RootComponent));
	}
	// ������Ʈ ����
	std::unique_ptr<Component> component = make_unique<ComponentType>();
	component->RegisterName(ComponentName);

	// ���� ���� ������Ʈ���� üũ�Ѵ�.
	MotivatedComponent* Check = dynamic_cast<MotivatedComponent*>(component.get());
	// ���� ���� ������Ʈ���.
	if (Check)
	{
		// ��Ʈ ������Ʈ �ڽ����� ������Ʈ�� �߰��Ѵ�.
		ComponentType* result = Check.get();
		m_RootComponent->AddChild(component.get());
		m_vComponents.push_back(move(component));
		return result;
	}
	else
	{
		// �ƴ϶�� �׳� �߰��Ѵ�.
		ComponentType* result = Check.get();
		m_vComponents.push_back(move(component));
		return result;
	}
}
