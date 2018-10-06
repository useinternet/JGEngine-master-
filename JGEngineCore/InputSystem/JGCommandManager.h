#pragma once
#include"../EngineStatics/Engine.h"

class JGPressManager;
class JGCommand;
class JGVector2D;
enum class EKey;
enum class EKeyState;

/*
Class : JGCommandManager 
@m map<wstring, shared_ptr<JGCommand>> m_mCommands : �����ϴ� Ŀ�ǵ� �迭 */
class ENGINE_EXPORT JGCommandManager
{
private:
	JGPressManager* m_pPressManager = nullptr;
	std::map<std::wstring, std::shared_ptr<JGCommand>> m_mCommands;
public:
	JGCommandManager();
	virtual ~JGCommandManager();
	/*
	Exp : Ŀ�ǵ� �Ŵ��� �ʱ�ȭ */
	void InitCommandManager(JGPressManager* Manager);
	/*
	Exp : 1�����Ӵ� �ѹ� ȣ���ϴ� �Լ� */
	void Tick();
	/*
	Exp : Ű ��� 
	@param const EKey key : ���ε��� Ű 
	@param const wstring& CommandName : ���ε��� Ŀ�ǵ� �̸� */
	void RegisterKeyCommand(const EKey key, const std::wstring& CommandName);
	/*
	Exp : Ű Ŀ�ǵ� �̺�Ʈ ���ε� 
	@param const wstring& CommandName : ���ε��� Ŀ�ǵ� �̸� 
	@param const EKeyState KeyState : Ű ����(���� , �E) 
	@param const function<void()>& Event : void() �� ���ε��� �̺�Ʈ */
	void BindKeyCommand(const std::wstring& CommandName, const EKeyState KeyState, const std::function<void()>& Event);
	/*
	Exp : ���ý� Ŀ�ǵ� �̺�Ʈ ���ε�
	@param const wstring& CommandName : ���ε��� Ŀ�ǵ� �̸�
	@param const function<void(const float)>& Event : void(const float) �� ���ε��� �̺�Ʈ */
	void BindAxisCommand(const std::wstring& CommandName, const std::function<void(const float)>& Event);


	void KillKeyCommand(const std::wstring& CommandName);
	void KillAxisCommand(const std::wstring& CommandName);
	/*
	Exp : ���콺 ��ǥ�� �����´�. */
	JGVector2D& GetMouseLocation();
};