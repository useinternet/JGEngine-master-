#pragma once
#include"Component.h"
#include"../../InputSystem/JGCommandManager.h"
#include"../../InputSystem/JGKeyCommand.h"
#include"../../InputSystem/JGPressManager.h"

class ENGINE_EXPORT InputComponent : public Component
{
private:
	
public:
	InputComponent(const InputComponent& copy) = delete;
	InputComponent();
	~InputComponent();

	/*
	Exp : Ű ���� Ŀ�ǵ带 �̺�Ʈ ���ε��Ѵ�.
	@param const wstring CommandName : ����� Ŀ�ǵ� �̸�
	@param const EKeyState KeyState : Ű ����
	@param const function<void()>& Event : void() �� ���ε� �� �̺�Ʈ */
	void BindKeyCommand(const std::wstring& CommandName,const EKeyState KeyState, const std::function<void()>& Event);
	/*
	Exp : Axist(��) Ŀ�ǵ带 �̺�Ʈ ���ε��Ѵ�. 
	@param const wstring CommandName : ����� Ŀ�ǵ� �̸�
	@param const function<void(const float)>& Event : void(const float) �� ���ε��� �̺�Ʈ */
	void BindAxisCommand(const std::wstring& CommandName, const std::function<void(const float)>& Event);
};