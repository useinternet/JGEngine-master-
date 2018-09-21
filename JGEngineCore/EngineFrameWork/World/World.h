#pragma once
#include"../../EngineStatics/Engine.h"

class Object;
class RenderSystem;
class JGMatrix;
class JGPhysicsWorld;
/*
Class : World 
@m m_WorldName : ���� �̸� 
@m vector<shared_ptr<Object>> m_vObjects : ������Ʈ �迭 */
class ENGINE_EXPORT World
{
private:
	std::wstring m_WorldName = TT("None");
	std::list<std::shared_ptr<Object>> m_sObjects;
	JGPhysicsWorld* m_pyWorld = nullptr;
	std::unique_ptr<JGMatrix> m_ViewMatrix;
public:
	World() = delete;
	World(const std::wstring& Name, JGPhysicsWorld* pyWorld);
	~World();

	/*
	Exp : ������ 1�� ȣ��Ǵ� �Լ� */
	void Tick(const float DeltaTime);
	/*
	Exp : ���� ���� �� �����ϴ� �������� �迭�� �����´�. */
	std::list<std::shared_ptr<Object>>* GetObjectArray();

	template<typename ObjectType>
	Object* SpawnObject();

	void TempViewMatrixInit();
	JGMatrix& GetViewMatrix();
	JGPhysicsWorld* GetPyWorld();
};

template<typename ObjectType>
inline Object* World::SpawnObject()
{
	std::shared_ptr<Object> object = std::make_unique<ObjectType>();
	object->BeginObject(this);
	Object* result = object.get();
	m_sObjects.push_back(move(object));
	return result;
}
