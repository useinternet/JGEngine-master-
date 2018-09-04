#pragma once
#include"../EngineStatics/Engine.h"



enum class ENGINE_EXPORT EKey
{
	// 숫자 키
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
	// F숫자 키
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	// NumPad 키
	NumLock, NumPad0, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7,
	NumPad8, NumPad9, NumPadEnter,
	// 기능 키
	Escape, Delete, End, Home, Insert, PgDn, PgUp,
	Shift, Ctrl, Alt, CapsLock, Tab,
	// 알파벳 키
	A, B, C, D, E, F, G, H, I, J, K, L, N, M, O, P, Q, R, S, T,
	U, V, W, X, Y, Z,
	// 마우스
	MouseXAxis, MouseYAxis, MouseLButton, MouseRButton,
	MouseHweelClick, MouseHweelMove
};

class ENGINE_EXPORT JGPressManager
{
	typedef DIMOUSESTATE  MouseState;
	typedef unsigned char KeyBoardState;
private:
	KeyBoardState* m_pKeyBoardState = nullptr;
	MouseState* m_pMouseState = nullptr;
public:
	JGPressManager();
	~JGPressManager();

	void LinkInputSystemKeyBoardState(KeyBoardState* state, MouseState* MouseState);

	bool IsPressed(const EKey Key);
	float GetMouseAxis(const EKey Key);
};