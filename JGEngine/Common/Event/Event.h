#pragma once

#include <string>
#include <functional>
#include <unordered_map>
// Event
enum class EventType
{
	WindowEvent,
	AppEvent,
	InputEvent,
	RenderEvent,
	PhysicsEvent,
	SoundEvent,
	GameFrameWorkEvent,
	AsyncEvent,
	CustomEvent,
};
enum EventCategory
{
	EventCategory_None = 0,
	EventCategory_Application = 0x0001,
	EventCategory_InputEngine = 0x0002,
	EventCategory_RenderEngine = 0x0004,
	EventCategory_PhysicsEngine = 0x008,
	EventCategory_SoundEngine = 0x0016,
	EventCategory_Game = 0x0032,
	EventCategory_All = 0xfffffff
};

#define REGISTER_EVENT_TYPE(event_type)  \
 static  EventType GetStaticType() { return EventType::##event_type; } \
 virtual EventType GetEventType() const override { return EventType::event_type;} \
 virtual const char* GetName() const override { return #event_type; } \

#define REGISTER_EVENT_CATEGORY(category) \
 virtual int GetEventCategory() const override { return category; } \


#define CONVERT_EVENT(EventType, Event) *(##EventType*)&##Event  

class Event
{
public:
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName()  const = 0;
	virtual int GetEventCategory() const = 0;
	virtual std::string ToString() const = 0;
public:
	bool IsInCategory(EventCategory gory) {
		return GetEventCategory() & gory;
	}
};


class CustomEvent : Event
{
	friend class EventManager;
private:
	std::string EventName;
	// �Ķ����
public:
	REGISTER_EVENT_TYPE(CustomEvent)
	REGISTER_EVENT_CATEGORY(EventCategory_None)
	virtual std::string ToString() const override {
		return EventName;
	}
	CustomEvent() = default;
	CustomEvent(const std::string& event_name) : EventName(event_name) {}
};

class AsyncEvent : Event
{
	friend class AsyncEventHandler;
	friend class EventManager;
private:
	bool m_IsCompelete;
public:
	std::function<void()> Function;
public:
	REGISTER_EVENT_TYPE(AsyncEvent)
	REGISTER_EVENT_CATEGORY(EventCategory_None)
	virtual std::string ToString() const override {
		return "AsyncEvent";
	}
};



//// Only Write in Cpp file  \\ macro(eventname, return type, param...)
//#define CREATE_DISPATCH_CUSTOMEVENT(Event_Name, Return, ...) \
//class Dispatch_##Event_Name : public CustomEvent  \
//{\
//public: \
//    std::function<##Return(__VA_ARGS__)> Function; \
//public: \
//    Dispatch_##Event_Name(const std::function<##Return(__VA_ARGS__)>& func) : CustomEvent(#Event_Name), Function(func) {} \
//}; \
//
//// Only Write in Cpp file \\ macro(eventname, return type, param...)
//#define DESIRE_DISPATCH_CUSTOMEVENT(Event_Name, Return, ...) \
//CREATE_DISPATCH_CUSTOMEVENT(Event_Name, Return, __VA_ARGS__) \
//class Event_Name \
//{\
//public: \
//	std::function<##Return(__VA_ARGS__)> Excute;\
//public:\
//	Event_Name() {\
//		auto custom_event = *(Dispatch_##Event_Name*)&EventManager::DesireCustomEvent(#Event_Name);\
//		Excute = custom_event.Function; \
//	}\
//};\









// �񵿱� �̺�Ʈ
/*
�񵿱� �̺�Ʈ �Ŵ����� �Լ��� ����ϸ� �񵿱� �̺�Ʈ �ڵ鷯�� �����µ� ���⼭ �ٽ� �ݺ��ϰų� ������ �������� ��� �˼��ִ�.
�񵿱� �̺�Ʈ �Ŵ����� �۷ι� Ŭ����

�̰͵� �ؿ����� ���
*/



// Ŀ���� �̺�Ʈ
/*
Ŀ���� �̺�Ʈ�� �̺�Ʈ�̸��� �Լ��� ����
�ٸ������� Dispatcher�� �̸��� ��� ����� �Լ��� �����ϴ� ��

1. CustomEvent ���� <- �̸� �ߺ� �Ǹ� ���� �α�
2. CreateCustomEvent(EventName, ReturnType, ...Param) ���� �̺�Ʈ �����ϰ� <- ������ cpp���Ϸ�
3. Dispatch_EventName_Event Ŭ���� �ڵ� ������ Dispatcher Ŭ������ ����
4. LoadCustomEvent(EventName, ReturnType, ...Param) ���� �̺�Ʈ �ҷ����� <- ������ cpp���Ϸ�
5. EventName Ŭ������ ReturnType Excute(Param, Param) �Լ� ����

*/

