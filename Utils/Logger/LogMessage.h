#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

typedef std::chrono::system_clock::time_point TimePoint;

enum class Severity
{
	Null = -1,

	// General logging
	Info,
	Warning,
	Fatal,
	ErrorL1,

	// Verbose logging
	Debug,
	ErrorL2,

	// logging levels
	Normal  = ErrorL1,
	Verbose = ErrorL2, 
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