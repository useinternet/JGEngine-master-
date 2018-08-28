#pragma once
#include"../../EngineStatics/Engine.h"

class JGDevice;
class JGHLSLShaderDevice;
class JGComponentMessage;

/*
Class : ObjectBase */
class ObjectBase
{
private:
	static const char* ObjectID;
public:
	ObjectBase();
	virtual ~ObjectBase();

	virtual void BeginObject() = 0;
	virtual void Send(JGComponentMessage& Message) = 0;
	virtual void Tick(const float DeltaTime) = 0;
	virtual void Render(JGDevice* Device, JGHLSLShaderDevice* HlslDevice) = 0;
	virtual void DestroyObject() = 0;
protected:
	/*
	Exp : ������Ʈ Ÿ�� ���� ���̵� ����Ѵ�. 
	@param : const type_info& ID : typeid(ObjecType) �������� �Ķ���͸� ����. */
	static void RegisterObjectID(const type_info& ID);
	/*
	Exp : ���̵� ���´�. */
	static const char* GetID();
};