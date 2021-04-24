#include "stdafx.h"
#include "SMEvent.h"


CSMEvent::CSMEvent(uint32_t id, std::string name, Priority priority)
	: c_eventPriority(priority)
	, c_eventID(id)
	, c_eventName(name)
{
}

CSMEvent::~CSMEvent()
{
}