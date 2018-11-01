#include"JGMainCore.h"
#include"JGRenderCore.h"
using namespace JGRC;

JGMainCore::JGMainCore() {}
JGMainCore::~JGMainCore() {}

bool JGMainCore::InitCore(HWND hWnd)
{
	m_JGRC = new JGRenderCore;
	m_JGRC->Init(hWnd);
	return true;
}
void JGMainCore::Run()
{
	MSG msg;
	while (!m_Paused)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			m_Paused = true;
		}
		m_JGRC->Draw();
	}
}
void JGMainCore::Destroy()
{
	delete m_JGRC;
	m_JGRC = nullptr;
}