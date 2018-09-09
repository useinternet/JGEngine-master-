#include"ExistObject.h"
#include"../Components/StaticMesh2DComponent.h"
#include"../../EngineStatics/JGConstructHelper.h"
#include"../../RenderSystem/JGMaterial/JG2DMesh.h"
#include"../Components/TextComponent.h"

//�̹�
#include"../2D/Text/JGFontLoader.h"
#include"../Components/InputComponent.h"
#include"../../EngineStatics/JGLog.h"
#include"../Components/AnimationMesh2DComponent.h"
#include"../../Test/TestAnim.h"
#include"../Components/SoundComponent.h"
#include"../2D/Widget/ImageBox.h"
#include"../../EngineStatics/JGSuperClass.h"
#include"../2D/Widget/Button.h"
using namespace std;
ExistObject::ExistObject()
{
	RegisterObjectID(typeid(this));



	Ground = RegisterComponentInObject<StaticMesh2DComponent>(TT("Ground"));

	static JGConstructHelper::StaticMesh2D GroundMesh(
		GetDevice(), GetBufferManager(), Ground->GetComponentName(),
		EPivot::TopLeft, TT("../ManagementFiles/Resource/SampleGround.png"));
	if (GroundMesh.Success)
	{
		Ground->SetConstructObject(GroundMesh.Object);
	}


	TestAnimation = RegisterComponentInObject<TestAnim>(TT("Animation"));
	TestAnimation->SetCurrentState(AnimationState::Idle);


	Frame = RegisterComponentInObject<TextComponent>(TT("Text"));
	static JGConstructHelper::TextFont FrameMesh(
		GetDevice(), "../ManagementFiles/Resource/Font/Godic.fnt",
		TT("../ManagementFiles/Resource/Font/Godic_0.png"));
	if (FrameMesh.Success)
	{
		Frame->SetConstructObject(FrameMesh.Object);
	}
	Frame->SetTextColor(1.0f, 0.0f, 0.0f);


	MousePosText = RegisterComponentInObject<TextComponent>(TT("MousePos"));
	static JGConstructHelper::TextFont MousePosMesh(
		GetDevice(), "../ManagementFiles/Resource/Font/Godic.fnt",
		TT("../ManagementFiles/Resource/Font/Godic_0.png"));
	if (MousePosMesh.Success)
	{
		MousePosText->SetConstructObject(MousePosMesh.Object);
	}


	TestAnimationPosText = RegisterComponentInObject<TextComponent>(TT("TestAnimationPosText"));
	static JGConstructHelper::TextFont TestAnimationPosTextMesh(
		GetDevice(), "../ManagementFiles/Resource/Font/Godic.fnt",
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
}
ExistObject::~ExistObject()
{


}
void ExistObject::BeginObject()
{
	Object::BeginObject();
	Frame->SetComponentLocation(300.0f, 500.0f);
	Input->BindKeyCommand(TT("Right"), EKeyState::Down,
		bind(&ExistObject::Right, this));
	Input->BindKeyCommand(TT("Left"), EKeyState::Down,
		bind(&ExistObject::Left, this));
	Input->BindKeyCommand(TT("Up"), EKeyState::Down,
		bind(&ExistObject::Up, this));
	Input->BindKeyCommand(TT("Down"), EKeyState::Down,
		bind(&ExistObject::Down, this));


	Ground->SetComponentLocation(0.0f, 1000.0f);
	Ground->SetComponentScale(2.0f, 1.0f);


	
	// ���� ����
	b2Vec2 gravity(0.0f, 50.0f);
	sampleworld = make_unique<b2World>(gravity);

	GroundBodyDef = make_unique<b2BodyDef>();
	GroundBox = make_unique<b2PolygonShape>();


	GroundBodyDef->position.Set(
		Ground->GetComponentWorldLocation().X(), Ground->GetComponentWorldLocation().Y());
	GroundBody = sampleworld->CreateBody(GroundBodyDef.get());
	GroundBox->SetAsBox(
		(float)Ground->GetTextureWdith() / 2, (float)Ground->GetTextureHeight() / 2);
	GroundBody->CreateFixture(GroundBox.get(), 0.0f);

	AnimBodyDef = make_unique<b2BodyDef>();
	AnimBodyDef->type = b2_dynamicBody;
	AnimBodyDef->position.Set(TestAnimation->GetComponentWorldLocation().X(),
		TestAnimation->GetComponentWorldLocation().Y());

	AnimBody = sampleworld->CreateBody(AnimBodyDef.get());
	AnimBodyBox = make_unique<b2PolygonShape>();
	AnimBodyBox->SetAsBox(50.0f, 50.0f);
	fixtureDef = make_unique<b2FixtureDef>();
	fixtureDef->shape = AnimBodyBox.get();
	fixtureDef->density = 1000.0f;
	fixtureDef->friction = 0.3f;
	fixtureDef->restitution = 0.5f;
	fixture = AnimBody->CreateFixture(fixtureDef.get());
}

void ExistObject::Tick(const float DeltaTime)
{
	Object::Tick(DeltaTime);
	// �ӽ� ������ �˾ƺ���
	float FPS = 1.0f / DeltaTime;
	Frame->SetText(TT("FPS : %d"), (int)FPS);
	JGVector2D animPos = TestAnimation->GetComponentWorldLocation();
	JGVector2D MousePos = GetRenderSuperClass()->GetCommandManager()->GetMouseLocation();
	TestAnimationPosText->SetText(TT(" X : %d  ,  Y : %d  "), (int)animPos.X(), (int)animPos.Y());
	MousePosText->SetText(TT("Mouse X : %d  , Mouse Y : %d  "), (int)MousePos.X(), (int)MousePos.Y());
	
	image->AddBlend(DeltaTime * (-0.3f));

	sampleworld->Step((float)1 / (float)60, 8, 2);
	b2Vec2 position = GroundBody->GetPosition();
	Ground->SetComponentLocation(position.x, position.y);
	b2Vec2 position2 = AnimBody->GetPosition();
	TestAnimation->SetComponentLocation(
		TestAnimation->GetComponentWorldLocation().X(), position2.y);
}
void ExistObject::Right()
{
	TestAnimation->AddComponentLocation(1.0f, 0.0f);
	TestAnimation->SetCurrentState(AnimationState::Right);
}
void ExistObject::Left()
{
	TestAnimation->AddComponentLocation(-1.0f, 0.0f);
	TestAnimation->SetCurrentState(AnimationState::Left);
}
void ExistObject::Up()
{
	b2Vec2 velocity = AnimBody->GetLinearVelocity();
	velocity.y = -100.0f;
	AnimBody->SetLinearVelocity(velocity);
	int remainingJumpSteps = 0;
	remainingJumpSteps = 10;
	if (remainingJumpSteps > 0) {
		AnimBody->ApplyForce(b2Vec2(0, 500), AnimBody->GetWorldCenter(), true);
		remainingJumpSteps--;
	}
	TestAnimation->AddComponentLocation(0.0f, -1.0f);
	TestAnimation->SetCurrentState(AnimationState::Up);
}
void ExistObject::Down()
{
	TestAnimation->AddComponentLocation(0.0f, 1.0f);
	TestAnimation->SetCurrentState(AnimationState::Down);
}





