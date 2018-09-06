#pragma once
#include"AnimationMesh2DComponent.h"
class Object;
typedef unsigned int EnumState;
/*
Exp : ������Ʈ ���¿����� �ִϸ��̼� ����� ������.. ���� ������Ʈ */
class ENGINE_EXPORT Animation2DSystemComponent : public Motivated2DComponent
{
private:
	struct SAnimNotify
	{
		EnumState State;
		size_t Frame;
		std::function<void()> Event;
	};

private:
	std::map<EnumState,std::shared_ptr<AnimationMesh2DComponent>> m_mAnimation;
	std::map<const std::wstring, SAnimNotify> m_mNotifys;
	Object* m_pLinkObject = nullptr;
	EnumState m_CurrentState;
public:
	Animation2DSystemComponent(const Animation2DSystemComponent& copy) = delete;
	Animation2DSystemComponent();
	virtual ~Animation2DSystemComponent();
	virtual void Tick(const float DeltaTime) override;
	virtual void Render() override;

	/*
	Exp : �� �ִϸ��̼��� ������ �ش� ������Ʈ�� �����Ѵ�. */
	void LinkObject(Object* object);
	/*
	Exp : �� �ִϸ��̼ǿ� ����� ������Ʈ�� �����´�. */
	Object* GetLinkObject();


	/*
	�ִϸ��̼� ���� ���¸� �����Ѵ�. */
	void SetCurrentState(const EnumState State);

	/*
	�ִϸ��̼� ���� ���¸� �����´�. */
	EnumState GetCurrentState();
	/*
	Exp : �ش� �����϶� �ִϸ��̼� �߰�..( ���⼭ �� ������Ʈ�� �ڽ����� �߰�..�׷��� ��ġ���� �����Ǵϱ�) */
	AnimationMesh2DComponent* AddAnimation(EnumState State,const std::wstring& AnimationName);
	/*
	Exp : �˸� �̺�Ʈ �߰��ϱ� */
	void AddNotifyEvent(const std::wstring& NotifyName, EnumState State, const size_t Frame, const std::function<void()>& Event);




	AnimationMesh2DComponent* GetAnimation(EnumState State);
};
