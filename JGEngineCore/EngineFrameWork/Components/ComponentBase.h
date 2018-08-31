#pragma once
#include"../../EngineStatics/Engine.h"

class JGDeviceD;
class JGHLSLShaderDevice;
class JGBufferManager;
class JGRenderSuperClass;
class JGViewportD;
/*
Class : JGComponentMessage */
class ENGINE_EXPORT JGComponentMessage
{
public:
	const char* ComponentID;
	UINT Flag;
};
class ENGINE_EXPORT ComponentBase
{
private:
	const char* ComponentID;
	static JGRenderSuperClass* m_RenderSuperClass;
public:
	ComponentBase();
	virtual ~ComponentBase();
	virtual void Receive(const JGComponentMessage& Message) = 0;
	virtual void BeginComponent() = 0;
	virtual void Tick(const float DeltaTime) = 0;
	virtual void Render() = 0;
	virtual void DestoryComponent() = 0;
	void InitComponent(JGRenderSuperClass* SuperClass);
protected:
	/*
	Exp : ������Ʈ ���̵� ����մϴ�.
	@param : const type_info& ID : typeid(ComponentType)���� �Ķ���͸� �����Ѵ�. */
	void RegisterComponentID(const type_info& ID);
	/*
	Exp : ������Ʈ���̵� �����ɴϴ�. */
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

};