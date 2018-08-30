#pragma once
#include"../../EngineStatics/Engine.h"

class JGDeviceD;
class JGHLSLShaderDevice;
class JGBufferManager;
class JGRenderSuperClass;
class JGComponentMessage;
class JGViewportD;
/*
Class : ObjectBase */
class ENGINE_EXPORT ObjectBase
{
private:
	const char* ObjectID;
	static JGRenderSuperClass* m_RenderSuperClass;
public:
	ObjectBase();
	virtual ~ObjectBase();
	virtual void BeginObject() = 0;
	virtual void Send(JGComponentMessage& Message) = 0;
	virtual void Tick(const float DeltaTime) = 0;
	virtual void Render() = 0;
	virtual void DestroyObject() = 0;
	void InitObejct(JGRenderSuperClass* SuperClass);
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
	JGRenderSuperClass* GetRenderSuperClass();

};