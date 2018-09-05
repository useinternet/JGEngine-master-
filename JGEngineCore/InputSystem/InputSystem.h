#pragma once
#include"../EngineStatics/Engine.h"

class JGPressManager;
class JGCommandManager;


/*
Class : InputSystem
@m IDirectInput8* m_DirectInput : ���̷�Ʈ �Է� ��ġ ������
@m IDirectInputDevice8* m_KeyBoard : ���̷�Ʈ Ű���� ��ġ ������
@m IDirectInputDevice8* m_Mouse : ���̷�Ʈ ���콺 ��ġ ������
@m unique_ptr<JGPressManager> m_PressManager : Ű�� ���ȴ��� �����ϴ� Ŭ����
@m unique_ptr<JGCommandManager> m_CommandManager : Ű���� �Բ� ���ε��� �̺�Ʈ�Լ����� �����ϴ� Ŭ����
@m MouseState m_MouseState : ���콺 ����
@m KeyBoardState m_KeyBoardState[256] : Ű���� ���� */
class ENGINE_EXPORT InputSystem
{
	typedef unsigned char KeyBoardState;
	typedef DIMOUSESTATE  MouseState;
private:
	IDirectInput8* m_DirectInput;
	IDirectInputDevice8* m_KeyBoard;
	IDirectInputDevice8* m_Mouse;
	std::unique_ptr<JGPressManager> m_PressManager;
	std::unique_ptr<JGCommandManager> m_CommandManager;
	MouseState m_MouseState;
	KeyBoardState m_KeyBoardState[256];

	HWND m_hWnd;
	float m_ScreenWidth;
	float m_ScreenHeight;
public:
	InputSystem();
	~InputSystem();
	/*
	Exp : �Է� ��ġ�� �����Ѵ�. 
	@param HINSTANCE hinst : �Է� �� ������ų ���α׷� �ν��Ͻ� 
	@param HWND hWnd : ������ �ڵ�
	@param const int width : Ŭ���̾�Ʈ ���α���
	@param const int height : Ŭ���̾�Ʈ ���α���*/
	bool CreateInputDevice(HINSTANCE hinst, HWND hWnd,const int width,const int height);
	void Tick();


	JGCommandManager* GetCommandManager();
private:
	bool ReadKeyboard();
	bool ReadMouse();

};