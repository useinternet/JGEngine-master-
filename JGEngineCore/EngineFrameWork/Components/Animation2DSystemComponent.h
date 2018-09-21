#pragma once
#include"AnimationMesh2DComponent.h"
class Object;
typedef unsigned int EnumState;


// ���߿� �߰��� ��
// �ִϸ��̼� ������.. ���� �ִϸ��̼��� ������ �ʱ�ȭ �Ѵ�.



/*
Class : Animation2DSystemComponent
Exp : �ִϸ��̼� �ý��� ������Ʈ( ��� �ؼ� �ִϸ��̼��� ���� �����ؼ�����.) 
@m map<EnumState,shared_ptr<AnimationMesh2DComponent>> m_mAnimation : �ִϸ��̼� ��������Ʈ �迭
@m map<const wstring, SAnimNotify> m_mNotifys : �ִϸ��̼� �˸� �̺�Ʈ
@m Object* m_pLinkObject    : ����� ������Ʈ( ���� ���� �� )
@m EnumState m_CurrentState : ���� �ִϸ��̼� ����(����� ����)    */
class ENGINE_EXPORT Animation2DSystemComponent : public Motivated2DComponent
{
private:
	/*
	Struct : SAnimNotify
	@s EnumState State : �ִϸ��̼� ����(����� ����)
	@s size_t Frame    : �˸� ������ ��ġ
	@s function<void()> Event : ��Ƽ����(�˸�) �̺�Ʈ �Լ�*/
	struct SAnimNotify
	{
		EnumState State;
		size_t    Frame;
		std::function<void()> Event;
	};
private:
	std::map<EnumState,std::shared_ptr<AnimationMesh2DComponent>> m_mAnimation;
	std::map<const std::wstring, SAnimNotify> m_mNotifys;
	EnumState m_CurrentState;
public:
	Animation2DSystemComponent(const Animation2DSystemComponent& copy) = delete;
	Animation2DSystemComponent();
	virtual ~Animation2DSystemComponent();
	/// Component �� ��� ���� ���� �Լ���..(�ڼ��Ѱ� Component Ŭ���� ����)
	virtual void BeginComponent(World* world) override;
	virtual void Tick(const float DeltaTime) override;
	virtual void Render() override;
	//
	/*
	Exp : �� �ִϸ��̼��� ������ �ش� ������Ʈ�� �����Ѵ�. */
	void LinkObject(Object* object);
	/*
	Exp : �� �ִϸ��̼ǿ� ����� ������Ʈ�� �����´�. */
	Object* GetLinkObject();
	/*
	Exp : �ִϸ��̼� ���� ���¸� �����Ѵ�. */
	void SetCurrentState(const EnumState State);
	/*
	Exp : �ִϸ��̼� ���� ���¸� �����´�. */
	EnumState GetCurrentState();
	/*
	Exp : �ش� �����϶� �ִϸ��̼� �߰�..( ���⼭ �� ������Ʈ�� �ڽ����� �߰�..�׷��� ��ġ���� �����Ǵϱ�) 
	@param const EnumState State : �ִϸ��̼� ����(����� ����) 
	@param const wstring& AnimationName : �ִϸ��̼� �̸�. */
	AnimationMesh2DComponent* AddAnimation(const EnumState State,const std::wstring& AnimationName);
	/*
	Exp : �˸� �̺�Ʈ �߰��ϱ� 
	@param const wstring&          NotifyName : �˸� �̺�Ʈ �̸�.
	@param const EnumState         State      : �˸��� �߰��� �ִϸ��̼� ����
	@param const function<void()>& Event      : �˸� �̺�Ʈ �Լ�*/
	void AddNotifyEvent(const std::wstring& NotifyName, const EnumState State, const size_t Frame, const std::function<void()>& Event);
	/*
	Exp : �ش� �ִϸ��̼� ������ ��������Ʈ�� �����´�. */
	AnimationMesh2DComponent* GetAnimation(const EnumState State);
};
