#include"JGEngineMain.h"
#include"JTimerManager.h"
#include"JThreadManager.h"
#include"JGLog.h"
#include"JGInputEvent.h"
#include"../RenderSystem/RenderSystem.h"
#include"../SoundSystem/SoundSystem.h"
#include"../InputSystem/InputSystem.h"
#include"JGSuperClass.h"
#include"../PhysicsSystem/JGBox2D/JGPhysicsSystem.h"
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
	m_PhysicsSystem = make_unique<JGPhysicsSystem>();
	m_ConfigManager = make_unique<JGConfigLoaderManager>();
	m_FontLoader   = make_unique<JGFontLoader>();

	m_GameLoop = make_unique<GameLoop>();
}
JGEngineMain::~JGEngineMain() {}
bool JGEngineMain::Init(HINSTANCE Instance,HWND hWnd)
{
	bool result = true;
	m_EngineTimer->Start();
	// �α� �ʱ�ȭ
	m_EngineLog->InitLog();
	//
	JGLog::Write(ELogLevel::Progress, TT("EngineTimer Init Complete..."));
	JGLog::Write(ELogLevel::Progress, TT("InputEvent Init Complete..."));

	//
	// ������ ������ ���δ� ���߿� JSon���Ϸ� �޾ƿͼ� �����Ѵ�.
	result = m_RenderSystem->InitRenderSystem(hWnd, false, 1920, 1080, 45.0f, 1000.0f, 0.1f);
	if (!result)
	{
		//����ó��
		return false;
	}
	m_ConstructHelper = make_unique<JGConstructHelper>(m_RenderSystem->GetDevice(),
		m_RenderSystem->GetBufferManager());
	result = m_InputSystem->CreateInputDevice(Instance, hWnd,1920, 1080);
	if (!result)
	{
		return false;
	}
	result = m_SoundSystem->CreateSoundSystem();
	JGLog::Write(ELogLevel::Progress, TT("SoundSystem Init Complete..."));
	if (!result)
	{
		JGLog::Write(ELogLevel::Error, TT("SoundSystem Init Falied..."));
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


	m_PhysicsSystem->InitPhysicsSystem(hWnd, m_SuperClass.get());
	JGLog::Write(ELogLevel::Progress, TT("PhysicsSystem Init Complete..."));
	// �������ϵ� �ʱ�ȭ...

	m_ConfigManager->LoadConfig(m_SuperClass.get());
	JGLog::Write(ELogLevel::Progress, TT("ConfigLoad Complete..."));
	// ���� ���� �ʱ�ȭ
	m_GameLoop->InitGameLoop(m_RenderSystem.get(),m_PhysicsSystem.get());
	JGLog::Write(ELogLevel::Progress, TT("GameLoop Init Complete..."));
	//
	m_InputEvent->LinkInputSystem(m_InputSystem.get());



	//
	JGLog::Write(ELogLevel::Progress, TT("Start JGEngine!"));
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
		m_PhysicsSystem->Step(m_EngineTimer->GetDeltaTime());
		m_SoundSystem->Tick();
		JTimerEventManager::Tick(m_EngineTimer->GetDeltaTime());
		m_GameLoop->Tick(m_EngineTimer->GetDeltaTime());
		m_InputSystem->Tick();
		m_RenderSystem->BeginRendering();
		m_PhysicsSystem->Render();
		m_RenderSystem->Render();
		m_RenderSystem->EndRendering();
	}
}
void JGEngineMain::DoEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_InputEvent.get())
	{
		m_InputEvent->DoEvent(message, wParam, lParam);
	}
}
void JGEngineMain::Destroy()
{
	m_RenderSystem.reset();
	m_InputEvent.reset();
	m_EngineLog->Destory();
	m_ThreadManager->Destroy();
	m_EngineTimer->Destroy();
}

GameLoop* JGEngineMain::GetGameLoop()
{
	return m_GameLoop.get();
}
