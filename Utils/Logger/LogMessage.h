#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

typedef std::chrono::system_clock::time_point TimePoint;

enum class Severity
{
	Null,
	Info,
	Debug,
	Warning,
	ErrorL1,
	ErrorL2,
	ErrorL3,
	Fatal,
};

class CLogMessage
{
public:
	CLogMessage(Severity sev, std::string msg);
	virtual ~CLogMessage();

private:
	std::string m_message;
	Severity	m_severity;
	TimePoint   m_timeStamp;

public:
	std::string getMsg() const   { return m_message; }
	void setMsg(std::string msg) { m_message = msg; }

	Severity getSev() const   { return m_severity; }
	void setSev(Severity sev) { m_severity = sev; }

	TimePoint getTimestamp() const { return m_timeStamp; }

	void resetTimeStamp() { m_timeStamp = std::chrono::system_clock::now(); }
};
typedef std::deque<CLogMessage> TLogQueue;

#endif // !LOGMESSAGE_H