#pragma once

#include "GUI/JGUIObject/JGUIWindow.h"


JGUI_MAIN_WINFORM(JGUIForm)


// �������� ������ ���� ������ ��ġ �� �������� �������� ������ ���� ��ġ����� �̻���
// Dock�� Dragging ���̰� ������ Panel�� �ε��������� Dock Ű��
// ������ũ�Ⱑ ���ϸ� Dock�� �ִ� ������ ũ�⵵ ����
// Left�� Right �� �ϳ��� ä������ ���Ʒ� �Ҷ� �Ʒ��� �Ұ�
// �� �Ʒ� �� �ϳ��� ä������ ������ ���� �Ұ�
class JGUIForm : public JGUIWindow
{
	class JGUITitleBar*       m_Title     = nullptr;
	class JGUIResizeBox*      m_ResizeBox = nullptr;
	class JGUIEmptyRectangle* m_Border    = nullptr;
	class JGUIMenu*           m_Menu      = nullptr;
	class JGUIDock*           m_Dock      = nullptr;
	bool m_DockSwitch = false;
//	class JGUITab*            m_Tab       = nullptr;

	//TestCode
	class JGUIText* m_Txt = nullptr;
	class JGUIEmptyRectangle* m_text = nullptr;
	
	JGUIWindow* testwindow = nullptr;
protected:
	virtual void Awake() override;
	virtual void Tick(const JGUITickEvent& e) override;

	virtual void Resize(const JGUIResizeEvent& e) override;
	virtual void KeyDown(const JGUIKeyDownEvent& e) override;
	virtual void KeyUp(const JGUIKeyUpEvent& e) override;
	virtual void ProcessByWindowFlags(EJGUI_WindowFlags flag) override;
public:
	JGUITitleBar* GetTitleBar() const;
	JGUIDock* GetDock() const;
//	JGUITab* GetTab() const;
	JGUIRect GetClientRect() const;
	void DockOn();
	void DockOff();
};





