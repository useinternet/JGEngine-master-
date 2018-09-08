#include"JGEngineMain.h"
#include"JTimerManager.h"
#include"JThreadManager.h"
#include"JGLog.h"
#include"JGInputEvent.h"
#include"../RenderSystem/RenderSystem.h"
#include"../SoundSystem/SoundSystem.h"
#include"../InputSystem/InputSystem.h"
#include"JGSuperClass.h"
#include"JGConfigLoader/JGConfigLoaderManager.h"
#include"../EngineFrameWork/2D/Text/JGFontLoader.h"
#include"JGConstructHelper.h"
#include"../EngineFrameWork/GameLoop.h"



// �ӽ� 
#include"../SoundSystem/JGSound.h"
using namespace std;

JGEngineMain::JGEngineMain()
{
	m_EngineTimer   = JMainTimer::GetInstance();
	m_ThreadManager = JThreadManager::GetInstance();
	m_EngineLog    = make_unique<JGLog>();
	m_InputEvent   = make_unique<JGInputEvent>();
	m_RenderSystem = make_unique<RenderSystem>();
	m_SoundSystem = make_unique<SoundSystem>();
	m_InputSystem  = make_unique<InputSystem>();
	m_SuperClass = make_unique<JGSuperClass>();
	m_ConfigManager = make_unique<JGConfigLoaderManager>();
	m_FontLoader   = make_unique<JGFontLoader>();
	m_ConstructHelper = make_unique<JGConstructHelper>();
	m_GameLoop = make_unique<GameLoop>();

}
JGEngineMain::~JGEngineMain() {}
bool JGEngineMain::Init(HINSTANCE Instance,HWND hWnd)
{
	bool result = true;
	m_EngineTimer->Start();
	// �α� �ʱ�ȭ
	m_EngineLog->InitLog();

	// ������ ������ ���δ� ���߿� JSon���Ϸ� �޾ƿͼ� �����Ѵ�.
	result = m_RenderSystem->InitRenderSystem(hWnd, false, 1920, 1080, 45.0f, 1000.0f, 0.1f);
	if (!result)
	{
		//����ó��
		return false;
	}
	result = m_InputSystem->CreateInputDevice(Instance, hWnd, 1920, 1080);
	if (!result)
	{
		return false;
	}
	result = m_SoundSystem->CreateSoundSystem();
	if (!result)
	{
		return false;
	}
	// ������ ���� Ŭ���� ����
	m_SuperClass->LinkPointer(m_RenderSystem->GetDevice(),
		m_RenderSystem->GetViewPort(),
		m_RenderSystem->GetShaderDevice(),
		m_RenderSystem->GetBufferManager(),
		m_InputSystem->GetCommandManager(),
		m_SoundSystem.get());
	m_RenderSystem->InitObjectProtoType(m_SuperClass.get());

	// �������ϵ� �ʱ�ȭ...
	m_ConfigManager->LoadConfig(m_SuperClass.get());
	// ���� ���� �ʱ�ȭ
	m_GameLoop->InitGameLoop(m_RenderSystem.get());

	return true;
}
void JGEngineMain::Run()
{
	MSG msg;

	while (!m_bPaused)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			m_bPaused = true;
		}
		m_EngineTimer->Tick();
		m_SoundSystem->Tick();
		m_GameLoop->Tick(m_EngineTimer->GetDeltaTime());
		m_InputSystem->Tick();
		m_RenderSystem->Render();
	}
}
void JGEngineMain::DoEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	m_InputEvent->DoEvent(message, wParam, lParam);
}
void JGEngineMain::Destroy()
{
	m_RenderSystem.reset();
	m_InputEvent.reset();
	m_EngineLog->Destory();
	m_ThreadManager->Destroy();
	m_EngineTimer->Destroy();
}