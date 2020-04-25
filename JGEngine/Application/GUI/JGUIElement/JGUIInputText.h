#pragma once
#include "GUI/JGUIObject/JGUIElement.h"


class JGUIPanel;
class JGUITextMesh;
class JGUIRectangle;


/* Text �и��� �ϱ�

1. Shader �ȿ��� Ư�� Pos ���� ���� �׸��� �ʰ��Ѵ�.
2. Text Ŭ���� �ȿ��� Ư�� Pos��ŭ �������� �̵�


*/
class JGUIInputText : public JGUIElement
{
protected:
	virtual void Awake() override;
	virtual void Start() override;


	virtual void MouseBtDown(const JGUIKeyDownEvent& e) override;
	virtual void KeyDown(const JGUIKeyDownEvent& e) override;
	virtual void KeyUp(const JGUIKeyUpEvent& e) override;
	virtual void Char(const JGUICharEvent& e) override;
	virtual void FocusEnter(const JGUIFocusEnterEvent& e) override;
    virtual void OnFocus() override;
	virtual void FocusExit(const JGUIFocusExitEvent& e) override;
	virtual void Resize(const JGUIResizeEvent& e) override;

public:
	void  SetTextSize(float size);
	float GetTextSize();
	const std::string& GetText();
private:
	void Setting();
	void InputText(const std::string& str);
	void RemoveText();


	void ReviseFlashPosByMousePos();
	void ReviseFlashPosByChar();
	void FlashMove(const JGUIKeyDownEvent& e);
	void Reset();

private:
	JGUIPanel*     m_TextBackGround = nullptr;
	JGUIRectangle* m_SelectedBox    = nullptr;
	JGUIRectangle* m_Flash = nullptr;
	JGUITextMesh*      m_Text  = nullptr;


	// ���� ���� ������
	float         m_PressedTick = 0.0f;
	float         m_FlashMoveTick = 0.0f;
	float         m_MouseDownTick = 0.0f;
	float         m_FlashMoveTime = 1.5f;
	std::string   m_CurrentChar;
	int           m_CurrentFlashTextIndex = 0;
	float         m_TextSize = 16;
	float         m_GapRatio = 1.1f;
	bool          m_InputSwitch = false;


	float  m_FlashTick      = 0.0f;
	float  m_FlashTime      = 0.5f;
	float  m_FlashThickness = 2.5f;
	bool   m_FlashMove      = false;


	JVector2Int m_PrevMousePos;

	int m_SelectedStartTextIndex = INT_MAX;
	int m_SelectedEndTextIndex   = -2;
	bool     m_Selected = false;
};