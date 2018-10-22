#pragma once

// �ͽ���Ʈ ����
#ifdef JGMAINCORE_EXPORTS
#define CORE_EXPORT __declspec(dllexport)
#else
#define CORE_EXPORT __declspec(dllimport)
#endif



class CORE_EXPORT JGMainCore
{
private:

public:
	JGMainCore();
	~JGMainCore();

	bool InitCore();
	void Run();
	void Destroy();
};