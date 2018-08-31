#include"ExistObject.h"
#include"../Components/StaticMesh2DComponent.h"
#include"../../EngineStatics/JGConstructHelper.h"
#include"../../RenderSystem/JGMaterial/JG2DMesh.h"
ExistObject::ExistObject()
{
	RegisterObjectID(typeid(this));

	// ���
	Breath = RegisterComponentInObject<StaticMesh2DComponent>(TT("Breath"));
	

	const JGConstructHelper::StaticMesh2D BreathMesh(GetDevice(),
		GetBufferManager(), Breath->GetComponentName(),
		EPivot::MiddleMiddle, TT("../ManagementFiles/Resource/Breath.png"));
	if (BreathMesh.Success)
	{
		Breath->SetStaticMesh2DComponent(BreathMesh.Object);
	}
	// ���
	Test = RegisterComponentInObject<StaticMesh2DComponent>(TT("Test"));

	const JGConstructHelper::StaticMesh2D TestTexture(GetDevice(),
		GetBufferManager(), Breath->GetComponentName(), EPivot::MiddleMiddle,
		TT("../ManagementFiles/Resource/Sample.jpg"));
	if (TestTexture.Success)
	{
		Test->SetStaticMesh2DComponent(TestTexture.Object);
	}
	Breath->AddChild(Test);

	static int count = 0;
	if (count == 0)
	{
		Breath->SetComponentLocation(0.0f, 0.0f);
		Test->SetComponentLocation(0.0f, 0.0f);
		SetZOrder(10);
	}
	else if (count == 1)
	{
		Breath->SetComponentLocation(300.0f, 300.0f);
		Test->SetComponentLocation(300.0f, 0.0f);
	}
	else if (count == 2)
	{
		Breath->SetComponentLocation(1000, 600.0f);
		Test->SetComponentLocation(100.0f, 0.0f);
	}
	count++;

}
ExistObject::~ExistObject()
{


}

void ExistObject::BeginObject()
{
	Object::BeginObject();



}

void ExistObject::Tick(const float DeltaTime)
{
	Object::Tick(DeltaTime);
	Breath->AddComponentAngle(DeltaTime*3);
	Test->AddComponentAngle(DeltaTime * 2);

}
