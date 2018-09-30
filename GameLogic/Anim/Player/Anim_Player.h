#pragma once
#include"EngineFrameWork/Components/Animation2DSystemComponent.h"


// �̹����� ����ƽ �Ž��� �������� �ؼ� �ؽ��ĸ� �ٲ㼭 �¿� ���� �ϰ� ������..
// right Move�� left move �� ��ġ��..
// �״����� player�� ���� Ŀ���� �÷��̾�� �и�..( �÷��̾� �̵��ϰ�.. �浹ü ����)
// Ŀ������ ���ҽ� �� �ִϸ��̼�..
enum EAnimState
{
	Anim_Idle,
	Anim_Move,
	Anim_Jump,
	Anim_SitDown,
	Anim_StandUp
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

	PlayerState PrevPlayerState = 0xFFFFFFF;
public:
	Anim_Player();
	virtual ~Anim_Player();

	virtual void BeginComponent(World* world) override;
	virtual void Tick(const float DeltaTime) override;



	void ConfigIdle(EReverse reverse);
	void ConfigMove(EReverse reverse);
	void ConfigJumpUp(EReverse reverse);
	void ConfigJumpDown(EReverse reverse);
};