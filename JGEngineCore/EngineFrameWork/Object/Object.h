#pragma once
#include"ObjectBase.h"

class Component;
class JGDeviceD;
class JGHLSLShaderDevice;
/*
Class : Object 
@m std::vector<std::unique_ptr<Component>>  m_vComponents : ������Ʈ �迭 ( �ּ� ������ )
@m Component* m_RootComponent : ���� ���� ������Ʈ ���� ���� */
class Object : public ObjectBase
{
private:
	std::vector<std::unique_ptr<Component>>  m_vComponents;
	Component* m_RootComponent;
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
	virtual void Render(JGDevice* Device, JGHLSLShaderDevice* HlslDevice) override;
	/*
	Exp : ������Ʈ�� �ı��ϱ��� ���� �ѹ� ����Ǵ� �̺�Ʈ�Դϴ�.(������ �θ� Ŭ������ DestroyObject�� ȣ���Ѵ�.) */
	virtual void DestroyObject() override;

protected:
	template<typename ComponentType>
	ComponentType* RegisterComponentInObject(std::wstring& ComponentName);
};

