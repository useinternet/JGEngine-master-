#include "pch.h"
#include "JGUIComponent.h"
#include "RenderEngine.h"
#include "GUI/JGUIComponent/JGUIRectTransform.h"
#include "GUI/JGUIComponent/JGUICollider.h"
#include "JGUIWindow.h"
using namespace std;


JGUIWindow* JGUIComponent::GetOwnerWindow() const
{
	return m_OwnerWindow;
}

void JGUIComponent::JGUIAwake()
{

	if (typeid(*this) != typeid(JGUIRectTransform))
	{
		m_RectTransform = CreateJGUIComponent<JGUIRectTransform>("JGUIRectTransform");
	}
	JGUIObject::JGUIAwake();
}
void JGUIComponent::JGUIStart()
{
	for (auto& child : m_ChildComponents)
	{
		child->JGUIStart();
	}
	JGUIObject::JGUIStart();
}
void JGUIComponent::JGUITick(const JGUITickEvent& e)
{
	JGUIObject::JGUITick(e);



	if (m_RectTransform)
	{
		if (m_PrevSize != m_RectTransform->GetSize())
		{
			m_PrevSize = m_RectTransform->GetSize();
			JGUIResizeEvent e;
			e.width = m_PrevSize.x;
			e.height = m_PrevSize.y;
			if (m_Collider)
			{
				m_Collider->GetTransform()->SetSize(e.width, e.height);
			}

			JGUIResize(e);
		}
	}



	for (auto& child : m_ChildComponents)
	{
		child->JGUITick(e);
	}
}
void JGUIComponent::JGUIDestroy()
{
	JGUIObject::JGUIDestroy();

	

	SetParent(nullptr);
	// �ڽ� ����
	for (int i = 0; i < m_ChildComponents.size();)
	{
		JGUI::DestroyObject(m_ChildComponents[i]);
	}
}

void JGUIComponent::JGUIResize(const JGUIResizeEvent& e)
{
	Resize(e);
}

void JGUIComponent::JGUIMouseMove(const JGUIMouseMoveEvent& e)
{
	if (m_IsMouseTracking == false && GetParent())
	{
		m_IsMouseTracking = true;
		JGUIMouseTrack track;
		track.com = this;
		track.flag = JGUI_MOUSETRACKFLAG_MOUSEHOVER;
		track.time = JGUI_DEFAULT_HOVERTRACKTIME;
		JGUI::RegisterMouseTrack(track);
	}

	auto p = JGUI::GetMousePos(GetOwnerWindow()->GetRootWindowHandle());
	for (auto& com : m_ChildComponents)
	{
		if (com->IsActive() && com->GetCollider() && com->GetCollider()->CheckInPoint(p))
		{
			com->JGUIMouseMove(e);
		}
	}
	MouseMove(e);
}

void JGUIComponent::JGUIMouseBtDown(const JGUIKeyDownEvent& e)
{

	auto p = JGUI::GetMousePos(GetOwnerWindow()->GetRootWindowHandle());
	for (auto& com : m_ChildComponents)
	{
		if (com->IsActive() && com->GetCollider() && com->GetCollider()->CheckInPoint(p))
		{
			com->JGUIMouseBtDown(e);
		}
	}
	MouseBtDown(e);
}

void JGUIComponent::JGUIMouseBtUp(const JGUIKeyUpEvent& e)
{
	ENGINE_LOG_INFO(GetName() + " : " + e.ToString());

	auto p = JGUI::GetMousePos(GetOwnerWindow()->GetRootWindowHandle());
	for (auto& com : m_ChildComponents)
	{
		if (com->IsActive() && com->GetCollider() && com->GetCollider()->CheckInPoint(p))
		{
			com->JGUIMouseBtUp(e);
		}
	}
	MouseBtUp(e);
}

void JGUIComponent::JGUIMouseLeave()
{
	if (m_IsMouseTracking)
	{
		m_IsMouseTracking = false;
		MouseLeave();
	}
}

void JGUIComponent::JGUIMouseHover()
{
	if (m_IsMouseTracking)
	{
		JGUIMouseTrack track;
		track.com = this;
		track.flag = JGUI_MOUSETRACKFLAG_MOUSELEAVE;
		track.time = JGUI_DEFAULT_LEAVETRACKTIME;
		JGUI::RegisterMouseTrack(track);
		MouseHover();
	}

}

void JGUIComponent::SetParent(JGUIComponent* parent)
{
	if (m_Parent)
	{
		// ����
		auto& childs = m_Parent->m_ChildComponents;
		childs.erase(std::remove_if(childs.begin(), childs.end(), [&](JGUIComponent* com)
		{
			return com == this;
		}), childs.end());

	}
	if (parent)
	{
		// ����
		auto& childs = parent->m_ChildComponents;
		childs.push_back(this);
	}

	m_Parent = parent;
}
JGUIComponent* JGUIComponent::GetParent() const
{
	return m_Parent;
}
const std::vector<JGUIComponent*>& JGUIComponent::GetChilds() const
{
	return m_ChildComponents;
}
std::vector<JGUIComponent*>& JGUIComponent::GetChilds()
{
	return m_ChildComponents;

}
JGUIComponent* JGUIComponent::FindChild(uint32_t index)
{
	if (m_ChildComponents.size() <= index)
		return nullptr;
	return m_ChildComponents[index];
}
JGUIComponent* JGUIComponent::FindChild(const std::string& name)
{
	for (auto& com : m_ChildComponents)
	{
		if (com->GetName() == name)
			return com;
	}
	return nullptr;
}
void JGUIComponent::RegisterBoxCollider()
{
	m_Collider = CreateJGUIComponent<JGUIBoxCollider>("JGUIBoxCollider");
	m_Collider->SetParent(this);
	m_Collider->GetTransform()->SetSize(GetTransform()->GetSize());
}
void JGUIComponent::Init(const std::string& name, JGUIWindow* owner_window)
{
	SetName(name);
	m_OwnerWindow = owner_window;

}