#pragma once
#include"ObjectBase.h"
#include"../Components/MotivatedComponent.h"

class JGDeviceD;
class JGHLSLShaderDevice;
class World;
/*
EnumClass : ������Ʈ ���� */
enum class EObjectState
{
	Active = 0,
	Behind    = 1,
	Destory   = 2
};
/*
Class : Object 
@m std::vector<std::unique_ptr<Component>>  m_vComponents : ������Ʈ �迭 ( �ּ� ������ )
@m Component* m_RootComponent : ���� ���� ������Ʈ ���� ���� */
class ENGINE_EXPORT Object : public ObjectBase
{
private:
	std::vector<std::shared_ptr<Component>> m_vComponents;
	std::vector<Component*> m_vSystemComponents;
	Component* m_RootComponent = nullptr;
	//
	bool m_bIsFirst = true;
	EObjectState m_ObjectState;
	World* m_pWorld = nullptr;
	static float m_DeltaTime;
public:
	Object();
	virtual ~Object();

	/*
	Exp : ������Ʈ�� �����ǰ� ���� ���� �ѹ� ����Ǵ� �̺�Ʈ�Դϴ�. (������ �θ� Ŭ������ BeginObject�� ȣ���Ѵ�.)*/
	virtual void BeginObject(World* world) override;
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
	virtual void EndObject() override;
	/*
	Exp : ������Ʈ�� ���� ���带 �ҷ��´�. */
	World* GetWorld();
	/*
	Exp : ������Ʈ ���¸� �����´�.*/
	EObjectState GetObjectState();
	/*
	Exp : ������Ʈ�� �ı��Ѵ�. */
	void DestoryObject();
	/*
	Exp : ������Ʈ�� �����. */
	void BehindObject();
	/*
	Exp : ������Ʈ�� Ȱ��ȭ��Ų��. */
	void ActiveObject();

	float GetDeltaTime();

protected:
	template<typename ComponentType>
	ComponentType* RegisterComponentInObject(const std::wstring& ComponentName, const bool IsRoot = false);
};

template<typename ComponentType>
inline ComponentType* Object::RegisterComponentInObject(const std::wstring& ComponentName, const bool IsRoot)
{
	// ���� ó�� ����ϴ� ������Ʈ���..
	if (m_bIsFirst)
	{
		m_bIsFirst = false;
		std::unique_ptr<Component> RootComponent;
		// ��Ʈ ������Ʈ�� �����.
		if (IsRoot)
		{
			RootComponent = std::make_unique<ComponentType>();
		}
		else
		{
			RootComponent = std::make_unique<MotivatedComponent>();
		}
		RootComponent->SetOwnerObject(this);
		RootComponent->InitComponent(GetRenderSuperClass());
		m_RootComponent = RootComponent.get();
		m_vComponents.push_back(move(RootComponent));
	}
	// ������Ʈ ����
	std::unique_ptr<Component> component = std::make_unique<ComponentType>();
	component->SetOwnerObject(this);
	component->RegisterName(ComponentName);
	m_RootComponent->AddChild(component.get());

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
		ComponentType* result = dynamic_cast<ComponentType*>(component.get());
		m_vSystemComponents.push_back(component.get());
		m_vComponents.push_back(move(component));
		return result;
	}
}