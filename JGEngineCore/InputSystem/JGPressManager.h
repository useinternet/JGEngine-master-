#pragma once
#include"../EngineStatics/Engine.h"

class JGVector2D;
/*
EnumClass : EKey 
Exp : Ű ��� (Axis�� �پ��ִ� enum���� Axis���� �Լ������� �̿��Ͽ� ���ε��ؾ��Ѵ�.)*/
enum class ENGINE_EXPORT EKey
{
	// ���� Ű
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
	// F���� Ű
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	// NumPad Ű
	NumLock, NumPad0, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7,
	NumPad8, NumPad9, NumPadEnter,
	// ��� Ű
	Escape, Delete, End, Home, Insert, PgDn, PgUp,
	Shift, Ctrl, Alt, CapsLock, Tab,
	// ���ĺ� Ű
	A, B, C, D, E, F, G, H, I, J, K, L, N, M, O, P, Q, R, S, T,
	U, V, W, X, Y, Z,
	// ���콺
	MouseXAxis, MouseYAxis, MouseLButton, MouseRButton,
	MouseWheelClick, MouseWheelAxis
};
/*
Class : JGPressManager
@m KeyBoardState* m_pKeyBoardState : ���� Ű���� ��Ȳ( ���Ű�� ���ȴ���..)
@m MouseState* m_pMouseState : ���콺 ����( ���콺 ��ư�� ��������.. ������������ǥ�� ���)  */
class ENGINE_EXPORT JGPressManager
{
	typedef DIMOUSESTATE  MouseState;
	typedef unsigned char KeyBoardState;
private:
	KeyBoardState* m_pKeyBoardState = nullptr;
	MouseState* m_pMouseState = nullptr;
	JGVector2D* m_pMousePos = nullptr;
public:
	JGPressManager();
	~JGPressManager();
	/*
	Exp : InputSystem ���� ������ Ű���� �� ���콺 ��ġ ������ �� Ŭ������ Ű������¿� ���콺���¸� �Ѱ��ش�. 
	@param KeyBoardState* state : Ű������� �ּҰ�
	@param MouseState* MouseSate : ���콺 ���� �ּҰ� 
	@param JGVector2D* MousePosPointer : ���콺 ��ǥ �� */
	void LinkInputSystemKeyBoardState(KeyBoardState* state, MouseState* MouseState,JGVector2D* MousePosPointer);
	/*
	Exp : �ش� Ű�� �������� ����*/
	bool IsPressed(const EKey Key);
	/*
	Exp : Axis �Է��� ������� �����´�. */
	float GetAxisKey(const EKey Key);

	/*
	Exp : ���콺 ��ǥ�� �����´�. */
	JGVector2D& GetMouseLocation();
};