#include "pch.h"
#include "JGUI.h"
#include "JGUIObject/JGUIObject.h"
#include "JGUIObject/JGUIWindow.h"


#include "JGUIComponent/JGUICollider.h"
#include "JGUIComponent/JGUIRectTransform.h"
#include "JGUIFont/JGUIFontManager.h"


#include "JGUIScreen.h"
#include "RenderEngine.h"
#include "Object/DxObject/RenderDevice.h"

#include "InputEngine.h"
using namespace std;


JGUI* JGUI::sm_GUI = nullptr;
JGUI::JGUI(IE::InputEngine* input)
{
	if (sm_GUI == nullptr)
	{
		sm_GUI = this;
	}
	JWindowManager::BindWindowProcFunc(std::bind(&JGUI::WindowProc, this,
		placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4));
	m_Input = input;
	m_Plugin.Load("JGEngine.dll");

	m_FontManager = make_shared<JGUIFontManager>();
	if (!m_FontManager->FontLoad(m_DefaultFont))
	{
		ENGINE_LOG_ERROR("Failed Default Font Load");
	}


	m_MainWindow = ((JGUIMainWindowFunc)m_Plugin.GetProcAddress("LoadMainWindowForm"))("JGUI");
	m_MainWindow->GetTransform()->SetSize(800, 600);
	m_MainHWND = m_MainWindow->GetRootWindowHandle();
}




void        JGUI::DestroyObject(JGUIObject* obj)
{
	auto& objqueue = sm_GUI->m_ExpectedDestroyObject;
	obj->JGUIDestroy();
	if (obj->GetID() != -1)
	{
		sm_GUI->m_IDQueue.push(obj->GetID());
	}
	sm_GUI->m_MouseTrackMap.erase(obj);
	sm_GUI->m_ExtraEventMap.erase(obj);
	objqueue.push(obj);

}

JGUIScreen* JGUI::ReqeustRegisterJGUIScreen(JGUIWindow* window)
{
	
	auto size = window->GetTransform()->GetSize();
	auto pos = window->GetTransform()->GetPosition();


	auto screen = make_shared<JGUIScreen>(window->GetName() + "_GUIScreen");
	bool result = screen->Init((uint32_t)size.x, (uint32_t)size.y, (uint32_t)pos.x, (uint32_t)pos.y);

	if (!result)
		return nullptr;
	HWND hWnd = screen->GetHandle();
	auto& screenPool = sm_GUI->m_ScreenPool;
	screenPool[hWnd].first = window;
	screenPool[hWnd].second = screen;
	return screen.get();
}
JGUIWindow* JGUI::FindRootJGUIWindow(HWND hWnd)
{
	auto& screenPool = sm_GUI->m_ScreenPool;
	return screenPool[hWnd].first;
}
JGUIScreen* JGUI::FindJGUIScreen(HWND hWnd)
{
	auto& screenPool = sm_GUI->m_ScreenPool;
	return screenPool[hWnd].second.get();
}
void JGUI::RequestDestroyScreen(HWND hWnd)
{
	auto& queue = sm_GUI->m_ExpectedDestroyWindow;
	queue.push(hWnd);
}

void JGUI::ClearExpectedDestroyScreen()
{
	auto& queue = sm_GUI->m_ExpectedDestroyWindow;
	auto& screenPool = sm_GUI->m_ScreenPool;
	while (!queue.empty())
	{
		auto hWnd = queue.front(); queue.pop();
		if (sm_GUI->m_MainHWND == hWnd)
		{
			RE::RenderEngine::GetDevice()->Flush();
		}
		screenPool.erase(hWnd);
	}
}

JGUIWindow* JGUI::GetMainWindow()
{
	return sm_GUI->m_MainWindow;
}

JVector2Int JGUI::GetCursorPos()
{
	JVector2Int result;
	POINT P;
	::GetCursorPos(&P);
	result.x = P.x;
	result.y = P.y;
	return result;
}

