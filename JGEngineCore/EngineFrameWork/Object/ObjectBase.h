#pragma once
#include"../../EngineStatics/Engine.h"

class JGDeviceD;
class JGHLSLShaderDevice;
class JGBufferManager;
class JGSuperClass;
class JGComponentMessage;
class JGViewportD;
class World;
/*
Class : ObjectBase */
class ENGINE_EXPORT ObjectBase
{
private:
	const char* ObjectID;
	int ZOrder = 0;
	static JGSuperClass* m_RenderSuperClass;
public:
	ObjectBase();
	virtual ~ObjectBase();
	virtual void BeginObject(World* world) = 0;
	virtual void Send(JGComponentMessage& Message) = 0;
	virtual void Tick(const float DeltaTime) = 0;
	virtual void Render() = 0;
	virtual void EndObject() = 0;
	void InitObejct(JGSuperClass* SuperClass);


	/*
	Exp : ZOrder ���� �ҷ��´�. */
	int GetZOrder();
	/*
	Exp : ZOrder ���� �����Ѵ�. */
	void SetZOrder(int Num);
protected:
	/*
	Exp : ������Ʈ Ÿ�� ���� ���̵� ����Ѵ�. 
	@param : const type_info& ID : typeid(ObjecType) �������� �Ķ���͸� ����. */
	void RegisterObjectID(const type_info& ID);
	/*
	Exp : ���̵� ���´�. */
	const char* GetID();

	/*
	Exp : JGDevice Ŭ������ ���´�. */
	JGDeviceD* GetDevice();
	/*
	Exp : JGHLSLShaderDevice Ŭ������ ���´�. */
	JGHLSLShaderDevice* GetHLSLDevice();
	/*
	Exp : JGBufferManager Ŭ������ ���´�. */
	JGBufferManager* GetBufferManager();
	/*
	Exp : JGViewportD Ŭ������ ���´�. */
	JGViewportD* GetViewport();
	/*
	Exp : JGViewportD Ŭ������ ���´�. */
	JGSuperClass* GetRenderSuperClass();


};