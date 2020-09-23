#pragma once

#include <map>
#include <queue>
#include <shared_mutex>
#include "Data/Builder.h"
#include "Math/JVector.h"

#define GRAPHICS_ENGINE_MESSAGE_ID "GraphicsEngine"
#define GFW_MESSAGE_ID             "GameFrameWork"
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

namespace GameFrameWorkMessage
{
	using GFWInstanceAddr = uint64_t;
	enum
	{
		Msg_RequestEditorCamera,
		Msg_GetSelectGameObject,
		Msg_TestSpawnGameObject




		// Msg_GetWorldInfo,

	};

	struct RequestEditorCamera
	{
	/* Msg_RequestEditorCamera
	* ������ ī�޶� �ν��Ͻ� ��������
	* Snd
	std::wstring        -> ���� �̸�
	* Rcv
	GE::SceneCamera*    -> �� ī�޶�
	GE::Texture*        -> �� �ؽ���
	*/
		struct Snd
		{
			std::wstring GameWorldName;
		};
		struct Rcv
		{
			GFWInstanceAddr SceneCameraAddr;
			GFWInstanceAddr SceneTextureAddr;
		};

		Snd snd;
		Rcv rcv;
	};

	struct GetSelectGameObject
	{
	/* Msg_GetSelectGameObject
	* ���� : ���õ� ���� ������Ʈ�� ���� ���� ���
	* Snd
	JVector2   -> ���콺 ��ġ
	
	* Rcv
	
	GameObject�� ���� ����
	{
		Name
		classID
		EditorLayoutBuilderFunc
		EditorDataBindBuilderFunc
		GameObject �����Ҽ��ִ� Pointer
	}
	
	*/
		struct Snd
		{
			JVector2 mousePos;
		};
		struct Rcv
		{
			std::wstring Name;
			std::string  ClassID;
			EditorLayoutBuilder   LayoutBuilder;
			EditorDataBindBuilder DataBindBuilder;

		};

		Snd snd;
		Rcv rcv;


	};



	/* Msg_TestSpawnGameObject
	* Snd
	meshPath = "";
	matName  = "";
	initlocation;
	initRotation;
	initScale;



	* Rcv
	
	
	*/
	/* Msg_SpawnGameObject
	* �̱���(���� ���� ����)
	* Snd
	



	* Rcv
	
	
	*/
}

// 0000 0000 0000 0000
// 0000 0000 0000 0000
// 0000 0000 0000 0000
// 0000 0000 0000 0000