#pragma once
#include"../../EngineStatics/Engine.h"

class World;
class JGPhysicsSystem;
/*
Class : WorldManager 
@m map<const wstring, shared_ptr<World>> m_mWorlds : �����ϴ� ���� �迭
@m wstring m_CurrentWorld : ���� ���� �̸�
@m wstring m_PrevWorld : ���� ���� ���� �̸�
@m bool m_bWorldChange : ���� ���尡 �ٲ������ ���� */
class ENGINE_EXPORT WorldManager
{
private:
	std::map<const std::wstring, std::shared_ptr<World>> m_mWorlds;
	std::wstring m_CurrentWorld = TT("None");
	std::wstring m_PrevWorld    = TT("None");
	JGPhysicsSystem* m_pPhysicsSystem;
	bool m_bWorldChange = true;
public:
	WorldManager();
	~WorldManager();
	void InitWorldManager(JGPhysicsSystem* pySystem);
	/*
    Exp : 1 �����Ӵ� �ѹ� ȣ��Ǵ� �Լ� */
	void Tick(const float DeltaTime);
	/*
	Exp : ���� ���带 ���Ѵ�. */
	void SelectWorld(const std::wstring& worldName);
	/*
	Exp : ���带 �߰��Ѵ�. */
	void AddWorld(const std::wstring& worldName);
	/*
	Exp : �ش��̸��� ���� ���带 �����´�.. */
	World* GetWorld(const std::wstring& WorldName);
	/*
	Exp : ���� ���带 �����´�. */
	World* GetCurrentWorld();
	/*
	Exp : ���� ���� �̸��� �����´�.*/
	const std::wstring& GetCurrentWorldName();
	/*
	Exp : ���� ���尡 �ٲ������ ���� */
	bool GetIsChangeWorld();
};