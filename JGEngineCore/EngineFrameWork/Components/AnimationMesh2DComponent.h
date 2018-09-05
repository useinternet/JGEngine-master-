#pragma once
#include"StaticMesh2DComponent.h"


class ENGINE_EXPORT AnimationMesh2DComponent : public StaticMesh2DComponent
{
private:
	// �ִϸ��̼� ���� 
	// 1. ���忡 �ً����ڴ� �ִϸ��̼�
	// 2. ���� ���� �ִϸ��̼�

	// �ִϸ��̼� ������..


	// - ���忡 �ً��� �ڴ� �ִϸ��̼ǿ� �ʿ��� ����
	// �� ������ / ���� ������ / ���� ������ ��



	// - ���� ���� �ִϸ��̼�
	// �� ������
	// �ִϸ��̼� ������..


	// �ݺ� �ִϸ��̼� �� �ʿ��� ����(1�� ��ó���̶� ����)
	// ù �κ� / ������ �ݺ� �κ�
	// �ݺ� Ƚ�� (-1 �̸� ���� �ݺ�)
	//
	bool FirstTick = true;

	float m_AnimationDelay = 0.0f;
	size_t m_TotalFrame  = 0;
	size_t m_WidthFrame  = 0;
	size_t m_HeightFrame = 0;

	bool m_IsRepeat = false;
	size_t m_RepeatStartFrame = 0;
	size_t m_RepeatEndFrame = 0;
	size_t m_RepeatCount = 0;

private:
	float m_AccTime = 0.0f;
	size_t m_CurrentFrame = 1;
	
	float m_IncreaseWidth = 0.0f;
	float m_IncreaseHeight = 0.0f;
	float m_CurrentIncreaseWidth  = 0.0f;
	float m_CurrentIncreaseHeight = 0.0f;
public:
	AnimationMesh2DComponent(const AnimationMesh2DComponent& copy) = delete;
	AnimationMesh2DComponent();
	~AnimationMesh2DComponent();

	virtual void Tick(const float DeltaTime) override;
	virtual void Render() override;
	void SetAnimation(const size_t TotalFrame, const size_t WidthFrame, const size_t HeightFrame, const float AnimationDelay,
		const float TextureWidth, const float TextureHeight);
	void AnimationRepeatOn(const size_t StartFrame, const size_t EndFrame, const size_t RepeatCount);

private:
	void AnimationCalulation();
};