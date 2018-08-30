#pragma once
#include"ObjectBase.h"
#include"../Components/MotivatedComponent.h"

class JGDeviceD;
class JGHLSLShaderDevice;
/*
Class : Object 
@m std::vector<std::unique_ptr<Component>>  m_vComponents : ������Ʈ �迭 ( �ּ� ������ )
@m Component* m_RootComponent : ���� ���� ������Ʈ ���� ���� */
class ENGINE_EXPORT Object : public ObjectBase
{
private:
	std::vector<std::shared_ptr<Component>> m_vComponents;
	Component* m_RootComponent = nullptr;
	//
	bool m_bIsFirst = true;

public:
	Object();
	virtual ~Object();

	/*
	Exp : ������Ʈ�� �����ǰ� ���� ���� �ѹ� ����Ǵ� �̺�Ʈ�Դϴ�. (������ �θ� Ŭ������ BeginObject�� ȣ���Ѵ�.)*/
	virtual void BeginObject() override;
	/*
	Exp : ������Ʈ �޼����� �����ϴ�. 
	@param : JGComponentMessage& Message : ������Ʈ �޽��� Ŭ���� (������ �θ� Ŭ������ Send�� ȣ���Ѵ�.)*/
	virtual void Send(JGComponentMessage& Message) override;
	/*
	Exp : 1�����Ӵ� �ѹ� ȣ�� �Ǵ� �̺�Ʈ �Դϴ�. (������ �θ� Ŭ������ Tick�� ȣ���Ѵ�.)*/
	virtual void Tick(const float DeltaTime) override;
	/*
	Exp : ������ �մϴ�.(��ӹ��� ����) */
	virtual void Render() override;
	/*
	Exp : ������Ʈ�� �ı��ϱ��� ���� �ѹ� ����Ǵ� �̺�Ʈ�Դϴ�.(������ �θ� Ŭ������ DestroyObject�� ȣ���Ѵ�.) */
	virtual void DestroyObject() override;

protected:
	template<typename ComponentType>
	ComponentType* RegisterComponentInObject(const std::wstring& ComponentName);
};

template<typename ComponentType>
typename ComponentType* Object::RegisterComponentInObject(const std::wstring& ComponentName)
{
	// ���� ó�� ����ϴ� ������Ʈ���..
	if (m_bIsFirst)
	{
		m_bIsFirst = false;
		// ��Ʈ ������Ʈ�� �����.
		std::unique_ptr<Component> RootComponent = std::make_unique<Component>();
		RootComponent->InitComponent(GetRenderSuperClass());
		m_RootComponent = RootComponent.get();
		m_vComponents.push_back(move(RootComponent));
	}
	// ������Ʈ ����
	std::unique_ptr<Component> component = std::make_unique<ComponentType>();
	component->RegisterName(ComponentName);

	// ���� ���� ������Ʈ���� üũ�Ѵ�.
	MotivatedComponent* Check = dynamic_cast<MotivatedComponent*>(component.get());
	// ���� ���� ������Ʈ���.
	if (Check)
	{
		// ��Ʈ ������Ʈ �ڽ����� ������Ʈ�� �߰��Ѵ�.
		ComponentType* result = dynamic_cast<ComponentType*>(Check);
		m_RootComponent->AddChild(component.get());
		m_vComponents.push_back(move(component));
		return result;
	}
	else
	{
		// �ƴ϶�� �׳� �߰��Ѵ�.
		ComponentType* result = dynamic_cast<ComponentType*>(Check);
		m_vComponents.push_back(move(component));
		return result;
	}
}