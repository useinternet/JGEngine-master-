#pragma once
#include"../../EngineStatics/Engine.h"

class Object;
class RenderSystem;
class PhysicsWorld;
/*
Class : World 
@m m_WorldName : ���� �̸� 
@m vector<shared_ptr<Object>> m_vObjects : ������Ʈ �迭 */
class ENGINE_EXPORT World
{
private:
	std::wstring m_WorldName = TT("None");
	std::list<std::shared_ptr<Object>> m_sObjects;
	std::unique_ptr<PhysicsWorld> m_PhysicsWorld;
public:
	World() = delete;
	World(const std::wstring& Name);
	~World();
	/*
	Exp : ������ 1�� ȣ��Ǵ� �Լ� */
	void Tick(const float DeltaTime);
	/*
	Exp : ���� ���� �� �����ϴ� �������� �迭�� �����´�. */
	std::list<std::shared_ptr<Object>>* GetObjectArray();
	/*
	Exp : ����ȿ� �ִ� ������ ����Ŭ������ �����´�. */
	PhysicsWorld* GetPhysicsWorld();
};