JVector2Int JGUI::GetMousePos(HWND hWnd)
{
	POINT P;
	::GetCursorPos(&P);
	::ScreenToClient(hWnd, &P);
	JVector2Int result = { P.x, P.y };
	return result;
}

bool JGUI::GetKeyDown(JGUIWindow* owner_window, KeyCode code)
{
	if (sm_GUI->m_Input->GetHandle() != owner_window->GetRootWindowHandle())
		return false;
	
	return sm_GUI->m_Input->GetKeyDown(code);
}

bool JGUI::GetKeyUp(JGUIWindow* owner_window, KeyCode code)
{
	if (sm_GUI->m_Input->GetHandle() != owner_window->GetRootWindowHandle())
		return false;

	return sm_GUI->m_Input->GetKeyUp(code);
}

void JGUI::InputFlush()
{
	sm_GUI->m_Input->Flush();
}

void JGUI::InputMouseFlush()
{
	sm_GUI->m_Input->MouseFlush();
}

void JGUI::RegisterMouseTrack(const JGUIMouseTrack& mt)
{
	auto& trcak_map = sm_GUI->m_MouseTrackMap;

	if (mt.com)
	{
		trcak_map[mt.com] = mt;
	}
	else if (mt.win)
	{
		trcak_map[mt.win] = mt;
	}
}
void JGUI::RegisterExtraEvent(const JGUIExtraEvent& e)
{
	auto& e_map = sm_GUI->m_ExtraEventMap;
	if (e.com)
	{
		e_map[e.com] = e;
	}
	if (e.win)
	{
		e_map[e.win] = e;
	}
}
JGUIFontManager* JGUI::GetJGUIFontManager()
{
	return sm_GUI->m_FontManager.get();
}
const std::string JGUI::GetDefaultFontName()
{
	return sm_GUI->m_DefaultFont;
}
void JGUI::Update()
{
	while (!m_ExpectedDestroyObject.empty())
	{
		m_ObjectPool.erase(m_ExpectedDestroyObject.front());
		m_ExpectedDestroyObject.pop();
	}


	while (!m_ExpectedCreateObject.empty())
	{
		auto obj = m_ExpectedCreateObject.front();
		m_ExpectedCreateObject.pop();
		m_ObjectPool[obj.get()] = obj;
	}


	static int focus_cnt = 0;
	// Focus
	auto focus_window = FindRootJGUIWindow(GetFocus());
	if (focus_window == nullptr) focus_cnt++;
	else focus_cnt = 0;

	if (focus_window)
	{
		focus_window->JGUIOnFocus();
		auto kdownList = m_Input->GetKeyBoardDownList();
		auto kupList = m_Input->GetKeyBoardUpList();
		auto mdownList = m_Input->GetMouseBtDownList();
		auto mupList = m_Input->GetMouseBtUpList();

		for (auto k : kdownList)
		{
			JGUIKeyDownEvent e;
			e.Code = k;
			focus_window->JGUIKeyDown(e);
		}
		for (auto k : kupList)
		{
			JGUIKeyUpEvent e;
			e.Code = k;
			focus_window->JGUIKeyUp(e);
		}
		for (auto k : mdownList)
		{
			JGUIKeyDownEvent e;
			e.Code = k;
			focus_window->JGUIMouseBtDown(e);
		}
		for (auto k : mupList)
		{
			JGUIKeyUpEvent e;
			e.Code = k;
			focus_window->JGUIMouseBtUp(e);
		}
	}

	for (auto iter = m_MouseTrackMap.begin(); iter != m_MouseTrackMap.end();)
	{
		JGUIWindow* ownerwin = nullptr;
		JGUICollider* collider = nullptr;
		JGUIComponent* com = nullptr;
		JGUIWindow* win = nullptr;

		if (iter->second.com)
		{
			com = iter->second.com;
			collider = com->GetCollider();
			ownerwin = com->GetOwnerWindow();
		}
		if (iter->second.win)
		{
			win = iter->second.win;
			ownerwin = JGUI::FindRootJGUIWindow(win->GetRootWindowHandle());
			if (ownerwin == win) collider = win->GetPanel()->GetCollider();
			else
			{

				collider = win->GetCollider();
			}
		}
		if (focus_cnt > JGUI_FOCUS_RANGE)
		{
			if (iter->second.flag == JGUI_MOUSETRACKFLAG_MOUSELEAVE)
			{
				if (com) com->JGUIMouseLeave();
				else if (win) win->JGUIMouseLeave();
			}
			if (com) com->m_IsMouseTracking = false;
			if (win) win->m_IsTracking = false;
			iter = m_MouseTrackMap.erase(iter);
		}
		else
		{

			bool check = collider->CheckInPoint(ownerwin->GetMousePos());

			switch (iter->second.flag)
			{
			case JGUI_MOUSETRACKFLAG_MOUSEHOVER:
				if (check) iter->second.start = true;
				break;
			case JGUI_MOUSETRACKFLAG_MOUSELEAVE:
				if (check == false) iter->second.start = true;
				break;
			}

			if (iter->second.start) iter->second.tick += GlobalLinkData::GetTick();
			if (iter->second.tick > iter->second.time)
			{
				auto flag = iter->second.flag;
				iter = m_MouseTrackMap.erase(iter);

				switch (flag)
				{
				case JGUI_MOUSETRACKFLAG_MOUSEHOVER:
					if (com) com->JGUIMouseHover();
					else if (win) win->JGUIMouseHover();
					break;
				case JGUI_MOUSETRACKFLAG_MOUSELEAVE:
					if (com) com->JGUIMouseLeave();
					else if (win) win->JGUIMouseLeave();
					break;
				}
			}
			else ++iter;
		}
	}

	for(auto& w_pair : m_ScreenPool)
	{
		auto window = w_pair.second.first;
		if (window == nullptr) continue;
		if (!window->IsActive()) continue;
		if (!window->IsExecuteStartFunc()) window->JGUIStart();
	

		JGUITickEvent e;
		e.deltaTime = GlobalLinkData::GetTick();
		e.totalTime = GlobalLinkData::GetTotalTime();
		e.fps = GlobalLinkData::GetFPS();
		window->JGUITick(e);
	}


	for (auto iter = m_ExtraEventMap.begin(); iter != m_ExtraEventMap.end();)
	{
		switch (iter->second.flag)
		{
		case JGUI_EXTRAEVENT_DEFAULT:
			if (iter->second.bindedFunc) iter->second.bindedFunc(iter->second);
			iter = m_ExtraEventMap.erase(iter);
			break;
		case JGUI_EXTRAEVENT_REPEAT:
			if (iter->second.bindedFunc) iter->second.bindedFunc(iter->second);
			++iter;
			break;
		case JGUI_EXTRAEVENT_EXIT:
			iter = m_ExtraEventMap.erase(iter);
		}
	}

}


