#ifndef SMEVENT_H
#define SMEVENT_H

class CSMEvent
{
public:
	enum class Priority : unsigned int
	{
		Low, 
		Medium,
		High,
		Critical,
	};

	CSMEvent(uint32_t id, std::string name,  Priority priority = Priority::Medium);
	~CSMEvent();

	const Priority		c_eventPriority;
	const uint32_t		c_eventID;
	const std::string   c_eventName;
};
typedef std::shared_ptr<CSMEvent> TEventPtr;
#endif // !SMEVENT_H