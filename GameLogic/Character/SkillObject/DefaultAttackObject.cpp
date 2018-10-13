#include"DefaultAttackObject.h"
#include"EngineFrameWork/Components/StaticMesh2DComponent.h"
#include"EngineFrameWork/Components/Circle2DCollisionComponent.h"
#include"EngineStatics/JGConstructHelper.h"
#include"EnemyUnit/EnemyUnitBase.h"
#include"EngineStatics/JTimerManager.h"
#include"StaticFilter/StaticCollisionFilter.h"
#include"EngineFrameWork/DamageInformation/SingleDamage.h"
#include"EngineStatics/JGLog.h"
DefaultAttackObject::DefaultAttackObject()
{
	RegisterObjectID(typeid(this));

	CircleCollison = RegisterComponentInObject<Circle2DCollisionComponent>(TT("CircleCollision"));
	CircleCollison->SetCategoryFilter(Filter_PlayerAttack);
	CircleCollison->SetMaskFilter(Filter_Enemy);

	CircleCollison->SetBodyType(E2DBodyType::Kinematic);
	CircleCollison->SetRadius(20.0f);
	CircleCollison->SetBeginOverlapEvent([](Object* obj)
	{
		EnemyUnitBase* enemy = dynamic_cast<EnemyUnitBase*>(obj);
		if (enemy)
		{
			SingleDamage dmg;
			dmg.Damage = 10.0f;
			enemy->SendDamage(dmg);
		}
	});
	JTimerEventManager::CreateTimerEvent(&DestroyTimerHandle,
		std::bind(&DefaultAttackObject::DestoryObject,this), EHandleType::EDefault,
		ObjectLife, 0.0f, 1);
}

DefaultAttackObject::~DefaultAttackObject()
{
}

void DefaultAttackObject::BeginObject(World* world)
{
	ExistObject::BeginObject(world);


	CircleCollison->GetBody()->PhysicsOff();


}
void DefaultAttackObject::Tick(const float DeltaTime)
{
	ExistObject::Tick(DeltaTime);

	CircleCollison->AddComponentLocation(1000.0f*DeltaTime, 0.0f);
}

void DefaultAttackObject::SetAttackSpawnLocation(const float x, const float y)
{
	CircleCollison->SetComponentLocation(x, y);
	JGVector2D location = CircleCollison->GetComponentWorldLocation();
	JGAngle2D  angle = CircleCollison->GetComponentWorldAngle();


	CircleCollison->GetBody()->SetTransform(location, angle);
}

void DefaultAttackObject::SetAttackSpawnLocation(const JGVector2D& vec)
{
	CircleCollison->SetComponentLocation(vec);
	JGVector2D location = CircleCollison->GetComponentWorldLocation();
	JGAngle2D  angle = CircleCollison->GetComponentWorldAngle();


	CircleCollison->GetBody()->SetTransform(location, angle);
}
