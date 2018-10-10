#include"DemoWorldSpawner.h"
#include"EngineFrameWork/World/World.h"
#include"Test/TestGround.h"
#include"Character/Player.h"
#include"EnemyUnit/EnemyUnitBase.h"
#include"Widget/GameWidget/PlayerStatusWidget.h"
DemoWorldSpawner::DemoWorldSpawner()
{
	RegisterObjectID(typeid(this));


}

DemoWorldSpawner::~DemoWorldSpawner()
{
}
// ���� ���� ����, ����, ����
void DemoWorldSpawner::Spawn(World* world)
{
	world->SpawnObject<TestGround>();
	world->SpawnObject<Player>();
	world->SpawnObject<EnemyUnitBase>();
	world->SpawnObject<PlayerStatusWidget>();
}
