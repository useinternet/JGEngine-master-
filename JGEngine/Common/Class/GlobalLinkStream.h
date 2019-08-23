#pragma once

#include "CommonCore.h"
#include "EngineTimer.h"
#include <memory>
struct GlobalLinkStream
{
	std::function<void(Event& e)>   OnEvent;
	std::shared_ptr<spdlog::logger> Logger;
	std::shared_ptr<EventManager>   EngineEventManager;
	std::shared_ptr<EngineTimer>    _EngineTimer;
};

class GlobalLinkData
{
	static std::shared_ptr<EngineTimer>    _EngineTimer;
public:
	static std::function<void(Event& e)>   OnEvent;
	static std::shared_ptr<spdlog::logger> Logger;
	static std::shared_ptr<EventManager>   EngineEventManager;
	static void Init(const GlobalLinkStream& stream, bool is_app = false)
	{
		OnEvent = stream.OnEvent;
		Logger = stream.Logger;
		EngineEventManager = stream.EngineEventManager;
		_EngineTimer = stream._EngineTimer;
		if(!is_app)
			Log::RegisterLog(Logger);
	}
	static float GetTick(TimeStepType type = TimeStepType::Second);
	static float GetTotalTime(TimeStepType type = TimeStepType::Second);
	static uint32_t GetFPS();
};