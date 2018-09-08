#pragma once
#include"../../EngineStatics/Engine.h"

class JGDeviceD;
class JGHLSLShaderDevice;
class JGBufferManager;
class JGSuperClass;
class JGViewportD;
class JGCommandManager;
class SoundSystem;
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
	static JGSuperClass* m_SuperClass;
public:
	ComponentBase();
	virtual ~ComponentBase();
	virtual void Receive(const JGComponentMessage& Message) = 0;
	virtual void BeginComponent() = 0;
	virtual void Tick(const float DeltaTime) = 0;
	virtual void Render() = 0;
	virtual void DestoryComponent() = 0;
	void InitComponent(JGSuperClass* SuperClass);
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
	/*
	Exp : CommandManager Ŭ������ ���´�. */
	JGCommandManager* GetCommandManager();
	/*
	Exp : SoundSystem �� �ҷ��´�. */
	SoundSystem* GetSoundSystem();
};