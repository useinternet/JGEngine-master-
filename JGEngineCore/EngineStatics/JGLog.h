#pragma once
#include"Engine.h"


/*
EnumClass : ELogLevel
@e Default : �⺻ �α�( ������ )
@e Warning : ��� �α�( ����� )
@e Error   : ���� �α�( ������ )
@e Progerss: ���� �α�( �ʷϻ� )
*/
enum class ENGINE_EXPORT ELogLevel
{
	Default,    // ������
	Warning,    // �����
	Error,      // ������
	Progress,   // �ʷϻ�
};
/*
Class : JGLog
@m	static std::queue<SLogMessage> m_MessageQue_1 : �޼����α� ť 1
@m	static std::queue<SLogMessage> m_MessageQue_2 : �޼����α� ť 2
@m  bool m_IsRunning : Ȱ��ȭ ����
*/
class ENGINE_EXPORT JGLog
{
private:
	enum CurrentWorkingQue
	{
		MessageQue_1,
		MessageQue_2
	};
	struct SLogMessage
	{
		D3DXCOLOR Color   = { 0.0f,0.0f,0.0f,0.0f };
		std::wstring Message = TT("None");
	};
private: 
#ifdef _DEBUG
	static std::queue<SLogMessage> m_MessageQue_1;
	static std::queue<SLogMessage> m_MessageQue_2;
	bool m_IsRunning = true;

#endif
public:
	JGLog();
	~JGLog();
	/* 
	Exp : �α׸� Ȱ��ȭ�Ѵ�.*/
	void InitLog();
	/*
	Exp : �α� �ۼ�
	@param ELogLevel level : �α� ����
	@param const wchar_t* LogMessage : �α� �޼���
	@param ... : ��������    */
	static void Write(ELogLevel level,const wchar_t* LogMessage,...);
	/*
	Exp : �α׸� �ı��Ѵ�..*/
	void Destory();
private:
	void Tick();
	void OutputMessage();
private: // �α� ���� ���� ������..
	static bool m_bWorking_1;
	static bool m_bWorking_2;
	static CurrentWorkingQue m_CurrentWorkingQue;
	static D3DXCOLOR Default_Color;
	static D3DXCOLOR Warning_Color;
	static D3DXCOLOR Error_Color;
	static D3DXCOLOR Progress_Color;
	static std::wstring Default_str;
	static std::wstring Warning_str;
	static std::wstring Error_str;
	static std::wstring Progress_str;
};