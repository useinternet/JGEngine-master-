#pragma once
#include"EngineFrameWork/Components/Animation2DSystemComponent.h"


// �̹����� ����ƽ �Ž��� �������� �ؼ� �ؽ��ĸ� �ٲ㼭 �¿� ���� �ϰ� ������..
// right Move�� left move �� ��ġ��..
// �״����� player�� ���� Ŀ���� �÷��̾�� �и�..( �÷��̾� �̵��ϰ�.. �浹ü ����)
// Ŀ������ ���ҽ� �� �ִϸ��̼�..

class Player;
enum EAnimState
{
	Anim_Idle,
	Anim_Move,
	Anim_Jump,
	Anim_SitDown,
	Anim_StandUp,
	Anim_DefaultAttack_1,
	Anim_DefaultAttack_2,
	Anim_DefaultSkill,
	Anim_SpecialSkill,
	Anim_JumpDefaultAttack,
	Anim_SitDefaultAttack_1,
	Anim_SitDefaultAttack_2,
	Anim_SitDownSkill,
	Anim_Dead
};
class Anim_Player : public Animation2DSystemComponent
{
private:
	typedef unsigned int PlayerState;
private:
	class AnimationMesh2DComponent* Idle;
	class AnimationMesh2DComponent* Move;
	class AnimationMesh2DComponent* Jump;
	class AnimationMesh2DComponent* SitDown;
	class AnimationMesh2DComponent* StandUp;

	// ���ĵ� ���� ����
	class AnimationMesh2DComponent* DefaultAttack_1;
	class AnimationMesh2DComponent* DefaultAttack_2;
	class AnimationMesh2DComponent* DefaultSkill;
	class AnimationMesh2DComponent* SpecialSkill;

	// ���� ���� ����
	class AnimationMesh2DComponent* JumpDefaultAttack;

	// �ɱ� ���� ����
	class AnimationMesh2DComponent* SitDefaultAttack_1;
	class AnimationMesh2DComponent* SitDefaultAttack_2;
	class AnimationMesh2DComponent* SitDownSkill;

	// ����
	class AnimationMesh2DComponent* Dead;
	class JTimerHandle* DeadTimerHandle;
	bool  bDeadAnimationEnd = false;

	// ��ȸ�� ����
	float DeadAnimAlpha = 1.0f;
	PlayerState PrevPlayerState = 0xFFFFFFF;
public:
	Anim_Player();
	virtual ~Anim_Player();

	virtual void BeginComponent(World* world) override;
	virtual void Tick(const float DeltaTime) override;
	void ConfigDefault(Player* p, EAnimState state);
	void ConfigSitDown(Player* p);
	void ConfigJump(Player* p);
	void ConfigAttack(Player* p);
	void ConfigSitAttack(Player* p);
	void AttackChangeIdleFromEnd();
	void SitAttackChangeSitDownFromEnd();
	void JumpAttackChangeJumpDownFromEnd();
};