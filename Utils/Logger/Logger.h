#ifndef LOGGER_H
#define LOGGER_H

#include "LogMessage.h"
#include "ActionContext.h"

typedef std::lock_guard<std::mutex> TLock;

class CLogAction;

class CLogger : public CActionContext
{
public:
	CLogger(std::string logPath = "");
	virtual ~CLogger();

	std::string logMsg(CLogMessage log);

	void enableLog(bool enabled) { TLock lock(m_mutex); m_writeEnabled.store(enabled); }

	std::string getLogFileName() const     { TLock lock(m_mutex); return m_logName; }
	void  setLogFileName(std::string name) { TLock lock(m_mutex); m_logName = name; }

	WriteMode getWriteMode() const    { TLock lock(m_mutex); return m_writeMode; }
	void setWriteMode(WriteMode mode) { TLock lock(m_mutex); m_writeMode = mode; }

	std::string getFilePath();
	TLogQueue getLogs();

	void purgeMessages();
private:
	std::ofstream	 m_logFile;
	std::string		 m_logPath;
	std::string		 m_logName;
	TLogQueue		 m_logs;
	Severity         m_logLevel;
	WriteMode        m_writeMode;

	std::thread			m_logThread;
	std::atomic_bool	m_writeEnabled;
	mutable std::mutex  m_mutex;

private:
	void writeChunk();
	void addLog(CLogMessage log) { TLock lock(m_mutex); m_logs.push_back(log); }
	size_t getLogSize() { TLock lock(m_mutex); return m_logs.size(); }

	std::string getTimePointString(TimePoint timePoint);
	std::string constructMessageString(CLogMessage log);

	std::unique_ptr<CLogAction> m_pLogAction;
};
typedef std::shared_ptr<CLogger> TLoggerPtr;

#endif // !LOGGER_H
