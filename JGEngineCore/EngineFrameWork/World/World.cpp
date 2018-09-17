#include"World.h"
#include"../Object/Object.h"
#include"../../Test/TestObject.h"
#include"../../Test/TestWidget.h"

using namespace std;
World::World(const std::wstring& Name)
{
	// �ӽ÷� �׳� �����Ѵ�.
	shared_ptr<Object> object = make_unique<TestObject>();
	object->BeginObject(this);
	m_sObjects.push_back(move(object));
	shared_ptr<Object> Widget = make_unique<TestWidget>();
	Widget->BeginObject(this);
	Widget->ActiveObject();
	m_sObjects.push_back(move(Widget));
}
World::~World()
{
}
void World::Tick(const float DeltaTime)
{
	for (auto object = m_sObjects.begin(); object != m_sObjects.end();)
	{
		if ((*object)->GetObjectState() != EObjectState::Destory)
		{
			(*object)->Tick(DeltaTime);
			object++;
		}
		else
		{
			object = m_sObjects.erase(object);
		}
	}
	
}
list<shared_ptr<Object>>* World::GetObjectArray()
{
	return &m_sObjects;
}
