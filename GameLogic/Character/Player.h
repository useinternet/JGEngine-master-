#pragma once
#include"EngineFrameWork/Object/ExistObject.h"

enum class EReSizePivot;

// �÷��̾� ����
enum EPlayerState
{
	Player_IdleStart,
	Player_Idle,
	Player_RightMove,
	Player_LeftMove,
	Player_JumpDown,
	Player_JumpUp,
	Player_Sit
};

// �ִϸ��̼� �� ��ġ �� �ڽ� ũ�� ���� ����ü
struct SPlayerPosByAnim
{
	float hx;
	float hy;
	float x;
	float y;
	EReSizePivot pivot;
};
class Player : public ExistObject
{

private:
	// �ִϸ��̼�
	class Anim_Player* AnimPlayer;
	// �浹ü 
	class Box2DCollisionComponent* BoxCollision;
	// �Է�
	class InputComponent* InputDevice;

	// �÷��̾� ���� ���� ����
	bool bFalling     = false;
	bool bFallingDown = false;
	bool bAnimationChange = false;
	EPlayerState PrevPlayerState;
	EPlayerState CurrentPlayerState;
	float desiredVel = 0;
	float FallingCheck_Y = 0.0f;


	// ȯ�� ����..
	float JumpForce = 500.0f;
	float Speed = 500.0f;

	// �ִϸ��̼� ��ġ ����
	std::map<EPlayerState, SPlayerPosByAnim> m_mBox_Pos_Config;

public:
	Player();
	virtual ~Player();
	virtual void BeginObject(World* world) override;
	virtual void Tick(const float DeltaTime) override;
	void RightStart();
	void RightEnd();
	void LeftStart();
	void LeftEnd();
	void Jump();
	void JumpEnd();
	void SitDownStart();
	void SitDownEnd();
	bool IsFalling();
	EPlayerState GetCurrentPlayerState();
	void SetCurrentPlayerState(EPlayerState State);
};