LRESULT JGUI::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	JGUIWindow* currentWindow = FindRootJGUIWindow(hWnd);
	TRACKMOUSEEVENT tme;
	static bool bin = false;
	switch (msg)
	{
	case WM_SIZE:
		return WindowProcResize(currentWindow, wParam, lParam);
	case WM_SETFOCUS:
		return WindowSetFocus(currentWindow, wParam);
	case WM_KILLFOCUS:
		return WindowKillFocus(currentWindow, wParam);
	case WM_CHAR:
		return WindowChar(currentWindow, wParam, lParam);
	case WM_KEYDOWN:
		return WindowKeyDownEvent(wParam);
	case WM_KEYUP:
		return WindowKeyUpEvent(wParam);
	case WM_LBUTTONDOWN:
		return WindowKeyDownEvent(KeyCode::LeftMouseButton);
	case WM_LBUTTONUP:
		return WindowKeyUpEvent(KeyCode::LeftMouseButton);
	case WM_RBUTTONDOWN:
		return WindowKeyDownEvent(KeyCode::RightMouseButton);
	case WM_RBUTTONUP:
		return WindowKeyUpEvent(KeyCode::RightMouseButton);
	case WM_MBUTTONDOWN:
		return WindowKeyDownEvent(KeyCode::MouseWheelButton);
	case WM_MBUTTONUP:
		return WindowKeyUpEvent(KeyCode::MouseWheelButton);
	case WM_MOUSEMOVE:
		if (bin == false)
		{
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER;
			tme.hwndTrack = hWnd;
			tme.dwHoverTime = 10;
			TrackMouseEvent(&tme);
		}
		return WindowMouseMove(currentWindow, lParam);
	case WM_MOUSEHOVER:
		bin = true;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = hWnd;
		tme.dwHoverTime = 10;
		TrackMouseEvent(&tme);
		return WindowMouseHover(currentWindow);
	case WM_MOUSELEAVE:
		bin = false;
		return WindowMouseLeave(currentWindow);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT JGUI::WindowProcResize(JGUIWindow* window, WPARAM wParam, LPARAM lParam)
{
	if (window == nullptr) return 0;

	JGUIResizeEvent e;
	e.width = (float)LOWORD(lParam);
	e.height = (float)HIWORD(lParam);
	if (e.width == 0.0f || e.height == 0.0f) return 0;
	window->JGUIResize(e);
	return 0;
}
LRESULT JGUI::WindowSetFocus(JGUIWindow* window, WPARAM wParam)
{
	if (window == nullptr) return 0;
	JGUIFocusEnterEvent e;
	auto prev_focus_window = FindRootJGUIWindow((HWND)wParam);
	e.prevFocus = prev_focus_window;
	window->JGUIFocusEnter(e);
	return 0;
}
LRESULT JGUI::WindowKillFocus(JGUIWindow* window, WPARAM wParam)
{
	if (window == nullptr) return 0;
	JGUIFocusExitEvent e;
	auto next_focus_window = FindRootJGUIWindow((HWND)wParam);
	e.nextFocus = next_focus_window;
	window->JGUIFocusExit(e);
	return 0;
}
LRESULT JGUI::WindowChar(JGUIWindow* window, WPARAM wParam, LPARAM lParam)
{
	if (wParam > 128) return 0;
	auto str = (TCHAR)wParam;
	JGUICharEvent e;
	char c;
	wcstombs(&c, (wchar_t*)& str, 1);
	e.str = c;
	window->JGUIChar(e);
	return 0;
}
LRESULT  JGUI::WindowKeyDownEvent(WPARAM wParam)
{
	m_Input->NotifyKeyDown(KeyCode(wParam));
	return 0;
}

LRESULT JGUI::WindowKeyUpEvent(WPARAM wParam)
{
	m_Input->NotifyKeyUp(KeyCode(wParam));
	return 0;
}

LRESULT JGUI::WindowMouseMove(JGUIWindow* window, LPARAM lParam)
{
	int x = (int)(short)LOWORD(lParam);
	int y = (int)(short)HIWORD(lParam);
	m_Input->NotifyMousePos(x, y);
	JGUIMouseMoveEvent e;
	e.pos = { (float)x,(float)y };
	auto delta =  m_Input->GetMouseDelta();
	e.delta = { (float)delta.x, (float)delta.y };
	window->JGUIMouseMove(e);
	return 0;
}

LRESULT JGUI::WindowMouseHover(JGUIWindow* window)
{
	window->JGUIMouseHover();
	return 0;
}

LRESULT JGUI::WindowMouseLeave(JGUIWindow* window)
{
	window->JGUIMouseLeave();
	return 0;
}

