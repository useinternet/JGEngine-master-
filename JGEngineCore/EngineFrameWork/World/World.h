#pragma once
#include"../../EngineStatics/Engine.h"

class Object;
class RenderSystem;
class JGMatrix;
class JGPhysicsWorld;
class WorldManager;
class GameMode;
/*
Class : World 
@m m_WorldName : ���� �̸� 
@m vector<shared_ptr<Object>> m_vObjects : ������Ʈ �迭 */
class ENGINE_EXPORT World
{
private:
	WorldManager* m_WorldManager;
private:
	std::wstring m_WorldName = TT("None");
	std::list<std::shared_ptr<Object>> m_sObjects;
	std::unique_ptr<GameMode> m_GameMode;

	JGPhysicsWorld* m_pyWorld = nullptr;
	class Camera2DComponent* m_Camera = nullptr;
	std::unique_ptr<JGMatrix> m_DefaultCamera;
public:
	World() = delete;
	World(const std::wstring& Name, JGPhysicsWorld* pyWorld,WorldManager* manager);
	~World();

	/*
	Exp : ������ 1�� ȣ��Ǵ� �Լ� */
	void Tick(const float DeltaTime);
	/*
	Exp : ���� ���� �� �����ϴ� �������� �迭�� �����´�. */
	std::list<std::shared_ptr<Object>>* GetObjectArray();

	template<typename ObjectType>
	Object* SpawnObject();
	template<typename GameModeType>
	void SetGameMode();
	void SetCurrentViewCamera(class Component* component);


	void TempViewMatrixInit();
	GameMode* GetGameMode();
	JGMatrix& GetViewMatrix();
	JGMatrix& GetDefaultViewMatrix();
	Camera2DComponent* GetCurrentCamera();
	JGPhysicsWorld* GetPyWorld();
	void Clear();
	WorldManager* GetManager();
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
template<typename GameModeType>
inline void World::SetGameMode()
{
	m_GameMode = std::make_unique<GameModeType>();
	m_GameMode->BeginObject(this);
}
