#pragma once
#include"../../EngineStatics/Engine.h"

class JGDevice;
class JGHLSLShaderDevice;

/*
Class : JGComponentMessage */
class JGComponentMessage
{
public:
	static const char* ComponentID;
	UINT Flag;
};
class ComponentBase
{
private:
	static const char* ComponentID;
public:
	ComponentBase();
	virtual ~ComponentBase();
	virtual void Receive(const JGComponentMessage& Message) = 0;
	virtual void BeginComponent() = 0;
	virtual void Tick(const float DeltaTime) = 0;
	virtual void Render() = 0;
	virtual void DestoryComponent() = 0;

protected:
	/*
	Exp : ������Ʈ ���̵� ����մϴ�.
	@param : const type_info& ID : typeid(ComponentType)���� �Ķ���͸� �����Ѵ�. */
	static void RegisterComponentID(const type_info& ID);
	/*
	Exp : ������Ʈ���̵� �����ɴϴ�. */
	static const char* GetID();
};