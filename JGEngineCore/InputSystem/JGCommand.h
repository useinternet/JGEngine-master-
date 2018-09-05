#pragma once
#include"../EngineStatics/Engine.h"


// �ҷ��� �� ����
// JGPressManager : �׳� Ű �޾Ƽ� ���Ǵ����� �Ǵ�..
// JGCommand : Ŀ�ǵ� �̸��ϱ� Ű �����ؼ� �̰� ��������( ���������� Ŭ����)
// JGKeyCommand : Ű�� ���Ǵ��� �H������ �����ؼ� �׿��°� �̺�Ʈ 
// JGAxisCommand : �ش� Ű�� ���������ΰ��� �����ϴ°�..


class JGPressManager;
enum class EKey;
/*
Class : JGCommand
@m unique_ptr<wstring> m_CommandName : ���ε��� Ŀ��Ʈ �̸�.
@m EKey m_Key : ���ε� �� Ű..
@m bool m_bIsPressed : Ű�� ���ȴ���.. */
class ENGINE_EXPORT JGCommand
{
private:
	std::unique_ptr<std::wstring> m_CommandName;
	EKey  m_Key;
	bool m_bIsPressed = false;


	JGPressManager* m_pPressManager = nullptr;
public:
	JGCommand(const JGCommand& copy) = delete;
	JGCommand();
	virtual ~JGCommand();
public:
	/*
	Exp : Ŀ�ǵ� ��� 
	@param JGPressManager* PressManager : JGPressManager Ŭ���� ������
	@param const EKey key : ���ε� �� Ű
	@param const wstring CommandName : ���ε��� Ŀ�ǵ� �̸� */
	void RegisterCommand(JGPressManager* PressManager, const EKey key, const std::wstring& CommandName);
	/*
	Exp : 1�����Ӵ� �ѹ� ȣ���ϴ� �Լ�. */
	virtual void Tick();
	/*
	Exp : ���ε� �� Key�� �����´�. */
	EKey GetKey();
	/*
	Exp : ���ε��� Key�����Ѵ�. */
	void SetKey(EKey key);
	/*
	Exp : Key�� ���ȴ��� ���� */
	bool GetIsPressed();
	/*
	Exp : ���ε� �� Ŀ�ǵ� �̸��� �����´�. */
	const std::wstring& GetCommandName();
	/*
	Exp : ���ε� �� Ŀ�ǵ� �̸� �����Ѵ�.*/
	void SetCommandName(const std::wstring& CommandName);
	/*
	Exp : PressManager�� �����´�. */
	JGPressManager* GetPressManager();
};