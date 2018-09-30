#pragma once
#include"EngineFrameWork/Object/Unit.h"
enum class EReSizePivot;

// ����  : FallingBlock()�̰ŋ���..
// Box ũ�⸦ �ۼ�Ʈ�� �����ؼ� ����� �����ϱ�� ����.
// ������ 2����..


// �÷��̾� ����
enum EPlayerState
{
	// �⺻
	Player_RightIdle,
	Player_LeftIdle,

	// �̵�
	Player_RightMove,
	Player_LeftMove,

	// ����
	Player_RightJumpDown,
	Player_RightJumpUp,
	Player_LeftJumpDown,
	Player_LeftJumpUp,
	
	// �ɱ� �Ͼ��
	Player_RightSit,
	Player_RightStand,
	Player_LeftSit,
	Player_LeftStand,




	// ����
};

// �ִϸ��̼� �� ��ġ �� �ڽ� ũ�� ���� ����ü
struct SPlayerPosByAnim
{
	float hx;
	float hy;
	float x;
	float y;
	float BoxDeltaScaleX;
	float BoxDeltaScaleY;
};
class Player : public Unit
{

private:
	// �ִϸ��̼�
	class Anim_Player* AnimPlayer;
	// �Է�
	class InputComponent* InputDevice;
	// �÷��̾� ���� ���� ����
	EPlayerState PrevPlayerState;
	EPlayerState CurrentPlayerState;
	float desiredVel = 0;

	// ȯ�� ����..
	float JumpForce = 500.0f;
	float Speed = 500.0f;


	// �ִϸ��̼� ��ġ ����
	std::map<EPlayerState, SPlayerPosByAnim> m_mBox_Pos_Config;
	JGVector2D m_CollisionCenter;
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
	EPlayerState GetCurrentPlayerState();
	void SetCurrentPlayerState(EPlayerState State);
	// �ӽ�..
	void PlayerStateLog();
};