#pragma once
#include"ComponentBase.h"
#include"../../EngineStatics/JMath/JGVector3D.h"
#include"../../EngineStatics/JMath/JGVector2D.h"
#include"../../EngineStatics/JMath/JGScale3D.h"
#include"../../EngineStatics/JMath/JGScale2D.h"
#include"../../EngineStatics/JMath/JGRotator.h"
#include"../../EngineStatics/JMath/JGMatrix.h"
#include"../../EngineStatics/JMath/JGMath.h"
#include"../../EngineStatics/JMath/JGAngle2D.h"

class JGDeviceD;
class JGHLSLShaderDevice;
/*
EnumClass : EComponentState 
@e Active : Ȱ��ȭ
@e Behind : ����  */
enum class EComponentState
{
	Active,
	Behind
};

class ENGINE_EXPORT Component : public ComponentBase
{
	std::vector<Component*> m_vChildComponent;
	std::wstring            m_ComponentName = TT("None");
	EComponentState         m_ComponentState;
	Component* m_ParentComponent = nullptr;
	World*     m_pWorld = nullptr;
	int        m_zOrder = 0;
public:
	Component();
	virtual ~Component();

	/*
	Exp : �޼����� �޴´�. ( �θ� Ŭ������ Receive�� ȣ���ؾ��Ѵ�. ) */
	virtual void Receive(const JGComponentMessage& Message) override;
	/*
	Exp : ������Ʈ�� �����ǰ� ���� ����Ǵ� �Լ��̴�. ( �θ� Ŭ������ BeginComponent�� ȣ���ؾ��Ѵ�. ) */
	virtual void BeginComponent(World* world) override;
	/*
	Exp : �� ������ �ѹ� ����Ǵ� �Լ��̴�. ( �θ� Ŭ������ Tick�� ȣ���ؾ��Ѵ�. ) */
	virtual void Tick(const float DeltaTime) override;
	/*
	Exp : ������ �Ѵ�.(���̴��� �ٸ��ٸ� ��� �޾Ƽ� Ŀ���� �ϼ�
	���!! : Component�� Render�Լ��� ȣ���Ұ�..) */
	virtual void Render() override;
	/*
	Exp :  ������Ʈ�� �ı��Ǳ��� �ѹ� ����Ǵ� �Լ��̴�. ( �θ� Ŭ������ DestoryComponent�� ȣ���ؾ��Ѵ�. ) */
	virtual void DestoryComponent() override;
public:
	/*
	Exp : ������Ʈ �̸��� ����Ѵ�. (������Ʈ�� Ž���Ҷ� ���δ�.)
	@param : const wstring& Name : ����� ������Ʈ �̸� */
	void RegisterName(const std::wstring& Name);
	/*
	Exp : ������Ʈ �̸��� �����´�. */
	const std::wstring& GetComponentName();

	/*
	Exp : ������Ʈ�� �ڽ��� �߰��Ѵ�. ( ���� �߰��ϴ� �ڽ� ������Ʈ�� �θ� �����Ѵٸ�
	���� �θ� ������Ʈ�� �ִ� �ڽ�������Ʈ�� ���ŵǰ� ���� �߰��ҷ��� �θ� ������Ʈ�� �߰��ȴ�.
	���� �ڽ�������Ʈ�� �θ�� ���� �߰��ҷ��� �θ� ������Ʈ���ȴ�.)
	@param : Component* Child : �ڽ��� �� ������Ʈ */
	void AddChild(Component* Child);
	/*
	Exp : ���� ������Ʈ�� �θ� ������Ʈ�� �����´�. */
	Component* GetParent();
	/*
	Exp : ���� ������Ʈ�� �������ִ� �ڽ� ������Ʈ���� �迭(vector����)�� �����´�. */
	std::vector<Component*>& GetChildArray();

	/*
	Exp : �ش� ������Ʈ ID�� �ش��ϴ� �ڽ�������Ʈ���� �迭(vector)�������� �����´�. */
	std::vector<Component*> GetChildArray(const char* ComponentID);
	/*
	Exp : �ش� ������Ʈ ID�� �ش��ϴ� �ڽ� ������Ʈ�� �����´�. (�������� �ִٸ� �� ó�� �ش��ϴ� ������Ʈ�� �����´�. )
	@param : const char* ComponentID : ������Ʈ ���̵� */
	Component* GetChild(const char* ComponentID);
	/*
	Exp : �ش� ������Ʈ �̸��� �ش��ϴ� �ڽ� ������Ʈ�� �����´�.( ���� �̸��� ���ٴ� �����Ͽ� �����´�.) 
	@param : const wstring& ComponentName : ������Ʈ �̸� */
	Component* GetChild(const std::wstring& ComponentName);
	/*
	Exp : index��ġ�� �����ϴ� �ڽ� ������Ʈ�� �����´�. 
	@param : const size_T index : �ڽ� ������Ʈ �ε��� */
	Component* GetChild(const size_t index);

	/*
	Exp : Zorder���� �����Ѵ�. ( �׸��� ����. zorder ���� Ŭ���� ���߿� �׸���.) */
	void SetZOrder(const int zorder);
	/*
	Exp : Zorder���� �����´�.*/
	int GetZOrder();
	/*
	Exp : ������Ʈ�� ����ϴ�. */
	void BehindComponent();
	/*
	Exp : ������Ʈ�� Ȱ��ȭ�մϴ�. */
	void ActiveComponent();
	/*
	Exp : ���� ������Ʈ ���¸� �����ɴϴ�. */
	EComponentState GetComponentState();

	/*
	Exp : ������Ʈ�� ���� ���带 �����´�. */
	World* GetWorld();
private:
	void SetParent(Component* Parent);
};