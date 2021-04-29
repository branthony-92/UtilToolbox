#include "stdafx.h"
#include "Logger.h"
#include <time.h>

using namespace std::chrono_literals;

namespace {
	const std::map<Severity, std::string> c_sevReflectionMap = {
		{ Severity::Info,	  "[INFO] "  },
		{ Severity::Debug,    "[DEBUG] " },
		{ Severity::Warning,  "[WARN] "  },
		{ Severity::ErrorL1,  "[ERR1] "  },
		{ Severity::ErrorL2,  "[ERR2] "  },
		{ Severity::ErrorL3,  "[ERR3] "  },
		{ Severity::Fatal,    "[FATAL] " }
	};
}

CLogger::CLogger(std::string logPath)
	: m_logFile()
	, m_logName("")
	, m_logPath(logPath)
	, m_logs()
	, m_logDebugMessage(false)
    , m_logThread()
    , m_writeEnabled(false)
    , m_mutex()
{
}

CLogger::~CLogger()
{
	stopLog();
}

void CLogger::logMsg(CLogMessage log)
{
	if (!m_logDebugMessage.load() && log.getSev() == Severity::Debug)
	{
		return;
	}

	queueMessage(log);
}

void CLogger::startLog()
{
	m_writeEnabled = true;
	initLogFile();

	m_logThread = std::thread([&]() {
		while (m_writeEnabled.load())
		{
			std::this_thread::sleep_for(1000ms);
			writeChunk(1000);
		}
	});
}

void CLogger::stopLog()
{
	m_writeEnabled = false;

	if (m_logThread.joinable())
	{
		m_logThread.join();
	}
	writeChunk();
	if (m_logFile.is_open())
	{
		m_logFile.close();
	}
}

bool CLogger::initLogFile()
{
	if (m_logFile.is_open())
	{
		m_logFile.close();
	}

	std::ostringstream oss;
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm nowTM = *std::localtime(&t);
	char buf[128];
	strftime(buf, sizeof(buf), ("%F"), &nowTM);

	std::string name = m_logName.empty() ? buf : m_logName;
	oss << m_logPath << "APPLOG-" << name << ".log";

	m_logFile.open(oss.str().c_str(), std::ios::out | std::ios::trunc);
	return m_logFile.is_open();
}

void CLogger::writeChunk(int chunkSize)
{
	unsigned int maxWrite = chunkSize;

	TLock lock(m_mutex);
	bool init = true;

	if (!m_logFile.is_open())
	{
		init = initLogFile();
	}

	if (!init) return;

	auto size = m_logs.size();

	for (auto i = 0u; i < size; i++)
	{
		if (!m_logFile.is_open()) break;

		auto log = popFrontMessage();
		if (log.getSev() == Severity::Null) continue;
		auto logStr = constructMessageString(log);

		m_logFile << logStr << "\n";
		if (i > maxWrite) break;
	}
}

void CLogger::queueMessage(CLogMessage msg)
{
	TLock lock(m_mutex);
	m_logs.push_back(msg);
}

CLogMessage CLogger::popFrontMessage()
{
	if (m_logs.empty()) return CLogMessage(Severity::Null, "");

	auto log = m_logs.front();
	m_logs.pop_front();
	return log;
}

void CLogger::purgeMessages()
{
	TLock lock(m_mutex);
	m_logs.clear();
}

std::string CLogger::getTimePointString(TimePoint timePoint)
{
	// todo - determine local
	std::time_t t = std::chrono::system_clock::to_time_t(timePoint);
	std::tm nowTM = *std::localtime(&t);
	char buf[128];

	strftime(buf, sizeof(buf), ("%F %r"), &nowTM);
	return buf;
}

std::string CLogger::constructMessageString(CLogMessage log)
{
	std::ostringstream oss;
	oss << "[" << getTimePointString(log.getTimestamp()) << "]";
	oss << c_sevReflectionMap.at(log.getSev());
	oss << log.getMsg();
	return oss.str();
}