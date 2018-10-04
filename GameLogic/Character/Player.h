#pragma once
#include"EngineFrameWork/Object/Side_Scroll_Unit.h"
enum class EReSizePivot;

// �÷��̾� ���� �ִ�� ���̱�..


// �÷��̾� ����
enum EPlayerState
{
	Player_Idle,
	Player_Move,
	Player_JumpUp,
	Player_JumpDown,
	Player_SitDown,
	Player_StandUp,
	Player_DefaultAttack,
	Player_DefaultSkill,
	Player_SpecialSkill,
	Player_JumpAttack,
	Player_SitAttack,
	Player_SitSkill
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
class Player : public Side_Scroll_Unit
{

private:
	// �ִϸ��̼�
	class Anim_Player* AnimPlayer;
	// �Է�
	class InputComponent* InputDevice;
	// �÷��̾� ���� ���� ����
	EPlayerState PrevPlayerState;
	EPlayerState CurrentPlayerState;
	bool m_bPlayerFix = false;
	// ȯ�� ����..
	float JumpForce   = 500.0f;
	float Speed       = 500.0f;
	// �ִϸ��̼� ��ġ ����
	std::map<EPlayerState, SPlayerPosByAnim> m_mBox_Pos_Config;
	JGVector2D m_CollisionCenter;

	// ���� ����
	class DefaultAttackComponent* defaultAttack;
	class DefaultSkillComponent*  defaultSkill;
	class SpecialSkillComponent*  specialSkill;
	class SitSkillComponent*      sitSkill;
public:
	Player();
	virtual ~Player();
	virtual void BeginObject(World* world) override;
	virtual void Tick(const float DeltaTime) override;



	// ��Ȳ 
	EPlayerState GetCurrentPlayerState();
	void SetCurrentPlayerState(EPlayerState State);
	bool IsJumping();
	bool IsSitting();
	bool IsMoving();
	void NotifyAttackComplete();
	// ����Ű�� ���� �ൿ
	void Move();
	void Attack(class AttackBaseComponent* com, EPlayerState skill);
	void func_Stop();
	void func_RightMove();
	void func_LeftMove();
	void func_Jump();
	void func_SitDown();
	void func_StandUp();
	void func_DefaultAttack();
	void func_DefaultSkill();
	void func_SpeicalSkill();
	// �ӽ�..
	void PlayerStateLog();
};