#include"JGEngineMain.h"
#include"JTimerManager.h"
#include"JThreadManager.h"
#include"JGLog.h"
#include"JGInputEvent.h"
#include"../RenderSystem/RenderSystem.h"
#include"JGConstructHelper.h"
#include"../EngineFrameWork/GameLoop.h"
using namespace std;

JGEngineMain::JGEngineMain()
{
	m_EngineTimer   = JMainTimer::GetInstance();
	m_ThreadManager = JThreadManager::GetInstance();


	m_EngineLog    = make_unique<JGLog>();
	m_InputEvent   = make_unique<JGInputEvent>();
	m_RenderSystem = make_unique<RenderSystem>();
	m_ConstructHelper = make_unique<JGConstructHelper>();
	m_GameLoop = make_unique<GameLoop>();
}
JGEngineMain::~JGEngineMain() {}
bool JGEngineMain::Init(HWND hWnd)
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
		m_GameLoop->Tick(m_EngineTimer->GetDeltaTime());
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