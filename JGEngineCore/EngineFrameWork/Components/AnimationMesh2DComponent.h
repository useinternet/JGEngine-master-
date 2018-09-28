#pragma once
#include"StaticMesh2DComponent.h"
/*
Class : AnimationMesh2DComponent 
@m float m_AnimationDelay : �ִϸ��̼� ������ ��.
@m size_t m_TotalFrame  : �� ������
@m size_t m_WidthFrame  : ���� ������
@m size_t m_HeightFrame : ���� ������
@m float m_AccTime : ���� �ð�
@m size_t m_CurrentFrame  : ���� ������
@m float m_IncreaseWidth  : ���� 1������ ������
@m float m_IncreaseHeight : ���� 1������ ������
@m float m_CurrentIncreaseWidth  : ���� ���� ������ ������
@m float m_CurrentIncreaseHeight : ���� ���� ������ ������ */
class ENGINE_EXPORT AnimationMesh2DComponent : public StaticMesh2DComponent
{
private:
	bool  m_bStop = false;
	float m_AnimationDelay = 0.0f;
	size_t m_TotalFrame    = 0;
	size_t m_WidthFrame    = 0;
	size_t m_HeightFrame   = 0;
private:
	float  m_AccTime = 0.0f;
	size_t m_CurrentFrame = 1;
	
	float m_IncreaseWidth  = 0.0f;
	float m_IncreaseHeight = 0.0f;
	float m_CurrentIncreaseWidth  = 0.0f;
	float m_CurrentIncreaseHeight = 0.0f;


	/// �ִϸ��̼� 1ȸ�� ����
	bool m_AnimationEnd = false;
	std::unique_ptr<struct SAnimationIncrementBuffer_VS> m_AnimationBuffer;
public:
	AnimationMesh2DComponent();
	virtual ~AnimationMesh2DComponent();
	/*
	Exp : ���̴� ��������� �Ķ���͵��� �����Ѵ�. ( ��� �޾Ƽ� ������ / �θ� �Լ� ȣ�� ���ص��ǿ�..) */
	virtual void ShaderParamSetting(JGShaderData* Data);
	/// ComponentŬ���� ���� ***
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
	/*
	Exp : �ִϸ��̼� ������ �ʱ�ȭ�Ѵ�. */
	void InitAnimationSetting();


	void Stop();
	void Play();
	bool IsPlaying();
	void SetCurrentFrame(const size_t CurrentFrame);
	/// StaticMesh2DComponent Ŭ���� ����...
	virtual void SetConstructObject(ConstructObject* Object) override;
private:
	/// �ִϸ��̼� ������ ���..
	void AnimationCalulation();
};