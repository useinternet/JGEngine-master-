#pragma once
#include"StaticMesh2DComponent.h"
/*
Class : AnimationMesh2DComponent 
@m float m_AnimationDelay : �ִϸ��̼� ������ ��.
@m size_t m_TotalFrame : �� ������
@m	size_t m_WidthFrame : ���� ������
@m	size_t m_HeightFrame : ���� ������
@m	bool m_IsRepeat : �ִϸ��̼� �ݺ� ����
@m	size_t m_RepeatStartFrame : �ݺ� ���� ����
@m	size_t m_RepeatEndFrame : �ݺ� �� ����
@m	size_t m_RepeatCount : �ݺ� Ƚ��(-1�̸� ���ѹݺ�)
@m float m_AccTime : ���� �ð�
@m size_t m_CurrentFrame : ���� ������
@m float m_IncreaseWidth : ���� 1������ ������
@m float m_IncreaseHeight : ���� 1������ ������
@m float m_CurrentIncreaseWidth : ���� ���� ������ ������
@m float m_CurrentIncreaseHeight : ���� ���� ������ ������ */
class ENGINE_EXPORT AnimationMesh2DComponent : public StaticMesh2DComponent
{
private:
	float m_AnimationDelay = 0.0f;
	size_t m_TotalFrame  = 0;
	size_t m_WidthFrame  = 0;
	size_t m_HeightFrame = 0;
private:
	float m_AccTime = 0.0f;
	size_t m_CurrentFrame = 1;
	
	float m_IncreaseWidth = 0.0f;
	float m_IncreaseHeight = 0.0f;
	float m_CurrentIncreaseWidth  = 0.0f;
	float m_CurrentIncreaseHeight = 0.0f;
	
	bool m_AnimationEnd = false;
	std::unique_ptr<struct SAnimationIncrementBuffer_VS> m_AnimationBuffer;
public:
	AnimationMesh2DComponent(const AnimationMesh2DComponent& copy) = delete;
	AnimationMesh2DComponent();
	~AnimationMesh2DComponent();
	/*
	Exp : ���̴� ��������� �Ķ���͵��� �����Ѵ�. ( ��� �޾Ƽ� ������ / �θ� �Լ� ȣ�� ���ص��ǿ�.. */
	virtual void ShaderParamSetting(JGShaderData* Data);

	virtual void Tick(const float DeltaTime) override;
	/*
	Exp : �ִϸ��̼� ������ �� ���� */
	void AnimationSetDelay(const float Delay);
	/*
	Exp : �ִϸ��̼��� ��������.. */
	bool AnimationIsEnd();
	/*
	Exp : �ش� �������ϋ� */
	bool AnimationIsFrame(const size_t Frame);


	virtual void SetConstructObject(ConsructObject* Object) override;
private:
	void AnimationCalulation();
};