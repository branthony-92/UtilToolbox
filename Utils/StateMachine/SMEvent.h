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
	virtual ~CSMEvent();

	const Priority		c_eventPriority;
	const uint32_t		c_eventID;
	const std::string   c_eventName;
};
typedef std::shared_ptr<CSMEvent> TEventPtr;

class CSMErrorEvent : public CSMEvent
{
public:
	CSMErrorEvent(uint32_t id, std::string name, Priority priority = Priority::Medium)
		: CSMEvent(id, name, priority)
		, m_errorMsg()
	{}
	virtual ~CSMErrorEvent() {}

	std::string getErrorMsg() const { return m_errorMsg; }
	void setErrorMsg(std::string msg) { m_errorMsg = msg; }

private:
	std::string m_errorMsg;
};
typedef std::shared_ptr<CSMErrorEvent> TErrorEventPtr;
#endif // !SMEVENT_H