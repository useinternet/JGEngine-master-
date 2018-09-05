#pragma once
#include"../EngineStatics/Engine.h"

class JGDeviceD;
class JGViewportD;
class JGHLSLShaderDevice;
class JGBufferManager;
class JGCommandManager;
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
public:
	void LinkPointer(JGDeviceD* Device, JGViewportD* Viewport, JGHLSLShaderDevice* HLSLDevice, JGBufferManager* BufferManager,
		JGCommandManager* CommandManager);

	JGDeviceD*          GetDevice();
	JGViewportD*        GetViewport();
	JGHLSLShaderDevice* GetHLSLDevice();
	JGBufferManager*    GetBufferManager();
	JGCommandManager*   GetCommandManager();
};