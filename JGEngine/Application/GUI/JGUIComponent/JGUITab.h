#pragma once

#include "GUI/JGUIObject/JGUIComponent.h"
#include "GUI/JGUIComponent/JGUIButton.h"
class JGUIWindow;
class JGUIText;
class JGUITabButton;
class JGUITab : public JGUIComponent
{
	friend JGUITabButton;
protected:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Resize(const JGUIResizeEvent& e) override;
	virtual void Tick(const JGUITickEvent& e) override;

	virtual void MouseMove(const JGUIMouseMoveEvent& e) override;
	virtual void MouseLeave() override;
	virtual void Destroy() override;

public:
	void BindWindow(JGUIWindow* win);
	void ActiveTab(JGUIWindow* win);
private:
	void ReAdjustTabBtPos();
private:
	struct TabInfo
	{
		JGUITabButton* bt  = nullptr;
		JGUIWindow* win = nullptr;
		JGUIText*   txt = nullptr;
	};
private:
	class JGUIRectangle*  m_Background = nullptr;
	class JGUIPanel*      m_ClientPanel = nullptr;
	JGUITabButton* m_PressedTabBt = nullptr;
	JVector2Int m_MousePosWhenPressed;
	std::vector<TabInfo>  m_TabInfo;
};


class JGUITabButton : public JGUIButton
{
	friend JGUITab;
protected:
	virtual void Awake() override;
	virtual void OnClick() override;
	virtual void MouseBtUp(const JGUIKeyUpEvent& e) override;
	virtual void MouseBtDown(const JGUIKeyDownEvent& e) override;
private:
	JGUIWindow* m_BindedWindow = nullptr;
	JGUITab*    m_OwnerTab = nullptr;
};


// Tab ���� ���� ����


/*
1. ���� �����ִ� �� ����

- �ǿ� ���ε��� ������� ���� �����ִ� �������� �ڽ��̴�.
- Ȱ��ȭ�� ���� �����ϰ� �ٸ� ���� �켱������ ���� �����ϰų� ��Ȱ��ȭ�Ѵ�.
- 


2. ���̵�
1. Ÿ��Ʋ �� �巡�� �� JGUI���� ������ ������ ����
2. Ŀ�� ��ġ


// -- ���� ����
-- �ؽ�Ʈ �Ͼ������
-- ��ư�� �����Ʃ��� �����̵�� ���� �� ����
-- �����ε� �̵� �����ϰ� 
-- Ÿ��Ʋ�ٰ� ����鼭 �״�� �̵������ϰ� (Ÿ��Ʋ�� ���涧 ��ư ũ�⿡ ����ؼ� Ÿ��Ʋ�� ���� ��ġ ���)
-- ���ֺ��� ������ Ÿ��Ʋ�ٰ� ��ư���� �Ǹ鼭 ��ư �̵�( ��ư�� ����� Ÿ��Ʋ�� ���� ��ġ ����ؼ� ��ġ ���)

*/