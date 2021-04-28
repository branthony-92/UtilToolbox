#ifndef LOGGER_H
#define LOGGER_H

#include "LogMessage.h"

class CLogger
{
public:
	CLogger(std::string logPath = "");
	virtual ~CLogger();


	void logMsg(CLogMessage log);

	void startLog();
	void stopLog();

	void enableLog(bool enabled) { m_writeEnabled.store(enabled); }
private:
	std::ofstream	 m_logFile;
	std::string		 m_logPath;
	std::string		 m_logName;
	TLogQueue		 m_logs;
	std::atomic_bool m_logDebugMessage;

	std::thread			m_logThread;
	std::atomic_bool	m_writeEnabled;
	mutable std::mutex  m_mutex;

	typedef std::lock_guard<std::mutex> TLock;


private:
	bool initLogFile();
	void writeChunk(int chunkSize = -1);

	void queueMessage(CLogMessage msg);
	CLogMessage popFrontMessage();
	void purgeMessages();

	std::string getTimePointString(TimePoint timePoint);
	std::string constructMessageString(CLogMessage log);
};
typedef std::shared_ptr<CLogger> TLoggerPtr;

#endif // !LOGGER_H