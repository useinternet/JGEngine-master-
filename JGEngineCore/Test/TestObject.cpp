#include"TestObject.h"
#include"../EngineFrameWork/Components/StaticMesh2DComponent.h"
#include"../EngineStatics/JGConstructHelper.h"
#include"../RenderSystem/JGMaterial/JG2DMesh.h"
#include"../EngineFrameWork/Components/TextComponent.h"

//�̹�
#include"../EngineFrameWork/2D/Text/JGFontLoader.h"
#include"../EngineFrameWork/Components/InputComponent.h"
#include"../EngineStatics/JGLog.h"
#include"../EngineFrameWork/Components/AnimationMesh2DComponent.h"
#include"TestAnim.h"
#include"../EngineFrameWork/Components/SoundComponent.h"
#include"../EngineStatics/JGSuperClass.h"
#include"../EngineFrameWork/2D/Widget/ImageBox.h"
#include"../EngineFrameWork/World/World.h"
#include"../PhysicsSystem/JGBox2D/JGPhysicsSystem.h"
#include"../PhysicsSystem/JGBox2D/JGDynamics/JG2DBody.h"
#include"../PhysicsSystem/JGBox2D/JGShape/JGPolygonShape.h"
#include"../EngineFrameWork/Components/Box2DCollisionComponent.h"
#include"../PhysicsSystem/JGBox2D/JGShape/JGCircle.h"
#include"TestCircle.h"
using namespace std;
TestObject::TestObject()
{
	RegisterObjectID(typeid(this));

	TestAnimation = RegisterComponentInObject<TestAnim>(TT("Animation"));
	TestAnimation->SetCurrentState(AnimationState::Idle);

	Frame = RegisterComponentInObject<TextComponent>(TT("Text"));
	static JGConstructHelper::TextFont FrameMesh("../ManagementFiles/Resource/Font/Godic.fnt",
		TT("../ManagementFiles/Resource/Font/Godic_0.png"));
	if (FrameMesh.Success)
	{
		Frame->SetConstructObject(FrameMesh.Object);
	}
	Frame->SetTextColor(1.0f, 0.0f, 0.0f);

	MousePosText = RegisterComponentInObject<TextComponent>(TT("MousePos"));
	static JGConstructHelper::TextFont MousePosMesh("../ManagementFiles/Resource/Font/Godic.fnt",
		TT("../ManagementFiles/Resource/Font/Godic_0.png"));
	if (MousePosMesh.Success)
	{
		MousePosText->SetConstructObject(MousePosMesh.Object);
	}


	TestAnimationPosText = RegisterComponentInObject<TextComponent>(TT("TestAnimationPosText"));
	static JGConstructHelper::TextFont TestAnimationPosTextMesh("../ManagementFiles/Resource/Font/Godic.fnt",
		TT("../ManagementFiles/Resource/Font/Godic_0.png"));
	if (TestAnimationPosTextMesh.Success)
	{
		TestAnimationPosText->SetConstructObject(MousePosMesh.Object);
	}
	TestAnimationPosText->SetComponentLocation(0.0f, 50.0f);


	// �Է� ����
	Input = RegisterComponentInObject<InputComponent>(TT("SampleInput"));


	image = RegisterComponentInObject<ImageBox>(TT("SampleImageBox"));
	image->CreateImage(TT("../ManagementFiles/Resource/Breath.png"), EPivot::TopLeft);




	// ���� ����
	BoxComponent = RegisterComponentInObject<Box2DCollisionComponent>(TT("SampleCollision"));
	BoxComponent->SetAsBox(50.0f, 50.0f);
	BoxComponent->SetDensity(100.0f);
	BoxComponent->SetComponentLocation(800.0f, 10.0f);
	BoxComponent->AddChild(TestAnimation);


}
TestObject::~TestObject()
{

}
void TestObject::BeginObject(World* world)
{
	ExistObject::BeginObject(world);
	Frame->SetComponentLocation(300.0f, 500.0f);
	Input->BindKeyCommand(TT("Right"), EKeyState::Down,
		bind(&TestObject::Right, this));
	Input->BindKeyCommand(TT("Left"), EKeyState::Down,
		bind(&TestObject::Left, this));
	Input->BindKeyCommand(TT("Up"), EKeyState::Down,
		bind(&TestObject::Up, this));
	Input->BindKeyCommand(TT("Down"), EKeyState::Down,
		bind(&TestObject::Down, this));
	Input->BindKeyCommand(TT("Spawn"), EKeyState::Down,
		bind(&TestObject::Spawn, this));

	BoxComponent->GetBody()->PhysicsOff();
}

void TestObject::Tick(const float DeltaTime)
{
	ExistObject::Tick(DeltaTime);
	TestAnimation->AddComponentAngle(DeltaTime * 10);
	// �ӽ� ������ �˾ƺ���
	float FPS = 1.0f / DeltaTime;
	Frame->SetText(TT("FPS : %d"), (int)FPS);
	JGVector2D animPos = TestAnimation->GetComponentWorldLocation();
	JGVector2D MousePos = GetRenderSuperClass()->GetCommandManager()->GetMouseLocation();
	image->AddBlend(DeltaTime * (-0.3f));
	TestAnimationPosText->SetText(TT(" X : %d  ,  Y : %d  "), (int)animPos.X(), (int)animPos.Y());
	MousePosText->SetText(TT("Mouse X : %d , Mouse Y : %d"), (int)MousePos.X(), (int)MousePos.Y());




	JGVector2D gravity = GetWorld()->GetPyWorld()->GetGravity();
	JGVector2D vel = BoxComponent->GetBody()->GetLinearVelocity();
	float velChangeX = BoxComponent->GetVelocity().X() - vel.X();
	JGVector2D Impulse(BoxComponent->GetBody()->GetMass() * velChangeX,0.0f);
	BoxComponent->GetBody()->ApplyLinearImpulse(Impulse);



	if (sample != PrevSample && sample)
	{
		GetWorld()->SpawnObject<TestCircle>();
	}

	PrevSample = sample;
	sample = false;
}
void TestObject::Right()
{
	BoxComponent->AddComponentLocation(300.0f * GetDeltaTime(), 0.0f);
	TestAnimation->SetCurrentState(AnimationState::Right);

}
void TestObject::Left()
{
	BoxComponent->AddComponentLocation(-300.0f * GetDeltaTime(), 0.0f);
	TestAnimation->SetCurrentState(AnimationState::Left);
}
void TestObject::Up()
{
	BoxComponent->AddComponentLocation(0.0f, -300.0f * GetDeltaTime());
	TestAnimation->SetCurrentState(AnimationState::Up);
}
void TestObject::Down()
{
	BoxComponent->AddComponentLocation(0.0f, 300.0f * GetDeltaTime());
	TestAnimation->SetCurrentState(AnimationState::Down);
}

void TestObject::Spawn()
{
	sample = true;
}




