#pragma once
#include"../EngineStatics/Engine.h"

class JGDeviceD;
class JGViewportD;
class JGHLSLShaderDevice;
class JGBufferManager;
class JGCommandManager;
class SoundSystem;
/*
Class : JGSuperClass
Exp : ������ �ý��ۿ��� �ʱ�ȭ�� ��ġ���� ������ ������!! */
class ENGINE_EXPORT JGSuperClass
{
private:
	JGDeviceD* m_Device;
	JGViewportD* m_Viewport;
	JGHLSLShaderDevice* m_HLSLDevice;
	JGBufferManager* m_BufferManager;
	JGCommandManager* m_CommandManager;
	SoundSystem* m_SoundSystem;
public:
	void LinkPointer(JGDeviceD* Device, JGViewportD* Viewport, JGHLSLShaderDevice* HLSLDevice, JGBufferManager* BufferManager,
		JGCommandManager* CommandManager, SoundSystem* soundSystem);

	JGDeviceD*          GetDevice();
	JGViewportD*        GetViewport();
	JGHLSLShaderDevice* GetHLSLDevice();
	JGBufferManager*    GetBufferManager();
	JGCommandManager*   GetCommandManager();
	SoundSystem* GetSoundSystem();
};