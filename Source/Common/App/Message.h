#pragma once

#include <map>
#include <queue>
#include <shared_mutex>

#define GRAPHICS_ENGINE_MESSAGE_ID "GraphicsEngine"
#define INPUT_ENGINE_MESSAGE_ID    "InputEngine"

class SystemCore;
class Application;
class Message
{
public:
	void*    data      = nullptr;
	uint64_t dataSize  = 0;
	uint64_t msgID     = -1;
	uint64_t receiptID = UINT64_MAX;
};

class MessageBus
{
	friend Application;
	friend SystemCore;
public:
	/*
	function : �޽����� ���޵�����, ���� ���������� �˼� �����ϴ�. */
	void Post(const Message& msg);
	/*
    function : �޽����� ��� ���޵˴ϴ�. */
	void Send(const Message& msg);

	/*
    function : ��ϵ� ID ������ */
	uint64_t GetID(const std::string& name) const;
private:
	/*
	function : Post�� �� �޽����� ó�����ϴ�. */
	void Clear();

	/*
	function : ID �߱�*/
	uint64_t IssueID(const std::string& name, SystemCore* sys);
private:
	std::queue<Message>        m_MsgQueue;
	mutable std::shared_mutex  m_MsgMutex;

	// 
	std::map<uint64_t, SystemCore*> m_Systems;
	std::map<std::string, uint64_t> m_IDMap;
	uint64_t m_IDOffset = 1;
	//
	uint32_t m_ProcessMessagePerFrame = 0;
	uint32_t m_AllowMaxProcessFrame = 5;
	uint32_t m_AllowMinProcessFrame = 2;
};






namespace GraphicsMessage
{
	enum 
	{
		Msg_BeginDraw,
		Msg_EndDraw,
	};
}

namespace GameFramwWorkMessage
{
	enum
	{
		Msg_ReceiveGameWorldTexture,
	};
}

// 0000 0000 0000 0000
// 0000 0000 0000 0000
// 0000 0000 0000 0000
// 0000 0000 0000 0000