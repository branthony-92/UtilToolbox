#ifndef LOGGER_H
#define LOGGER_H

#include "LogMessage.h"
#include "LogAction.h"

class CLogger
{
public:
	CLogger(std::string logPath = "");
	virtual ~CLogger();

	void logMsg(CLogMessage log);

	void enableLog(bool enabled) { m_writeEnabled.store(enabled); }

	std::string getLogFileName() const     { return m_logName; }
	void  setLogFileName(std::string name) { m_logName = name; }

private:
	std::ofstream	 m_logFile;
	std::string		 m_logPath;
	std::string		 m_logName;
	TLogQueue		 m_logs;
	Severity         m_logLevel;

	std::thread			m_logThread;
	std::atomic_bool	m_writeEnabled;
	mutable std::mutex  m_mutex;

	typedef std::lock_guard<std::mutex> TLock;

	TLogQueue getLogs() const { TLock lock(m_mutex); return m_logs; }
private:
	void writeChunk(int chunkSize = -1);
	void purgeMessages();

	std::string getTimePointString(TimePoint timePoint);
	std::string constructMessageString(CLogMessage log);

	TLogActionPtr m_pLogAction;
};
typedef std::shared_ptr<CLogger> TLoggerPtr;

#endif // !LOGGER_H